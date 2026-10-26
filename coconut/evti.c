/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Event management
*
*  DESCRIPTION : EVTI.C sourcefile
*
*      $Archive: /AIO/Project/INFRA/SRC/EVTI.C $
*     $Revision: 2 $
*  Last $Author: Hoffmann $
*         $Date: 2.04.97 14:50 $
*
*  First Author: Paul Schenk
* Creation Date: 20.11.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: EVTI.C $
 *
 * *****************  Version 2  *****************
 * User: Hoffmann     Date: 2.04.97    Time: 14:50
 * Updated in $/AIO/Project/INFRA/SRC
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/


#define   MOD_EVTI

#include "coco_types.h"
#include "cnf.h"
#include "legacy_macros.h"
#include "erri_if.h"

#include "evti_if.h"
#include "evti.h"

/* Initialisation */

VOID EVTI_Init(VOID)
{
    ASSERT(!BPCI_IsStarted());

    nr_events = 0;
}

/*
 * Creation
 */

#ifdef CNF_ASSERTING
EVENT mEVTI_Create(STRING s)
#else
EVENT mEVTI_Create(VOID)
#endif
{
    ASSERT(!BPCI_IsStarted());
    ASSERT(nr_events < CNF_MAX_NR_EVENTS );

#ifndef PCV
    if(nr_events >= CNF_MAX_NR_EVENTS)
		ERRI_vErrorHandler(ERR_NO_MORE_EVENTS, EVTI_LOCATION_4, NULL);
#endif

    ++nr_events;

#ifdef CNF_ASSERTING
    EVT_NAME( nr_events )    = s;
#endif
    EVT_PENDING( nr_events ) = (FLAG)FALSE;
    EVT_TASK( nr_events )    = ZERO;

    return nr_events;
}

/*
 * Connection
 */

VOID EVTI_Connect(EVENT e, TASK t)
{
  #ifdef PCV
    ASSERT(!(t > nr_tasks || t == ZERO || e > nr_events || e == ZERO));
  #else
    if(t > nr_tasks || t == ZERO || e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_CONNECT, EVTI_LOCATION_1, NULL);
  #endif

    ASSERT(EVT_TASK(e) == ZERO);

    EVT_TASK(e) = t;
}

#if (0) // uncalled
VOID EVTI_Disconnect(EVENT e, TASK t)
{
    if (EVT_TASK(e) == t)
        EVT_TASK(e) = ZERO;
}
#endif

/*
 * Sending
 */

VOID EVTI_Send(EVENT e)
{
  #ifdef PCV
    ASSERT(!(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO));
  #else
    if(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_SEND, EVTI_LOCATION_2, NULL);
  #endif

    if (EVT_TASK(e))
    {
        EVT_PENDING(e) = (FLAG)TRUE;
        BPCI_WakeUpTask(EVT_TASK(e));
    }
}

/*
 * Sending out of interrupts
 */
VOID __adecl EVTI_IsrSend(EVENT e)
{
  #ifdef PCV
    ASSERT(!(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO));
  #else
    if(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO)
        ERRI_SoftwareReset();   // i can not use the error handler out of an interrupt
  #endif

    if (EVT_TASK(e))
    {
        EVT_PENDING(e) = (FLAG)TRUE;
        BPCI_IsrWakeUpTask(EVT_TASK(e));    // isr wakeup
    }
}

/*
 * Receiving
 */

BOOL EVTI_Receive(EVENT e)
{
  #ifdef PCV
    ASSERT(!(e > nr_events || e == ZERO));
  #else
    if(e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_RECEIVE, EVTI_LOCATION_3, NULL);
  #endif

    if (EVT_PENDING(e) == (FLAG)TRUE)
    {
        EVT_PENDING(e) = (FLAG)FALSE;
        return (BOOL)TRUE;
    }

    return (BOOL)FALSE;
}
