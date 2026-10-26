/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Mailbox management
*
*  DESCRIPTION : MBXI.H local headerfile
*
*      $Archive: /AIO/Project/INFRA/SRC/MBXI.H $
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
*      $History: MBXI.H $
 * 
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/SRC
*
****************************************************************************/

#ifdef  MOD_MBXI
#ifndef  MOD_H_MBXI
#define  MOD_H_MBXI
/*******************************************************************************
* Includefiles
*******************************************************************************/


/*******************************************************************************
* Defines
*******************************************************************************/


/*******************************************************************************
* Typedefs
*******************************************************************************/

struct MCBlock          /* mailbox control blocks */
{
#ifdef CNF_ASSERTING
    STRING  name;       /* name of the mailbox                    */
#endif
    QUEUE   rq;         /* receiver message queue                 */
    TASK    task;       /* receiver task connected to the mailbox */
};

/*******************************************************************************
* Variables             STATIC
*******************************************************************************/

static struct MCBlock mbx_base[CNF_MAX_NR_MAILBOXES];
static MAILBOX __tiny nr_mailboxes;   /* number of mailboxes */

/*******************************************************************************
* Macros
*******************************************************************************/

#define MBX_FIELD( x, f )   ( mbx_base[(x)-1].f )

#ifdef CNF_ASSERTING
#define MBX_NAME( x )       MBX_FIELD( x, name )
#endif

#define MBX_RQ( x )         MBX_FIELD( x, rq )
#define MBX_TASK( x )       MBX_FIELD( x, task )

/*******************************************************************************
* Functionprototypes    STATIC
*******************************************************************************/

#endif
#else
#error YOU ARE NOT OWNER OF THIS MODULE !!
#endif

