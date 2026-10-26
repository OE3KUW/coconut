#include "ccn.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

typedef struct {
    ccn_task_fn_t function;
    uint8_t priority;
    bool ready;
} task_control_t;

typedef struct {
    ccn_task_t receiver;
    bool pending;
} event_control_t;

typedef struct {
    ccn_task_t receiver;
    ccn_message_t messages[CCN_MAILBOX_CAPACITY];
    uint8_t head;
    uint8_t count;
} mailbox_control_t;

typedef struct {
    ccn_task_t receiver;
    uint16_t delay;
    uint16_t remaining;
    bool periodic;
    bool running;
    bool expired;
} timer_control_t;

static task_control_t tasks[CCN_MAX_TASKS];
static event_control_t events[CCN_MAX_EVENTS];
static mailbox_control_t mailboxes[CCN_MAX_MAILBOXES];
static timer_control_t timers[CCN_MAX_TIMERS];
static uint8_t task_count;
static uint8_t event_count;
static uint8_t mailbox_count;
static uint8_t timer_count;
static uint8_t next_task_index;
volatile uint32_t pending_ticks;
volatile bool tick_overflow;

void ccn_timer_hardware_init(void);

static bool task_id_valid(ccn_task_t task)
{
    return task != CCN_INVALID_ID && task <= task_count;
}

static bool wake_task(ccn_task_t task)
{
    bool valid = false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (task_id_valid(task)) {
            tasks[task - 1u].ready = true;
            valid = true;
        }
    }

    return valid;
}

bool ccn_task_wake(ccn_task_t task)
{
    return wake_task(task);
}

bool ccn_task_wake_from_isr(ccn_task_t task)
{
    return wake_task(task);
}

void ccn_init(void)
{
    task_count = 0u;
    event_count = 0u;
    mailbox_count = 0u;
    timer_count = 0u;
    next_task_index = 0u;
    pending_ticks = 0u;
    tick_overflow = false;

    ccn_timer_hardware_init();
}

ccn_task_t ccn_task_create(ccn_task_fn_t function, uint8_t priority)
{
    uint8_t index;

    if (function == 0 || task_count >= CCN_MAX_TASKS) {
        return CCN_INVALID_ID;
    }

    index = task_count++;
    tasks[index].function = function;
    tasks[index].priority = priority;
    tasks[index].ready = false;
    return (ccn_task_t)(index + 1u);
}

ccn_event_t ccn_event_create(ccn_task_t receiver)
{
    uint8_t index;

    if (!task_id_valid(receiver) || event_count >= CCN_MAX_EVENTS) {
        return CCN_INVALID_ID;
    }

    index = event_count++;
    events[index].receiver = receiver;
    events[index].pending = false;
    return (ccn_event_t)(index + 1u);
}

static bool send_event(ccn_event_t event)
{
    bool valid = false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (event != CCN_INVALID_ID && event <= event_count) {
            event_control_t *control = &events[event - 1u];
            control->pending = true;
            tasks[control->receiver - 1u].ready = true;
            valid = true;
        }
    }

    return valid;
}

bool ccn_event_send(ccn_event_t event)
{
    return send_event(event);
}

bool ccn_event_send_from_isr(ccn_event_t event)
{
    return send_event(event);
}

bool ccn_event_receive(ccn_event_t event)
{
    bool was_pending = false;

    if (event == CCN_INVALID_ID || event > event_count) {
        return false;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        was_pending = events[event - 1u].pending;
        events[event - 1u].pending = false;
    }

    return was_pending;
}

ccn_mailbox_t ccn_mailbox_create(ccn_task_t receiver)
{
    uint8_t index;

    if (!task_id_valid(receiver) || mailbox_count >= CCN_MAX_MAILBOXES) {
        return CCN_INVALID_ID;
    }

    index = mailbox_count++;
    mailboxes[index].receiver = receiver;
    mailboxes[index].head = 0u;
    mailboxes[index].count = 0u;
    return (ccn_mailbox_t)(index + 1u);
}

bool ccn_mailbox_send(ccn_mailbox_t mailbox, ccn_message_t message)
{
    mailbox_control_t *control;
    uint8_t tail;

    if (mailbox == CCN_INVALID_ID || mailbox > mailbox_count) {
        return false;
    }

    control = &mailboxes[mailbox - 1u];
    if (control->count >= CCN_MAILBOX_CAPACITY) {
        return false;
    }

    tail = (uint8_t)((control->head + control->count) % CCN_MAILBOX_CAPACITY);
    control->messages[tail] = message;
    control->count++;
    (void)ccn_task_wake(control->receiver);
    return true;
}

bool ccn_mailbox_receive(ccn_mailbox_t mailbox, ccn_message_t *message)
{
    mailbox_control_t *control;

    if (mailbox == CCN_INVALID_ID || mailbox > mailbox_count || message == 0) {
        return false;
    }

    control = &mailboxes[mailbox - 1u];
    if (control->count == 0u) {
        return false;
    }

    *message = control->messages[control->head];
    control->head = (uint8_t)((control->head + 1u) % CCN_MAILBOX_CAPACITY);
    control->count--;
    return true;
}

uint8_t ccn_mailbox_count(ccn_mailbox_t mailbox)
{
    if (mailbox == CCN_INVALID_ID || mailbox > mailbox_count) {
        return 0u;
    }
    return mailboxes[mailbox - 1u].count;
}

ccn_timer_t ccn_timer_create(uint16_t delay_ticks, bool periodic)
{
    uint8_t index;

    if (delay_ticks == 0u || timer_count >= CCN_MAX_TIMERS) {
        return CCN_INVALID_ID;
    }

    index = timer_count++;
    timers[index].receiver = CCN_INVALID_ID;
    timers[index].delay = delay_ticks;
    timers[index].remaining = delay_ticks;
    timers[index].periodic = periodic;
    timers[index].running = false;
    timers[index].expired = false;
    return (ccn_timer_t)(index + 1u);
}

bool ccn_timer_connect(ccn_timer_t timer, ccn_task_t receiver)
{
    if (timer == CCN_INVALID_ID || timer > timer_count ||
        !task_id_valid(receiver)) {
        return false;
    }
    timers[timer - 1u].receiver = receiver;
    return true;
}

bool ccn_timer_start(ccn_timer_t timer)
{
    timer_control_t *control;

    if (timer == CCN_INVALID_ID || timer > timer_count) {
        return false;
    }

    control = &timers[timer - 1u];
    control->remaining = control->delay;
    control->expired = false;
    control->running = true;
    return true;
}

bool ccn_timer_stop(ccn_timer_t timer)
{
    if (timer == CCN_INVALID_ID || timer > timer_count) {
        return false;
    }

    timers[timer - 1u].running = false;
    timers[timer - 1u].expired = false;
    return true;
}

bool ccn_timer_expired(ccn_timer_t timer)
{
    bool expired;

    if (timer == CCN_INVALID_ID || timer > timer_count) {
        return false;
    }

    expired = timers[timer - 1u].expired;
    timers[timer - 1u].expired = false;
    return expired;
}

static void process_timers(uint32_t elapsed)
{
    uint8_t index;

    for (index = 0u; index < timer_count; index++) {
        timer_control_t *timer = &timers[index];
        uint32_t timer_elapsed = elapsed;
        uint32_t remaining;

        if (!timer->running) {
            continue;
        }

        remaining = timer->remaining;
        if (timer_elapsed < remaining) {
            timer->remaining = (uint16_t)(remaining - timer_elapsed);
            continue;
        }

        timer->expired = true;
        if (timer->receiver != CCN_INVALID_ID) {
            (void)ccn_task_wake(timer->receiver);
        }

        if (!timer->periodic) {
            timer->running = false;
            continue;
        }

        timer_elapsed -= remaining;
        timer->remaining = timer->delay;
        if (timer_elapsed >= timer->delay) {
            timer_elapsed %= timer->delay;
        }
        timer->remaining = (uint16_t)(timer->delay - timer_elapsed);
    }
}

static void process_pending_ticks(void)
{
    uint32_t elapsed;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        elapsed = pending_ticks;
        pending_ticks = 0u;
    }

    if (elapsed != 0u) {
        process_timers(elapsed);
    }
}

bool ccn_tick_overflowed(void)
{
    bool overflowed;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        overflowed = tick_overflow;
    }
    return overflowed;
}

static int8_t take_ready_task(void)
{
    int8_t selected = -1;
    uint8_t offset;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        for (offset = 0u; offset < task_count; offset++) {
            uint8_t index = (uint8_t)((next_task_index + offset) % task_count);
            if (!tasks[index].ready) {
                continue;
            }
            if (selected < 0 ||
                tasks[index].priority > tasks[(uint8_t)selected].priority) {
                selected = (int8_t)index;
            }
        }

        if (selected >= 0) {
            tasks[(uint8_t)selected].ready = false;
            next_task_index = (uint8_t)(((uint8_t)selected + 1u) % task_count);
        }
    }

    return selected;
}

void ccn_start(void)
{
    sei();

    for (;;) {
        int8_t selected;

        process_pending_ticks();
        selected = take_ready_task();
        if (selected >= 0) {
            tasks[(uint8_t)selected].function();
        }
    }
}
