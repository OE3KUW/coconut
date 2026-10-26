/************************************************************************************
                                  <o<onut
************************************************************************************/
#ifndef  MOD_H_IF_TIMI
#define  MOD_H_IF_TIMI

#include "bpci_if.h"

/*******************************************************************************
* Includes
*******************************************************************************/

#ifdef   MOD_TIMI
#define  EXT
#define  CONST
#else
#define  EXT     extern
#define  CONST   const
#endif

/*******************************************************************************
* Defines
*******************************************************************************/

/*********************************************************** @adt   @Hadt */
#define NULL_TMR    ((TIMER)ZERO)
/* @END.Hadt
 *  INFO    symbolic name for timer of type NIL; used as return value if
 *          creation of a timer has failed
 *  ACCESS
 @END**********************************************************************/


/*******************************************************************************
* Typedefs
*******************************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR    TIMER;
/* @END.Hadt
 *  INFO    data type for timers
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UINT     TIMERDELAY;
/* @END.Hadt
 *  INFO    data type for timer delay
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR    TIMERMODE;
/* @END.Hadt
 *  INFO    data type for timer mode; example: TIMI_PERIODIC, TIMI_ONESHOT
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
#define TIMI_PERIODIC   ((TIMERMODE)0)
/* @END.Hadt
 *  INFO    set mode of timer to be of type periodic; used as parameter for
 *          TIMI_SetMode() and TIMI_Create()
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
#define TIMI_ONESHOT    ((TIMERMODE)1)
/* @END.Hadt
 *  INFO    set mode of timer to be of type oneshot; used as parameter for
 *          TIMI_SetMode() and TIMI_Create()
 *  ACCESS
 @END**********************************************************************/

 /*******************************************************************************
* Variables EXT
*******************************************************************************/

/********************************************************** @pubv  @Hpubv */
EXT    UCHAR  __tiny  elapsed_ticks; /* incemented by interrupt handler */
/* @END.Hpubv
 *  INFO    counts the elapsed timer ticks
 *  ACCESS  W from timer interrupt; R/W from this module (in an critical section)
 @END**********************************************************************/


/*******************************************************************************
* Macros
*******************************************************************************/

#ifdef CNF_ASSERTING
/********************************************************* @trans @Htrans */
#define TIMI_CreateWakeUpTimer(s,p,m,d) mTIMI_CreateWakeUpTimer(s,m,d)
/* @END.Htrans
 *  IN      STRING s, PROCESS p, TIMERMODE m, TIMERDELAY d
 *  PRE
 *  POST
 *  RETURN  ((TIMER)(Value)) <> NULL_TMR if creation was a success
 *  INFO
 @END**********************************************************************/

#else
#define TIMI_CreateWakeUpTimer(s,p,m,d) mTIMI_CreateWakeUpTimer(m,d)
#endif

/*******************************************************************************
* Functionprototypes EXT
*******************************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Init(VOID);
/* @END.Htrans
 *  IN
 *  PRE     has not been called before
 *  POST    all internal data structures are initialised; all timers are free
 *  RETURN
 *  INFO    should be called once only during startup
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Activate(VOID);
/* @END.Htrans
 *  IN
 *  PRE     has not been called before && TIMI_Init() has been called
 *  POST    timer services are active
 *  RETURN
 *  INFO    has to be called once after TIMI_Init()
 @END**********************************************************************/


#ifdef CNF_ASSERTING
    EXT TIMER mTIMI_CreateWakeUpTimer(STRING s,TIMERMODE m,TIMERDELAY d);
#else
    EXT TIMER mTIMI_CreateWakeUpTimer(TIMERMODE m,TIMERDELAY d);
#endif

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Connect(TIMER t, TASK u);
/* @END.Htrans
 *  IN
 *  PRE     TRUE
 *  POST    NORMAL: timer (t) is connected to task (u)
 *          FAIL:   a system reset is forced if timer (t) or task (u)
 *                  are not valid or timer (t) is already connected to a task
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Disconnect(TIMER t, TASK u);
/* @END.Htrans
 *  IN
 *  PRE     TRUE
 *  POST    if timer (t) is connected to task (u) then
 *          timer (t) is disconnected from task (u)
 *  RETURN
 *  INFO    NOT USED
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Start(TIMER t);
/* @END.Htrans
 *  IN
 *  PRE     TRUE
 *  POST    timer (t) is started or restarted if it was already started
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_Stop(TIMER t);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST    timer (t) is stopped; the event flag of timer (t) is cleared
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_SetMode(TIMER t, TIMERMODE m);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST    mode of timer (t) set according to timermode (m)
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID  TIMI_SetDelay(TIMER t, TIMERDELAY d);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST    timer delay of timer (t) set accourding to timer delay (d)
 *  RETURN
 *  INFO
 @END**********************************************************************/

/*********************************************************** @obs   @Hobs */
EXT BOOL  TIMI_IsTimer(TIMER t);
/* @END.Hobs
 *  IN
 *  PRE
 *  POST
 *  RETURN  TRUE if timer (t) is running or has elapsed
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT BOOL  TIMI_Event(TIMER t);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST    the elapsed information of timer (t) is cleared
 *  RETURN  TRUE if timer (t) has elapsed
 *  INFO
 @END**********************************************************************/


/********************************************************* @taskf @Htaskf */
EXT VOID  TIMI_TaskFunction(VOID); /* direct called from scheduler */
/* @END.Htaskf
 *  IN
 *  PRE
 *  POST    expired one_shot timers:    are removed from the timer queue
 *          expired periodic timers:    are requeued
 *          FOR ALL expired timers:     their state is set to elapsed and
 *                                      their associated tasks are signalled
 *  RETURN
 *  INFO
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
#define DEF_TIME    ((TIMERDELAY)(1))
/* @END.Hadt
 *  INFO
 *  ACCESS
 @END**********************************************************************/

#undef   EXT
#undef   CONST
#endif
/* be sure to have the last preprocessor command on a seperate line */
