#include <avr/interrupt.h>
#include <avr/io.h>

#include "ccn.h"
#include "sioi_if.h"

#if F_CPU != 16000000UL
#error "The Uno timer setup expects a 16 MHz clock."
#endif

#define TIMER1_PRESCALER 64UL
#define TIMER1_COMPARE ((F_CPU / TIMER1_PRESCALER / (1000UL / CCN_TICK_MS)) - 1UL)

#if TIMER1_COMPARE > 65535UL
#error "Timer1 compare value does not fit in 16 bits."
#endif

extern volatile uint32_t pending_ticks;
extern volatile bool tick_overflow;

void ccn_timer_hardware_init(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = (uint16_t)TIMER1_COMPARE;
    TIFR1 = _BV(OCF1A);
    TIMSK1 = _BV(OCIE1A);
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
}

ISR(TIMER1_COMPA_vect)
{
    SIOI_TickFromISR();
    if (pending_ticks != UINT32_MAX) {
        pending_ticks++;
    } else {
        tick_overflow = true;
    }
}
