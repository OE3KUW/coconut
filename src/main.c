#include <avr/io.h>

#include "ccn.h"

#define LED_PIN PB5
#define LED_TIMER_TICKS (1000u / CCN_TICK_MS)

static ccn_timer_t led_timer;

static void led_task(void)
{
    if (ccn_timer_expired(led_timer)) {
        PINB = _BV(LED_PIN);
    }
}

int main(void)
{
    ccn_task_t task;

    DDRB |= _BV(LED_PIN);
    ccn_init();

    task = ccn_task_create(led_task, 1u);
    led_timer = ccn_timer_create(LED_TIMER_TICKS, true);
    if (task == CCN_INVALID_ID || led_timer == CCN_INVALID_ID ||
        !ccn_timer_connect(led_timer, task) || !ccn_timer_start(led_timer)) {
        for (;;) {
        }
    }

    ccn_start();
}
