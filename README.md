# <o<onut (CCN) on Arduino Uno with PlatformIO

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

## Serial demonstration

The firmware uses UART at 9600 baud (8N1). Connect a serial terminal to the
Uno's USB serial port. The built-in LED on pin 13 toggles about once per
second.

- Send `e` to wake the output task through a CCN event.
- Send `m` to wake it with a mailbox message containing elapsed whole seconds.
- Send `w` to wake it directly.
- Send `t` to start a one-shot timer; after three seconds it wakes the output
  task, which reports the timer expiration.

The output task prints a different message for each wake-up reason. UART
receive data is buffered by an interrupt handler; a periodic CCN task checks
the buffer. The serial module derives elapsed seconds from the Timer1 tick.
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
- Mailboxes hold four 32-bit messages; sending to a full mailbox returns
  `false`.
- Timer delays are expressed in 8 ms ticks. Periodic timer notifications are
  coalesced if a task has not processed the previous notification yet.
- The serial command `t` starts or restarts a one-shot 3-second timer.
- If the pending hardware-tick counter reaches its maximum, it saturates and
  `ccn_tick_overflowed()` reports the condition.

## Project layout

- `src/` contains the application entry point (`main.c`).
- `coconut/` contains the CCN kernel sources and their module headers. The
  PlatformIO build compiles every `.c` file in this folder. The legacy BPCI
  entry point is disabled with `CCN_NO_MAIN`, leaving `src/main.c` as the
  firmware entry point.
- `include/` contains the public CCN header and all interface headers ending
  in `_if.h`, plus shared compatibility headers for the legacy modules and
  the serial interface.

The PlatformIO source directory is set to the project root so that sources
from both `src/` and `coconut/` can be selected. `build_src_filter` selects
`src/main.c` and all `coconut/*.c` files. The legacy APIs are compiled for
AVR and their module state is initialized in `main.c` in dependency order.
The demo tasks and event handling use the newer `ccn_*` API.

See [Dokumentation_coconut.docx](./Dokumentation_coconut.docx) for the scheduler,
events, mailboxes, timers, serial commands, UML sequence diagrams, flowcharts,
and API appendices. Editable UML sources are in `docs/uml/`.
