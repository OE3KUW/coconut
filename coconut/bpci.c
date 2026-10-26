/***************************************************************************
*   PROJECT    : PAOLINA
****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Basic process and task management
*
*  DESCRIPTION : BPCI.C sourcefile
*
*      $Archive: /AIO/Project/INFRA/SRC/BPCI.C $
*     $Revision: 5 $
*  Last $Author: Burgstaller $
*         $Date: 11.04.97 10:25 $
*
*  First Author: Paul Schenk
* Creation Date: 01.12.95
*
****************************************************************************
* MODIFICATIONS
****************************************************************************
*
*      $History: BPCI.C $
 *
 * *****************  Version 5  *****************
 * User: Burgstaller  Date: 11.04.97   Time: 10:25
 * Updated in $/AIO/Project/INFRA/SRC
 *
 * *****************  Version 4  *****************
 * User: Reichl       Date: 8.04.97    Time: 8:32
 * Updated in $/AIO/Project/INFRA/SRC
 * changes with flag HRE commented
 *
 * *****************  Version 3  *****************
 * User: Reichl       Date: 4.04.97    Time: 14:10
 * Updated in $/AIO/Project/INFRA/SRC
 *
 * *****************  Version 2  *****************
 * User: Reichl       Date: 19.03.97   Time: 10:29
 * Updated in $/AIO/Project/INFRA/SRC
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
\***************************************************************************/


#define   MOD_BPCI

#include "CDIV.H"
//#include "COMPILER.H"
#include "TYPE.H"
#include "CNF.H"
#include "MACRO.H"
#include "ERRORS.H"

#include "QUEI_IF.H"
#include "MBXI_IF.H"
#include "EVTI_IF.H"
#include "TIMI_IF.H"
#include "MALI_IF.H"

#include "BPCI_IF.H"
#include "BPCI.H"
#include "ERRI_IF.H"


#ifdef PCV
#include "ctype.h"  // toupper
#endif

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

#ifdef CNF_ASSERTING
#ifndef PCV
    /* ASSERT TRAP */
    VOID ____f(VOID)
    {
        __asm("ei 6");      /* disable all interrupts level<6 */
        while(TRUE);
    } // only a trap, because our VCR has no console
#else
    VOID ____f(char *ass, char *fn, int ln)
    {
        clrscr();
        printf("\n\n*******************************************\n");
        printf("****** A S S E R T I O N  F A I L E D *****\n");
        printf("*******************************************\n");

        printf("Assert     = %s\n", ass);
        printf("File       = %s\n", fn);
        printf("Line       = %d\n", ln);

        if (BPCI_IsStarted())
        {
            if (t_self)
                printf("ActiveTask = %s\n", BPC_T_NAME(BPCI_GetCurrentTask()) );
            else
                printf("No Task active\n");
        }
        else
            printf("System was not running\n");

        printf("*******************************************\n");
        while(kbhit()) getch();
        printf("Press any key....");
        getch();

        exit(99);
    }
#endif
#endif

#ifdef PCV
    #define CONTINUE 1
    int CBreakHandler(void)
    {
        while(kbhit()) getch(); // eat all key's
        return CONTINUE;
    }

    struct text_info BPCI_ti;
#endif

/*
 * since the scheduler will not return, it's better to call it "main"
 * and execute all the startup code prior to enter the endless loop.
 * we would otherwise loose stack space for the call.
 *
 */

#ifndef CCN_NO_MAIN
int main(void)
{
#ifndef PCV
    UINT StackPointer;      /* only the lower 16 bits of XSP are saved */
#endif

#ifdef PCV
    int __key;
    #define ESC 27
#endif

    USES_CRITICAL_SECTION;

#ifdef PCV
    //-----------------------------------------------------
    // define swapping area for overlay
    //-----------------------------------------------------
    printf("\nSoll extended memory f�r Overlay verwendet werden ? J/N");
    if (toupper(getch()) == 'J')
        _OvrInitExt(0, 0);
    //-----------------------------------------------------
    ctrlbrk(CBreakHandler);
    gettextinfo(&BPCI_ti);
#endif

#ifndef PCV
    __asm("ei 6");                    /* disable interrupts level<6 */

    __asm("ldf 0");                   /* be sure to be on RB==0 */
#endif
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
#ifndef PCV
    __asm("ei 0");             /* enable all interrupts      */
#endif
    TIMI_Activate();                /* start timer management */

    do_start();     /* execute external start routine */

#ifndef PCV
    StackPointer = __SP;
#endif

    for ( ; ; )
    {
#ifndef PCV
        __asm("ldf 0");         /* be sure to be on RB==0 */

        if (StackPointer != __SP)
            ERRI_vErrorHandler(ERR_BAD_STACK, BPCI_LOCATION_0, NULL);
#endif

#ifdef PCV
        if (kbhit())
        {
            if ((__key = getch()) == ESC)
            {
                printf("\n\nSystem stopped due to ESC\n");
                exit(0);
            }
            else
                ungetch(__key);
        }
#endif

        if (elapsed_ticks)
            TIMI_TaskFunction();

        if (t_first)
        {
  #ifdef PCV
    ASSERT(!(t_first > nr_tasks));
  #else
            if (t_first > nr_tasks)
                ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_1, NULL);
  #endif

            ENTER_CRITICAL_SECTION;

            t_self = t_first;
            t_first = BPC_T_NEXT(t_self);

            LEAVE_CRITICAL_SECTION;

  #ifdef PCV
    ASSERT(!(t_first > nr_tasks || t_self > nr_tasks || t_self == ZERO));
  #else
            if (t_first > nr_tasks || t_self > nr_tasks || t_self == ZERO)
                ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_2, NULL);
  #endif

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
    ASSERT(!BPCI_IsStarted());

    nr_tasks     = 0;
    t_self       = ZERO;
    t_first      = ZERO;
    t_last       = ZERO;
}


#ifdef CNF_ASSERTING
TASK mBPCI_CreateTask(STRING s, PPV c, TASK_PRIORITY n)
#else
TASK mBPCI_CreateTask(PPV c, TASK_PRIORITY n)
#endif
{
    ASSERT(!BPCI_IsStarted());
    ASSERT(nr_tasks < CNF_MAX_NR_TASKS);

#ifndef PCV
    if(nr_tasks >= CNF_MAX_NR_TASKS)
        ERRI_vErrorHandler(ERR_NO_MORE_TASKS, BPCI_LOCATION_4, NULL);
#endif

    ++nr_tasks;

#ifdef CNF_ASSERTING
    BPC_T_NAME( nr_tasks )       = s;
#endif

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

  #ifdef PCV
    ASSERT(!(t > nr_tasks || t == 0));
  #else
    if(t > nr_tasks || t == 0)
        ERRI_vErrorHandler(ERR_INVAL_TASK, BPCI_LOCATION_3, NULL);
  #endif

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

            ASSERT(t_first != t_last);
            ASSERT(BPC_T_PRIORITY(t_first) >= BPC_T_PRIORITY(t));
            ASSERT(BPC_T_PRIORITY(t) > BPC_T_PRIORITY(t_last));

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

  #ifdef PCV
    ASSERT(!(t > nr_tasks || t == ZERO));
  #else
    if(t > nr_tasks || t == ZERO)  // i can not use the error handler out
        ((void (*) ()) 0) ();      // of an interrupt handler
  #endif

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

            ASSERT(t_first != t_last);
            ASSERT(BPC_T_PRIORITY(t_first) >= BPC_T_PRIORITY(t));
            ASSERT(BPC_T_PRIORITY(t) > BPC_T_PRIORITY(t_last));

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

