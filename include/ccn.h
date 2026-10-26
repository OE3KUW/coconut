#ifndef CCN_H
#define CCN_H

#include <stdbool.h>
#include <stdint.h>

#define CCN_MAX_TASKS 8
#define CCN_MAX_EVENTS 8
#define CCN_MAX_MAILBOXES 4
#define CCN_MAILBOX_CAPACITY 4
#define CCN_MAX_TIMERS 8

#define CCN_TICK_MS 8

typedef uint8_t ccn_task_t;
typedef uint8_t ccn_event_t;
typedef uint8_t ccn_mailbox_t;
typedef uint8_t ccn_timer_t;
typedef uint32_t ccn_message_t;
typedef void (*ccn_task_fn_t)(void);

#define CCN_INVALID_ID 0

void ccn_init(void);
ccn_task_t ccn_task_create(ccn_task_fn_t function, uint8_t priority);
bool ccn_task_wake(ccn_task_t task);
bool ccn_task_wake_from_isr(ccn_task_t task);

ccn_event_t ccn_event_create(ccn_task_t receiver);
bool ccn_event_send(ccn_event_t event);
bool ccn_event_send_from_isr(ccn_event_t event);
bool ccn_event_receive(ccn_event_t event);

ccn_mailbox_t ccn_mailbox_create(ccn_task_t receiver);
bool ccn_mailbox_send(ccn_mailbox_t mailbox, ccn_message_t message);
bool ccn_mailbox_receive(ccn_mailbox_t mailbox, ccn_message_t *message);
uint8_t ccn_mailbox_count(ccn_mailbox_t mailbox);

ccn_timer_t ccn_timer_create(uint16_t delay_ticks, bool periodic);
bool ccn_timer_connect(ccn_timer_t timer, ccn_task_t receiver);
bool ccn_timer_start(ccn_timer_t timer);
bool ccn_timer_stop(ccn_timer_t timer);
bool ccn_timer_expired(ccn_timer_t timer);

bool ccn_tick_overflowed(void);
void ccn_start(void);

#endif
