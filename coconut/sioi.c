#include "sioi.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/atomic.h>

#include "ccn.h"

#define SIOI_BAUD 9600UL
#define SIOI_UBRR ((F_CPU / (16UL * SIOI_BAUD)) - 1UL)
#define SIOI_RX_BUFFER_SIZE 32
#define SIOI_RX_BUFFER_MASK (SIOI_RX_BUFFER_SIZE - 1)
#define SIOI_TICKS_PER_SECOND (1000 / CCN_TICK_MS)

static volatile uint8_t rx_buffer[SIOI_RX_BUFFER_SIZE];
static volatile uint8_t rx_head;
static volatile uint8_t rx_tail;
static volatile uint8_t ticks_in_second;
static volatile uint32_t uptime_seconds;

static int serial_putchar(char character, FILE *stream)
{
    (void)stream;

    if (character == '\n') {
        while ((UCSR0A & _BV(UDRE0)) == 0) {
        }
        UDR0 = '\r';
    }

    while ((UCSR0A & _BV(UDRE0)) == 0) {
    }
    UDR0 = (uint8_t)character;
    return 0;
}

static FILE serial_output = FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);

void SIOI_Init(void)
{
    rx_head = 0;
    rx_tail = 0;
    ticks_in_second = 0;
    uptime_seconds = 0;

    UBRR0H = (uint8_t)(SIOI_UBRR >> 8);
    UBRR0L = (uint8_t)SIOI_UBRR;
    UCSR0A = 0;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    stdout = &serial_output;
}

int SIOI_ReadChar(void)
{
    uint8_t character;

    if (rx_head == rx_tail) {
        return -1;
    }

    character = rx_buffer[rx_tail];
    rx_tail = (uint8_t)((rx_tail + 1) & SIOI_RX_BUFFER_MASK);
    return character;
}

uint32_t SIOI_Seconds(void)
{
    uint32_t seconds;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        seconds = uptime_seconds;
    }

    return seconds;
}

void SIOI_TickFromISR(void)
{
    ticks_in_second++;
    if (ticks_in_second >= SIOI_TICKS_PER_SECOND) {
        ticks_in_second = 0;
        uptime_seconds++;
    }
}

ISR(USART_RX_vect)
{
    uint8_t character = UDR0;
    uint8_t next_head = (uint8_t)((rx_head + 1) & SIOI_RX_BUFFER_MASK);

    if (next_head != rx_tail) {
        rx_buffer[rx_head] = character;
        rx_head = next_head;
    }
}
