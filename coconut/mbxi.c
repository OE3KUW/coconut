/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Mailbox management
*
*  DESCRIPTION : MBXI.C sourcefile
*
*      $Archive: /AIO/Project/INFRA/SRC/MBXI.C $
*     $Revision: 1 $
*  Last $Author: Hoffmann $
*         $Date: 7.03.97 8:18 $
*
*  First Author: Paul Schenk
* Creation Date: 20.11.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: MBXI.C $
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/


#define   MOD_MBXI

#include "coco_types.h"
#include "cnf.h"
#include "legacy_macros.h"
//#include "CNF.H"
//#include "MACRO.H"
#include "erri_if.h"

#include "bpci_if.h"
#include "quei_if.h"

#include "mbxi_if.h"
#include "mbxi.h"

/*
 * Initialisation
 */

VOID MBXI_Init(VOID)
{
    ASSERT(!BPCI_IsStarted());

    nr_mailboxes = 0;
}

/*
 * Creation
 */

#ifdef CNF_ASSERTING
MAILBOX mMBXI_Create(STRING s)
#else
MAILBOX mMBXI_Create(VOID)
#endif
{
    ASSERT(!BPCI_IsStarted());
    ASSERT(nr_mailboxes < CNF_MAX_NR_MAILBOXES);

#ifndef PCV
    if(nr_mailboxes >= CNF_MAX_NR_MAILBOXES)
		ERRI_vErrorHandler(ERR_NO_MORE_MAILBOXES, MBXI_LOCATION_4, NULL);
#endif

    ++nr_mailboxes;

#ifdef CNF_ASSERTING
    MBX_NAME( nr_mailboxes )  = s;
#endif

    MBX_RQ( nr_mailboxes )    = ZERO;
    MBX_TASK( nr_mailboxes )  = ZERO;

    return nr_mailboxes;
}

/*
 * Connection
 */

VOID MBXI_Connect(MAILBOX m, TASK t)
{
  #ifdef PCV
    ASSERT(!(MBX_TASK(m) != ZERO || t > nr_tasks || t == ZERO || m > nr_mailboxes || m == ZERO));
  #else
    if(MBX_TASK(m) != ZERO || t > nr_tasks || t == ZERO || m > nr_mailboxes || m == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_MBX_CONNECT, MBXI_LOCATION_1, NULL);
  #endif

    ASSERT(MBX_TASK(m) == ZERO);

    MBX_TASK(m) = t;
}

#if (0) // uncalled
VOID MBXI_Disconnect(MAILBOX m, TASK t)
{
    if (MBX_TASK(m) == t)
    MBX_TASK(m) = ZERO;
}
#endif

/*
 * Sending
 *
 */

BOOL MBXI_Send(MAILBOX m, MAIL x)
{
  #ifdef PCV
    ASSERT(!(MBX_TASK(m) == ZERO || m > nr_mailboxes || m == ZERO));
  #else
    if(MBX_TASK(m) == ZERO || m > nr_mailboxes || m == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_MBX_SEND, MBXI_LOCATION_2, NULL);
  #endif

    ASSERT(MBX_TASK(m) != ZERO);

    if (QUEI_Put(&MBX_RQ(m), x ))
    {
        BPCI_WakeUpTask(MBX_TASK(m));
        return (BOOL)TRUE;
    }
    return (BOOL)FALSE;
}

/*
 * Receiving
 */

UCHAR MBXI_Count(MAILBOX m)
{
    return QUEI_Len(MBX_RQ(m));
}

BOOL MBXI_Receive(MAILBOX m, MAIL * x)
{
  #ifdef PCV
    ASSERT(!(MBX_TASK(m) == ZERO || m > nr_mailboxes || m == ZERO));
  #else
    if(MBX_TASK(m) == ZERO || m > nr_mailboxes || m == ZERO)
		ERRI_vErrorHandler(ERR_INVAL_MBX_RECEIVE, MBXI_LOCATION_3, NULL);
  #endif

    if (MBX_RQ(m))
    {
        QUEI_Get(&MBX_RQ(m), x);
        return (BOOL)TRUE;
    }
    return (BOOL)FALSE;
}
