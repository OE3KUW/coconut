/******************************************************************
1995 - 2026


                          < o < o n u t


                                                       Paul Schenk
                                                    қuran Wolfgang
******************************************************************/
#define   MOD_BPCI

#include "coco_types.h"
#include "cnf.h"
#include "legacy_macros.h"

#include "quei_if.h"
#include "mbxi_if.h"
#include "evti_if.h"
#include "timi_if.h"

#include "bpci_if.h"
#include "bpci.h"
#include "erri_if.h"


struct TCBlock bpc_t_base[CNF_MAX_NR_TASKS];
TASK t_first = ZERO;
TASK t_last = ZERO;
TASK t_self = ZERO;
BIT system_is_running = FALSE;
TASK nr_tasks = 0;

VOID do_main(VOID) __attribute__((weak));
VOID do_start(VOID) __attribute__((weak));

VOID do_main(VOID)
{
    /* Default no-op implementation used when the application does not provide one. */
}

VOID do_start(VOID)
{
    /* Default no-op implementation used when the application does not provide one. */
}

/*
 * since the scheduler will not return, it's better to call it "main"
 * and execute all the startup code prior to enter the endless loop.
 * we would otherwise loose stack space for the call.
 *
 */

#ifndef CCN_NO_MAIN
int main(void)
{
    UINT StackPointer;      /* only the lower 16 bits of XSP are saved */

    USES_CRITICAL_SECTION;

    __asm("ei 6");                    /* disable interrupts level<6 */

    __asm("ldf 0");                   /* be sure to be on RB==0 */
    system_is_running = FALSE; /* indicate that system is not running */

    /* Startup of the operating system    */

    BPCI_Init();    /* process control    */
    MALI_Init();    /* memory management  */
    QUEI_Init();    /* queue management   */
    MBXI_Init();    /* mailbox management */
    EVTI_Init();    /* event management   */
    TIMI_Init();    /* timer management   */

    do_main();      /* execute external main routine (create proc, task,..) */

    system_is_running = TRUE;  /* system is running      */
    __asm("ei 0");             /* enable all interrupts      */
    TIMI_Activate();                /* start timer management */

    do_start();     /* execute external start routine */

    StackPointer = __SP;

    for ( ; ; )
    {
        __asm("ldf 0");         /* be sure to be on RB==0 */

        if (StackPointer != __SP)
            ERRI_vErrorHandler(ERR_BAD_STACK, BPCI_LOCATION_0, NULL);

        if (elapsed_ticks)
            TIMI_TaskFunction();

        if (t_first)
        {
            if (t_first > nr_tasks)
                ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_1, NULL);

            ENTER_CRITICAL_SECTION;

            t_self = t_first;
            t_first = BPC_T_NEXT(t_self);

            LEAVE_CRITICAL_SECTION;

            if (t_first > nr_tasks || t_self > nr_tasks || t_self == ZERO)
                ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_2, NULL);

            BPC_T_FLAG(t_self) = (FLAG)FALSE;
            (* BPC_T_ACTION(t_self))();
            t_self = ZERO;
        }
    }

    return 0;
}
#endif

/*
 * Initialisation of the basic process control component
 */
VOID BPCI_Init(VOID)
{
    nr_tasks     = 0;
    t_self       = ZERO;
    t_first      = ZERO;
    t_last       = ZERO;
}


TASK mBPCI_CreateTask(PPV c, TASK_PRIORITY n)
{
    if(nr_tasks >= CNF_MAX_NR_TASKS)
        ERRI_vErrorHandler(ERR_NO_MORE_TASKS, BPCI_LOCATION_4, NULL);

    ++nr_tasks;

    BPC_T_ACTION( nr_tasks )     = c;
    BPC_T_PRIORITY( nr_tasks )   = n;
    BPC_T_FLAG( nr_tasks )       = (FLAG)FALSE;
    BPC_T_NEXT( nr_tasks )       = ZERO;
    BPC_T_PREV( nr_tasks )       = ZERO;

    return nr_tasks;
}

VOID BPCI_WakeUpTask(TASK t)
{
    register TASK u;
    USES_CRITICAL_SECTION;

    if(t > nr_tasks || t == 0)
        ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_3, NULL);

    ENTER_CRITICAL_SECTION;

    if(BPC_T_FLAG(t) == (FLAG)FALSE)   /* if t is not already awake */
    {
    /* set wake-up flag of t and put t in the task priority queue */
    /* of process p                                               */

        BPC_T_FLAG(t) = (FLAG)TRUE;

        if (!t_first)
        {
        /* the queue is empty, so we have to set up the priority queue */
        /* as a single-element queue                                   */

            BPC_T_NEXT(t)               = ZERO;
            BPC_T_PREV(t)               = ZERO;
            t_first                     = t;
            t_last                      = t;
        }
        else if (BPC_T_PRIORITY(t) <= BPC_T_PRIORITY(t_last))
        {
        /* all tasks in the queue have a priority at least as great as */
        /* that of t, so we have to append t at the tail of the queue  */

            BPC_T_NEXT(t)               = ZERO;
            BPC_T_PREV(t)               = t_last;
            BPC_T_NEXT(t_last)          = t;
            t_last                      = t;
        }
        else if (BPC_T_PRIORITY(t) > BPC_T_PRIORITY(t_first))
        {
        /* t has a priority greater than any of the tasks in the queue, */
        /* so we have to append t at the head of the queue              */

            BPC_T_NEXT(t)               = t_first;
            BPC_T_PREV(t)               = ZERO;
            BPC_T_PREV(t_first)         = t;
            t_first                     = t;
        }
        else
        {
        /* we have to insert t somewhere inside the queue */

        /* start searching from the tail of the queue for the first     */
        /* element u with a priority greater than or equal to that of t */

            u = t_last;

            do
                u = BPC_T_PREV(u);
            while (BPC_T_PRIORITY(u) < BPC_T_PRIORITY(t));

        /* insert t immediately behind u in the priority queue */

            BPC_T_NEXT(t)               = BPC_T_NEXT(u);
            BPC_T_PREV(t)               = u;
            BPC_T_PREV(BPC_T_NEXT(u))   = t;
            BPC_T_NEXT(u)               = t;
        }
    }
    LEAVE_CRITICAL_SECTION;
}

VOID BPCI_IsrWakeUpTask(TASK t)
{
    register TASK u;
    USES_CRITICAL_SECTION;

    ENTER_CRITICAL_SECTION;

    if(t > nr_tasks || t == ZERO)  // i can not use the error handler out
        ERRI_SoftwareReset();

    if(BPC_T_FLAG(t) == (FLAG)FALSE)   /* if t is not already awake */
    {
    /* set wake-up flag of t and put t in the task priority queue */
    /* of process p                                               */

        BPC_T_FLAG(t) = (FLAG)TRUE;

        if (!t_first)
        {
        /* the queue is empty, so we have to set up the priority queue */
        /* as a single-element queue                                   */

            BPC_T_NEXT(t)               = ZERO;
            BPC_T_PREV(t)               = ZERO;
            t_first                     = t;
            t_last                      = t;
        }
        else if (BPC_T_PRIORITY(t) <= BPC_T_PRIORITY(t_last))
        {
        /* all tasks in the queue have a priority at least as great as */
        /* that of t, so we have to append t at the tail of the queue  */

            BPC_T_NEXT(t)               = ZERO;
            BPC_T_PREV(t)               = t_last;
            BPC_T_NEXT(t_last)          = t;
            t_last                      = t;
        }
        else if (BPC_T_PRIORITY(t) > BPC_T_PRIORITY(t_first))
        {
        /* t has a priority greater than any of the tasks in the queue, */
        /* so we have to append t at the head of the queue              */

            BPC_T_NEXT(t)               = t_first;
            BPC_T_PREV(t)               = ZERO;
            BPC_T_PREV(t_first)         = t;
            t_first                     = t;
        }
        else
        {
        /* we have to insert t somewhere inside the queue */

        /* start searching from the tail of the queue for the first     */
        /* element u with a priority greater than or equal to that of t */

            u = t_last;

            do
                u = BPC_T_PREV(u);
            while (BPC_T_PRIORITY(u) < BPC_T_PRIORITY(t));

        /* insert t immediately behind u in the priority queue */

            BPC_T_NEXT(t)               = BPC_T_NEXT(u);
            BPC_T_PREV(t)               = u;
            BPC_T_PREV(BPC_T_NEXT(u))   = t;
            BPC_T_NEXT(u)               = t;
        }
    }
    LEAVE_CRITICAL_SECTION;
}
