/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Queue management
*
*  DESCRIPTION : QUEI.C sourcefile
*
*      $Archive: /AIO/Project/INFRA/SRC/QUEI.C $
*     $Revision: 2 $
*  Last $Author: Hoffmann $
*         $Date: 12.06.97 14:44 $
*
*  First Author: Paul Schenk
* Creation Date: 21.11.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: QUEI.C $
 *
 * *****************  Version 2  *****************
 * User: Hoffmann     Date: 12.06.97   Time: 14:44
 * Updated in $/AIO/Project/INFRA/SRC
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/


#define   MOD_QUEI

#include "CDIV.H"
//#include "COMPILER.H"
#include "TYPE.H"
//#include "CNF.H"
//#include "MACRO.H"

#include "BPCI_IF.H"
#include "ERRI_IF.H"

#include "QUEI_IF.H"
#include "QUEI.H"

/*
 * Initialisation
 */
VOID QUEI_Init(VOID)
{
    register QUEUE u;

    ASSERT(!BPCI_IsStarted());

    q_free = ZERO;

    for (u = (QUEUE)CNF_MAX_NR_QUEUE_CELLS; u; u--)
    {
        QUE_HEAD(u) = (QOBJECT)ZERO;
        QUE_TAIL(u) = q_free;
        q_free      = u;
    }
}

/*
 * QUEI_Put
 */

BOOL QUEI_Put(QUEUE * q, QOBJECT x)
{
    register QUEUE u;

    if( q_free )
    {
        u = q_free;
        q_free = QUE_TAIL(u);
        QUE_HEAD(u) = x;

        if( !(*q) )
            QUE_TAIL(u) = u;
        else
        {
            QUE_TAIL(u) = QUE_TAIL(*q);
            QUE_TAIL(*q) = u;
        }
        *q = u;
        return (BOOL)TRUE;
    }
    return (BOOL)FALSE;
}

VOID QUEI_Get(QUEUE * q, QOBJECT * x)
{
    register QUEUE u;

  #ifdef PCV
    ASSERT(*q != ZERO);
  #else
    if(*q == ZERO || *q > ((QUEUE) CNF_MAX_NR_QUEUE_CELLS))
        ERRI_SoftwareReset();   // system is not longer stable
  #endif

    u = QUE_TAIL(*q);

    if (u == *q)
        *q           = ZERO;
    else
        QUE_TAIL(*q) = QUE_TAIL(u);

    *x          = QUE_HEAD(u);
    QUE_TAIL(u) = q_free;
    q_free      = u;
}

#if (0) // uncalled
VOID QUEI_Append(QUEUE * q, QUEUE * r)
{
    register QUEUE u;

    if ( *r )
    {
        if ( *q )
        {
            u            = QUE_TAIL(*r);
            QUE_TAIL(*r) = QUE_TAIL(*q);
            QUE_TAIL(*q) = u;
        }

        *q = *r;
        *r = ZERO;
    }
}
#endif

UCHAR QUEI_Len(QUEUE q)
{
    register QUEUE r;
    register UCHAR n;

    n = 0;

    if ( q )
    {
        r = q;
        do
        {
            r = QUE_TAIL(r);
            n++;
        }
        while(  (r != q)    &&  /* end of queue */
                (r != ZERO) &&                          /* illegal value */
                (n <= ((QUEUE) CNF_MAX_NR_QUEUE_CELLS)) /* illegal value */
             );

        // check if queue has been overwritten
        if((r == ZERO) || (n > ((QUEUE) CNF_MAX_NR_QUEUE_CELLS)))
            ERRI_SoftwareReset();   // system is not longer stable
    }
    return n;
}
