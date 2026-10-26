# CCN on Arduino Uno with PlatformIO

This project targets an Arduino Uno with an ATmega328P running at 16 MHz. It
uses PlatformIO and AVR-GCC directly. It does not use the Arduino framework or
Arduino libraries.

## Build and upload

Open this folder as a PlatformIO project in VS Code, then run **PlatformIO:
Build** and **PlatformIO: Upload**. The command-line equivalents are:

```text
pio run -e uno
pio run -e uno -t upload
```

The upload environment is configured for the Uno bootloader over USB. Connect
the board with a USB data cable; PlatformIO normally detects the serial port
automatically. If more than one serial device is connected, set `upload_port`
in `platformio.ini` to the correct port. Do not change fuse bits for this setup.

## Current hardware demonstration

The firmware toggles the Uno's built-in LED on pin 13 about once per second.
The demonstration uses a periodic CCN timer and the cooperative scheduler.
Timer1 is reserved by CCN.

Timer1 runs in CTC mode with a prescaler of 64. At 16 MHz its counter clock is
250 kHz. The compare value is 1999, so each interrupt is exactly
`(1999 + 1) / 250000 s = 8 ms`. The ISR only increments a pending-tick counter;
timer expiry handling and task execution happen in the main loop.

## Kernel behavior

- Tasks are short, cooperative functions and return to the scheduler.
- A larger numeric priority is higher priority.
- Equal-priority ready tasks are selected round-robin.
- Waking an already-ready task does not create a duplicate activation.
- Repeated events are coalesced into one pending event.
- Mailboxes hold four one-byte messages; sending to a full mailbox returns
  `false`.
- Timer delays are expressed in 8 ms ticks. Periodic timer notifications are
  coalesced if a task has not processed the previous notification yet.
- If the pending hardware-tick counter reaches its maximum, it saturates and
  `ccn_tick_overflowed()` reports the condition.

## Project layout

- `src/` contains the application entry point (`main.c`).
- `coconut/` contains the CCN kernel sources and their module headers. The
  PlatformIO build compiles `ccn.c` and `timer_avr.c`; the historical module
  sources are retained here but are not part of the current build.
- `include/` contains the public CCN header and all interface headers ending
  in `_if.h`.

The PlatformIO source directory is set to the project root so that sources
from both `src/` and `coconut/` can be selected. `build_src_filter` explicitly
limits the build to the application and active kernel sources.
