/*******************************************************************************
1995 - 2027


                                 ᴄ o ᴄ o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#define   MOD_TIMI

#include "types.h"
#include "cnf.h"
#include "erri_if.h"

#include "bpci_if.h"

#include "timi_if.h"
#include "timi.h"

#if (COCO_TIMER_IN_C == ON)
#include "dckb_if.h"
#endif

/******************************************************************
 * Initialisation
 ******************************************************************/
VOID TIMI_Init(VOID)
{
/*    TIMI_HW_INTENABLE = TIMI_HW_RUN = 0;      cho*/

    elapsed_ticks   = 0;
    nr_timers       = 2;
    head            = (TIMER) 1;
    tail            = (TIMER) 2;
    TIM_DELTA(head) = 0;         /* other fields of "head" and "tail" than */
    TIM_NEXT(head)  = tail;      /* those initialised here are not used    */
    TIM_PREV(head)  = ZERO;
    TIM_DELTA(tail) = (TIMERDELAY)CNF_MAX_TIMERDELAY;
    TIM_NEXT(tail)  = ZERO;
    TIM_PREV(tail)  = head;
}

/******************************************************************
 * Activate the timer service
 ******************************************************************/
VOID TIMI_Activate(VOID)
{
/*    TIMI_HW_REG_HIGH = (TIMI_TIMER_TIME >> 8);  cho*/
/*    TIMI_HW_REG_LOW  = TIMI_TIMER_TIME & 255;   cho*/

/*    TIMI_SET_MODE();                            cho*/

/*    TIMI_HW_INTENABLE = TIMI_HW_RUN = 1;        cho*/
}

/******************************************************************
 * Creation
 ******************************************************************/
TIMER mTIMI_CreateWakeUpTimer(TIMERMODE m, TIMERDELAY d)
{
    if(nr_timers >= CNF_MAX_NR_TIMERS)
		ERRI_vErrorHandler(ERR_NO_MORE_TIMERS, TIMI_LOCATION_7, NULL);

    ++nr_timers;

    TIM_FLAGS(nr_timers)   = 0; /* default */

    if (m == TIMI_PERIODIC)
        SetBit(TIM_FLAGS(nr_timers), (UCHAR)B_PERIODIC);  /* if periodic */

    TIM_DELAY(nr_timers)   = d;

    TIM_TASK(nr_timers)    = ZERO;
    TIM_DELTA(nr_timers)   = 0;
    TIM_NEXT(nr_timers)    = ZERO;
    TIM_PREV(nr_timers)    = ZERO;

    return nr_timers;
}

/******************************************************************
 * Connection
 ******************************************************************/
VOID TIMI_Connect(TIMER t, TASK u)
{
    if(t > nr_timers || u > nr_tasks || t == ZERO || u == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_CONNECT, TIMI_LOCATION_1, NULL);

    ClearBitInByte(TIM_FLAGS(t), B_ELAPSED); /* default = not elapsed */
    TIM_TASK(t) = u;
}

#if (0)  // uncalled
VOID TIMI_Disconnect(TIMER t, TASK u)
{
    if (TIM_TASK(t) == u)
        TIM_TASK(t) = ZERO;
}
#endif

/******************************************************************
 * Activation
 ******************************************************************/
VOID TIMI_Start(TIMER t)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_START, TIMI_LOCATION_2, NULL);

    if (TIM_PREV( t ))        /* if timer is still in the delta lst */
        remove_timer( t );  /* remove it from the delta list      */

    ClearBitInByte(TIM_FLAGS(t), B_ELAPSED);

    if (TIM_DELAY( t ))
        add_timer( t, ZERO); /* add the timer to the delta list */
}

VOID TIMI_Stop(TIMER t)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_STOP, TIMI_LOCATION_3, NULL);

    remove_timer( t );            /* remove the timer from the delta list */

    ClearBitInByte(TIM_FLAGS( t ), B_ELAPSED);
}

/******************************************************************
 * IsTimer running or has it elapsed
 ******************************************************************/
BOOL TIMI_IsTimer(TIMER t)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_EVENT, TIMI_LOCATION_4, NULL);

    if (TIM_PREV(t) || GetBit(TIM_FLAGS(t), B_ELAPSED))
        return TRUE;

    return FALSE;
}

/******************************************************************
 * Timerevent
 ******************************************************************/
BOOL TIMI_Event(TIMER t)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_EVENT, TIMI_LOCATION_4, NULL);

    if ( GetBit(TIM_FLAGS(t), B_ELAPSED) )
    {
        ClearBitInByte(TIM_FLAGS(t), B_ELAPSED);
        return (BOOL) TRUE;
    }
    return (BOOL) FALSE;
}

#if (0) // uncalled
VOID TIMI_SetMode(TIMER t, TIMERMODE m)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_MODE, TIMI_LOCATION_5, NULL);

    if (m == TIMI_PERIODIC)
        SetBit(TIM_FLAGS(t), B_PERIODIC);  /* periodic */
    else
        ClrBit(TIM_FLAGS(t), B_PERIODIC);  /* oneshot */
}
#endif

VOID TIMI_SetDelay( TIMER t, TIMERDELAY d)
{
    if(t > nr_timers || t == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_TIMER_DELAY, TIMI_LOCATION_6, NULL);

    TIM_DELAY(t) = d;
    if (TIM_PREV(t))        /* if timer is in the delta list */
        TIMI_Start( t );    /* restart it                    */
}

/******************************************************************
 * Timertask (direct called from scheduler)
 ******************************************************************/
VOID TIMI_TaskFunction(VOID)
{
    register TIMER t;
    register TIMERDELAY real_time_ticks;
    register TIMERDELAY actual_delta;

    USES_CRITICAL_SECTION;

    // get the elapsed realtime ticks since the last execution
    // of this taskfunction. due to the cooperative behaviour
    // of ᴄoᴄonut's tasks it may happen that we've missed
    // some ticks, so let's run our timers faster in this case.

    ENTER_CRITICAL_SECTION;

        real_time_ticks = (TIMERDELAY)elapsed_ticks; /* is always > 0 */
        elapsed_ticks   = ZERO;

    LEAVE_CRITICAL_SECTION;

    t = TIM_NEXT(head);

    // do this while we are in the past and there is a timer

    while ((t != tail) && real_time_ticks)
    {
        actual_delta = TIM_DELTA(t);

        if (real_time_ticks >= actual_delta)
        {
            real_time_ticks -= actual_delta;
            TIM_DELTA(t)     = ZERO;

            do
            {
                TIM_PREV(t)             = ZERO;
                TIM_NEXT(head)          = TIM_NEXT(t);
                TIM_PREV(TIM_NEXT(t))   = head;

                if (GetBit(TIM_FLAGS(t), B_PERIODIC))
                    add_timer(t, real_time_ticks);

                if (TIM_TASK(t))
                {
                    SetBit(TIM_FLAGS(t), (UCHAR)B_ELAPSED);
                    BPCI_WakeUpTask(TIM_TASK(t));
                }
                t = TIM_NEXT(head);
            }
            while(!TIM_DELTA(t));
        }
        else
        {
            TIM_DELTA(t)    -= real_time_ticks;

            return; /* thats all */
        }
    }
}

/******************************************************************
 * Delta list operations
 ******************************************************************/
static VOID __adecl remove_timer(TIMER t)
{
    if (TIM_PREV(t))   /* if t is in the list */
    {
        if (TIM_NEXT(t) != tail)
            TIM_DELTA(TIM_NEXT(t)) += TIM_DELTA(t);

        TIM_PREV(TIM_NEXT(t)) = TIM_PREV(t);
        TIM_NEXT(TIM_PREV(t)) = TIM_NEXT(t);
        TIM_PREV(t)           = ZERO;
    }
}

static VOID __adecl add_timer(TIMER t, TIMERDELAY offset)
{
    register TIMER u;
    register TIMERDELAY d;
    USES_CRITICAL_SECTION;

    ENTER_CRITICAL_SECTION;

    offset += elapsed_ticks;

    LEAVE_CRITICAL_SECTION;

    d = TIM_DELAY(t);
    if ((CNF_MAX_TIMERDELAY - offset) > d)
        d += offset;
    else
        d = CNF_MAX_TIMERDELAY;

    for(u = TIM_NEXT(head); TIM_DELTA(u) < d; u = TIM_NEXT(u))
        d -= TIM_DELTA(u);

    TIM_NEXT(t)             = u;
    TIM_PREV(t)             = TIM_PREV(u);
    TIM_NEXT(TIM_PREV(u))   = t;
    TIM_PREV(u)             = t;
    TIM_DELTA(t)            = d;

    if (u != tail)
        TIM_DELTA(u) -= d;
}

#if (COCO_TIMER_IN_C == ON)
        /* TIMER 0 Interrupt Handler USES REGISTERBANK 1 */
        // located in the common code-area (keep it as small as possible)
        static VOID Timer_Handler(VOID) interrupt TIMI_INTERRUPT_NR using 1
        {
            ProcessTimer(); /* reduce commonarea  */
        }
#endif

#if (COCO_TIMER_IN_C == ON)
 // this function will be located in a separate bank of the code-memory
 static VOID ProcessTimer(VOID)
 {
    static BIT do_wake_up = ZERO;

    data union
    {
        UINT y;
        UCHAR y1[2];
    } tu;

    // correct the interrupt latency

    TIMI_HW_RUN      = 0;              // stop the hardwaretimer
    tu.y1[1]         = TIMI_HW_REG_LOW;
    tu.y1[0]         = TIMI_HW_REG_HIGH;
    tu.y            += TIMI_TIMER_TIME + 15; /* 15 cycles from TR=0 to TR=1 */
    TIMI_HW_REG_LOW  = tu.y1[1];
    TIMI_HW_REG_HIGH = tu.y1[0];
    TIMI_HW_RUN      = 1;              // start the timer again

    if (do_wake_up = !do_wake_up)
        elapsed_ticks++;

    DCKB_DeckAction(); // Tapedeck Communication Interrupt Call (Send+Receive)
 }
#endif
