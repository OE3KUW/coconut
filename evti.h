/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Event management
*
*  DESCRIPTION : EVTI.H local headerfile
*
*      $Archive: /AIO/Project/INFRA/SRC/EVTI.H $
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
*      $History: EVTI.H $
 * 
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/

#ifdef  MOD_EVTI
#ifndef  MOD_H_EVTI
#define  MOD_H_EVTI
/*******************************************************************************
* Includefiles
*******************************************************************************/


/*******************************************************************************
* Defines
*******************************************************************************/


/*******************************************************************************
* Typedefs
*******************************************************************************/

struct ECBlock          /* event control blocks */
{
#ifdef CNF_ASSERTING
    STRING  name;       /* name of the event                     */
#endif
    FLAG    pending;    /* event flag                            */
    TASK    task;       /* receiver task connected to the event  */
};

/*******************************************************************************
* Variables             STATIC
*******************************************************************************/

static struct ECBlock evt_base[CNF_MAX_NR_EVENTS];
static EVENT  __tiny nr_events;   /* number of events */

/*******************************************************************************
* Macros
*******************************************************************************/

#define EVT_FIELD( x, f )   ( evt_base[(x)-1].f )

#ifdef CNF_ASSERTING
#define EVT_NAME( x )       EVT_FIELD( x, name )
#endif

#define EVT_PENDING( x )    EVT_FIELD( x, pending )
#define EVT_TASK( x )       EVT_FIELD( x, task )

/*******************************************************************************
* Functionprototypes    STATIC
*******************************************************************************/


#endif
#else
#error YOU ARE NOT OWNER OF THIS MODULE !!
#endif

