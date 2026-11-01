/*******************************************************************************
1995 - 2027


                                 𝘊 o 𝘊 o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#define   MOD_EVTI

#include "types.h"
#include "cnf.h"
#include "erri_if.h"

#include "evti_if.h"
#include "evti.h"

/******************************************************************
 * Initialisation
 ******************************************************************/

VOID EVTI_Init(VOID)
{
    nr_events = 0;
}

/******************************************************************
 * Creation
 ******************************************************************/

EVENT mEVTI_Create(VOID)
{
    if(nr_events >= CNF_MAX_NR_EVENTS)
		ERRI_vErrorHandler(ERR_NO_MORE_EVENTS, EVTI_LOCATION_4, NULL);

    ++nr_events;

    EVT_PENDING( nr_events ) = (FLAG)FALSE;
    EVT_TASK( nr_events )    = ZERO;

    return nr_events;
}

/******************************************************************
 * Connection
 ******************************************************************/

VOID EVTI_Connect(EVENT e, TASK t)
{
    if(t > nr_tasks || t == ZERO || e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_CONNECT, EVTI_LOCATION_1, NULL);

    EVT_TASK(e) = t;
}

#if (0) // uncalled
VOID EVTI_Disconnect(EVENT e, TASK t)
{
    if (EVT_TASK(e) == t)
        EVT_TASK(e) = ZERO;
}
#endif

/******************************************************************
 * Sending
 ******************************************************************/

VOID EVTI_Send(EVENT e)
{
    if(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_SEND, EVTI_LOCATION_2, NULL);

    if (EVT_TASK(e))
    {
        EVT_PENDING(e) = (FLAG)TRUE;
        BPCI_WakeUpTask(EVT_TASK(e));
    }
}

/******************************************************************
 * Sending out of interrupts
 ******************************************************************/
VOID __adecl EVTI_IsrSend(EVENT e)
{
    if(EVT_TASK(e) > nr_tasks || EVT_TASK(e) == ZERO || e > nr_events || e == ZERO)
        ERRI_SoftwareReset();   // i can not use the error handler out of an interrupt

    if (EVT_TASK(e))
    {
        EVT_PENDING(e) = (FLAG)TRUE;
        BPCI_IsrWakeUpTask(EVT_TASK(e));    // isr wakeup
    }
}

/******************************************************************
 * Receiving
 ******************************************************************/

BOOL EVTI_Receive(EVENT e)
{
    if(e > nr_events || e == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_EVENT_RECEIVE, EVTI_LOCATION_3, NULL);

    if (EVT_PENDING(e) == (FLAG)TRUE)
    {
        EVT_PENDING(e) = (FLAG)FALSE;
        return (BOOL)TRUE;
    }

    return (BOOL)FALSE;
}
