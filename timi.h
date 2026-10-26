/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Timer management
*
*  DESCRIPTION : TIMI.H local headerfile
*
*      $Archive: /AIO/Project/INFRA/SRC/TIMI.H $
*     $Revision: 1 $
*  Last $Author: Hoffmann $
*         $Date: 7.03.97 8:18 $
*
*  First Author: Paul Schenk
* Creation Date: 22.11.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: TIMI.H $
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/

#ifdef  MOD_TIMI
#ifndef  MOD_H_TIMI
#define  MOD_H_TIMI
/*******************************************************************************
* Includefiles
*******************************************************************************/


/*******************************************************************************
* Defines
*******************************************************************************/
#define COCO_TIMER_IN_C         OFF       /* switch definition shifted from cdiv...*/

/*******************************************************************************
* Typedefs
*******************************************************************************/


struct TimerControlBlock   /* timer control blocks */
{
#ifdef CNF_ASSERTING
    STRING      name;      /* name of the timer                  */
#endif
    UCHAR       flags;     /* type, mode, status, event,...      */

/* usage of flags
ÚÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ BIT ³ MEANING          ³ ON          ³ OFF             ³
ÃÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
³  0  ³ TIMERMODE        ³ PERIODIC    ³ ONESHOT         ³
ÃÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
³  1  ³ TIMEREVENTFLAG   ³ ELAPSED     ³ NOT ELAPSED     ³
ÀÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
    TASK        task;     /* receiver task    */
    TIMERDELAY  delay;    /* timer delay value                            */
    TIMERDELAY  delta;    /* delta timer delay, used in timer delta list  */
    TIMER       next;     /* forward link used to chain the timer in the  */
                          /* delta list of active timers                  */
    TIMER       prev;     /* backward link used to chain the timer in the */
                          /* delta list of active timers                  */
};


/*******************************************************************************
* Variables             STATIC
*******************************************************************************/

static struct TimerControlBlock tim_base[CNF_MAX_NR_TIMERS];

static TIMER       nr_timers;     /* number of timers               */

static TIMER __tiny  head;   /* dummy timer used as head of delta list */
static TIMER __tiny  tail;   /* dummy timer used as tail of delta list */

/*******************************************************************************
* Macros
*******************************************************************************/

#define TIM_FIELD(x, f) (tim_base[(x)-1].f)

#ifdef CNF_ASSERTING
#define TIM_NAME( x )   TIM_FIELD( x, name )
#endif

#define TIM_FLAGS( x )  TIM_FIELD( x, flags )
#define TIM_TASK( x )   TIM_FIELD( x, task )
#define TIM_DELAY( x )  TIM_FIELD( x, delay )
#define TIM_DELTA( x )  TIM_FIELD( x, delta )
#define TIM_NEXT( x )   TIM_FIELD( x, next )
#define TIM_PREV( x )   TIM_FIELD( x, prev )

#define B_PERIODIC      DBIT(0)
#define B_ELAPSED       DBIT(1)

/*******************************************************************************
* Functionprototypes    STATIC
*******************************************************************************/

#if (COCO_TIMER_IN_C == ON) || defined(PCV)
    static VOID ProcessTimer(VOID);        /* called by Timer_handler()      */
#endif

static VOID __adecl remove_timer(TIMER t);
static VOID __adecl add_timer(TIMER t, TIMERDELAY offset);


#endif
#else
#error YOU ARE NOT OWNER OF THIS MODULE !!
#endif
