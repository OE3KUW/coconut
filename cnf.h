/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : Central controller
*
*       MODULE : CNF.H
*
*  DESCRIPTION : CoCoNut configuration headerfile
*
*      $Archive: /AIO/Project/INC/CNF.H $
*     $Revision: 1 $
*  Last $Author: Hoffmann $
*         $Date: 7.03.97 8:17 $
*
*  First Author: PSC
* Creation Date: 07.03.1996 15:18
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: CNF.H $
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:17
 * Created in $/AIO/Project/INC
 ****************************************************************************/
 #ifndef MOD_CNF
 #define MOD_CNF

 #include <stdint.h>

 #define CNF_MAX_NR_TASKS        16u
 #define CNF_MAX_NR_EVENTS       8u
 #define CNF_MAX_NR_MAILBOXES    8u
 #define CNF_MAX_NR_TIMERS       8u
 #define CNF_MAX_NR_QUEUE_CELLS  32u
 #define CNF_MAX_TASK_PRIORITY   255u
 #define CNF_MAX_TIMERDELAY      65535u

 #define OFF 0u
 #define ON  1u

 #define F_CPU 16000000UL
 #define TIMI_TIMER_TIME ((uint16_t)(-5333))
 #define MSEC / 8

 #define CNF_ASSERTING

 #ifdef CNF_ASSERTING
 #define ASSERT(cond) do { if (!(cond)) __builtin_trap(); } while (0)
 #else
 #define ASSERT(cond) do { (void)sizeof(cond); } while (0)
 #endif

 #ifndef PCV
 #define TIMI_INTCALL() DCKB_DeckAction()
 #endif

 #endif

