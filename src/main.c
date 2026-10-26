#include <avr/io.h>
#include <stdio.h>

#include "bpci_if.h"
#include "ccn.h"
#include "evti_if.h"
#include "mbxi_if.h"
#include "quei_if.h"
#include "sioi_if.h"
#include "timi_if.h"

#define LED_PIN PB5
#define LED_TIMER_TICKS (1000 / CCN_TICK_MS)
#define SERIAL_POLL_TICKS 1

static ccn_task_t output_task_id;
static ccn_event_t input_event;
static ccn_mailbox_t input_mailbox;
static ccn_timer_t serial_poll_timer;
static ccn_timer_t led_timer;
static bool direct_wake_pending;

static void output_task(void)
{
    bool handled = false;
    ccn_message_t timestamp;

    if (ccn_event_receive(input_event)) {
        printf("Event empfangen.\n");
        handled = true;
    }

    while (ccn_mailbox_receive(input_mailbox, &timestamp)) {
        printf("Mailbox-Nachricht: %lu Sekunden seit Start.\n",
               (unsigned long)timestamp);
        handled = true;
    }

    if (direct_wake_pending) {
        direct_wake_pending = false;
        printf("Task direkt aufgeweckt.\n");
        handled = true;
    }

    if (!handled) {
        printf("Task ohne ausstehendes Ereignis aufgeweckt.\n");
    }
}

static void serial_input_task(void)
{
    int character;

    if (!ccn_timer_expired(serial_poll_timer)) {
        return;
    }

    while ((character = SIOI_ReadChar()) >= 0) {
        switch (character) {
        case 'e':
        case 'E':
            if (!ccn_event_send(input_event)) {
                printf("Event konnte nicht gesendet werden.\n");
            }
            break;
        case 'm':
        case 'M':
            if (!ccn_mailbox_send(input_mailbox, SIOI_Seconds())) {
                printf("Mailbox voll; Nachricht verworfen.\n");
            }
            break;
        case 'w':
        case 'W':
            direct_wake_pending = true;
            (void)ccn_task_wake(output_task_id);
            break;
        default:
            break;
        }
    }
}

static void led_task(void)
{
    if (ccn_timer_expired(led_timer)) {
        PINB = _BV(LED_PIN);
    }
}

int main(void)
{
    ccn_task_t serial_task;
    ccn_task_t led_task_id;

    DDRB |= _BV(LED_PIN);
    SIOI_Init();
    BPCI_Init();
    QUEI_Init();
    MBXI_Init();
    EVTI_Init();
    TIMI_Init();
    ccn_init();

    output_task_id = ccn_task_create(output_task, 2);
    serial_task = ccn_task_create(serial_input_task, 1);
    led_task_id = ccn_task_create(led_task, 1);
    input_event = ccn_event_create(output_task_id);
    input_mailbox = ccn_mailbox_create(output_task_id);
    serial_poll_timer = ccn_timer_create(SERIAL_POLL_TICKS, true);
    led_timer = ccn_timer_create(LED_TIMER_TICKS, true);
    if (output_task_id == CCN_INVALID_ID || serial_task == CCN_INVALID_ID ||
        led_task_id == CCN_INVALID_ID || input_event == CCN_INVALID_ID ||
        input_mailbox == CCN_INVALID_ID ||
        serial_poll_timer == CCN_INVALID_ID || led_timer == CCN_INVALID_ID ||
        !ccn_timer_connect(serial_poll_timer, serial_task) ||
        !ccn_timer_connect(led_timer, led_task_id) ||
        !ccn_timer_start(serial_poll_timer) || !ccn_timer_start(led_timer)) {
        for (;;) {
        }
    }

    printf("<o<onut bereit. Tasten: e = Event, m = Sekunden-Nachricht, w = direkt.\n");
    ccn_start();
}
