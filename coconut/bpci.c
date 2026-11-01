/*******************************************************************************
1995 - 2027


                                 𝘊 o 𝘊 o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#define   MOD_BPCI

#include "types.h"
#include "cnf.h"

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

/******************************************************************
 * Initialisation of the basic process control component
 ******************************************************************/
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
