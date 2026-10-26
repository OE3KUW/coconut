/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Basic process and task management
*
*  DESCRIPTION : BPCI_IF.H interface headerfile
*
*      $Archive: /AIO/Project/INFRA/INC/BPCI_IF.H $
*     $Revision: 3 $
*  Last $Author: Schenk $
*         $Date: 23.06.97 11:12 $
*
*  First Author: Paul Schenk
* Creation Date: 01.12.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: BPCI_IF.H $
 *
 * *****************  Version 3  *****************
 * User: Schenk       Date: 23.06.97   Time: 11:12
 * Updated in $/AIO/Project/INFRA/INC
 * problem with last line corrected
 *
 * *****************  Version 2  *****************
 * User: Schenk       Date: 21.06.97   Time: 14:38
 * Updated in $/AIO/Project/INFRA/INC
 * first draft of interface description macros added
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/INC
*
****************************************************************************/

#ifndef  MOD_H_IF_BPCI
#define  MOD_H_IF_BPCI

/*******************************************************************************
* Includes
*******************************************************************************/

#include "CNF.H"
#include "type.h"

#ifdef   MOD_BPCI
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
#define NULL_TSK    ((TASK)ZERO)
/* @END.Hadt
 *  INFO    symbolic name for task of type NIL; used as return value if
 *          creation of a task has failed
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
#define NULL_PRO    ((PROCESS)ZERO)
/* @END.Hadt
 *  INFO    symbolic name for process of type NIL; used as return value if
 *          creation of a process has failed
 * ACCESS
 @END**********************************************************************/


/*******************************************************************************
* Typedefs
*******************************************************************************/

//####################################
//##only for compatibility reasons####
/*********************************************************** @adt   @Hadt */
typedef UCHAR        PROCESS;
/* @END.Hadt
 *  INFO    data type for processes
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR        PROCESS_PRIORITY;
/* @END.Hadt
 *  INFO    data type for process priority
 *  ACCESS
 @END**********************************************************************/

//####################################

/*********************************************************** @adt   @Hadt */
typedef UCHAR        TASK;
/* @END.Hadt
 *  INFO    data type for tasks
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR        TASK_PRIORITY;
/* @END.Hadt
 *  INFO    data type for task priority
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR        FLAG;               /* flag type                     */
/* @END.Hadt
 *  INFO    data type for flag
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef const char * STRING;            /* pointer to c-string           */
/* @END.Hadt
 *  INFO    data type for strings
 *  ACCESS
 @END**********************************************************************/

/*********************************************************** @adt   @Hadt */
typedef VOID (* PPV)(VOID );             /* void  para  procedure pointer */
/* @END.Hadt
 *  INFO    data type for void paraeter procedure pointer
 *  ACCESS
 @END**********************************************************************/

struct TCBlock                           /* task control block      (8) 6 */
{
#ifdef CNF_ASSERTING
    STRING              name;            /* name of the task           (2)*/
#endif
    PPV                 action;          /* void (* action)(void)       2 */
    TASK_PRIORITY       priority;        /* priority of the task        1 */
    FLAG                flag;            /* awake                       1 */
    TASK                next;            /* next active task            1 */
    TASK                prev;            /* previous active task        1 */
};

/*******************************************************************************
* Variables EXT CONST or EXT
*
* NOTE: it is not allowed to access these variables although they are decleared to
*       be external; this is only necessary because of error checking or because
*       they are accessed via macros !!
*******************************************************************************/

EXT  struct TCBlock __near bpc_t_base[CNF_MAX_NR_TASKS];

EXT   TASK   __tiny   t_first;           /* first task      */
EXT   TASK   __tiny   t_last;            /* last task       */
EXT   TASK   __tiny   t_self;            /* current task    */
EXT   BIT    __tiny   system_is_running; /* system active   */
EXT   TASK   __tiny   nr_tasks;          /* number of tasks      */

/*******************************************************************************
* Macros
*******************************************************************************/

/*********************************************************** @obs   @Hobs */
#define BPCI_GetCurrentProcess()    ((PROCESS)0)
/* @END.Hobs
 *  IN
 *  PRE     TRUE
 *  POST
 *  RETURN  get the identification of the actual active process
 *  INFO
 @END**********************************************************************/


/*********************************************************** @obs   @Hobs */
#define BPCI_GetCurrentTask()       t_self
/* @END.Hobs
 *  IN
 *  PRE     TRUE
 *  POST
 *  RETURN  get the identification of the actual active task
 *  INFO
 @END**********************************************************************/


/*********************************************************** @obs   @Hobs */
#define BPCI_IsStarted()            system_is_running
/* @END.Hobs
 *  IN
 *  PRE     TRUE
 *  POST
 *  RETURN  TRUE if coconut has been started
 *  INFO
 @END**********************************************************************/

#define SYSI_IsStarted()            system_is_running

/* macros used by interrupt processes */

/********************************************************** @pubv  @Hpubv */
#define BPCI_INTERRUPT_STORE
/* @END.Hpubv
 *  INFO
 *  ACCESS
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
#define BPCI_EnterInterrupt(p)
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
#define BPCI_LeaveInterrupt()
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/


/*******************************************************************************
* Internal access macros (located here because ASSERTS only !)
*******************************************************************************/

#define BPC_T_FIELD( x, f )   ( bpc_t_base[(x)-1].f )

#ifdef CNF_ASSERTING
#define BPC_T_NAME( x )       BPC_T_FIELD( x, name )
#endif

#define BPC_T_ACTION( x )     BPC_T_FIELD( x, action )
#define BPC_T_PRIORITY( x )   BPC_T_FIELD( x, priority )
#define BPC_T_FLAG( x )       BPC_T_FIELD( x, flag )
#define BPC_T_NEXT( x )       BPC_T_FIELD( x, next )
#define BPC_T_PREV( x )       BPC_T_FIELD( x, prev )


/********************************************************* @trans @Htrans */
#define BPCI_CreateInterruptProcess(s)  ((PROCESS)0)
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/


#ifdef CNF_ASSERTING
/********************************************************* @trans @Htrans */
#define BPCI_CreateTask(s,c,p,n)    mBPCI_CreateTask(s,c,n)
/* @END.Htrans
 *  IN      STRING s, PPV c, PROCESS p, TASK_PRIORITY n
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

#else
#define BPCI_CreateTask(s,c,p,n)    mBPCI_CreateTask(c,n)
#endif

/*******************************************************************************
* Functionprototypes EXT
*******************************************************************************/

/********************************************************* @trans @Htrans */
VOID BPCI_Init(VOID);
/* @END.Htrans
 *  IN
 *  PRE     has not been called before
 *  POST    all internal data structures are initialised
 *  RETURN
 *  INFO    should be called once only during startup
 @END**********************************************************************/


#ifdef CNF_ASSERTING
EXT TASK    mBPCI_CreateTask(STRING s, PPV c, TASK_PRIORITY n);
#else
EXT TASK    mBPCI_CreateTask(PPV c, TASK_PRIORITY n);
#endif

/********************************************************* @trans @Htrans */
EXT VOID BPCI_WakeUpTask(TASK t);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID BPCI_IsrWakeUpTask(TASK t);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

#undef   EXT
#undef   CONST
#endif
/* be sure to have the last preprocessor command on a seperate line */
