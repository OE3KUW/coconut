/****************************************************************************
*   PROJECT    : PAOLINA
*****************************************************************************
*
*   SUBPROJECT : <o<oNut
*
*       MODULE : Event management
*
*  DESCRIPTION : EVTI_IF.H interface headerfile
*
*      $Archive: /AIO/Project/INFRA/INC/EVTI_IF.H $
*     $Revision: 4 $
*  Last $Author: Schenk $
*         $Date: 23.06.97 11:12 $
*
*  First Author: Paul Schenk
* Creation Date: 20.11.95
*
*****************************************************************************
* MODIFICATIONS
*****************************************************************************
*
*      $History: EVTI_IF.H $
 *
 * *****************  Version 4  *****************
 * User: Schenk       Date: 23.06.97   Time: 11:12
 * Updated in $/AIO/Project/INFRA/INC
 * problem with last line corrected
 *
 * *****************  Version 3  *****************
 * User: Schenk       Date: 21.06.97   Time: 14:38
 * Updated in $/AIO/Project/INFRA/INC
 * first draft of interface description macros added
 *
 * *****************  Version 2  *****************
 * User: Hoffmann     Date: 2.04.97    Time: 14:50
 * Updated in $/AIO/Project/INFRA/INC
 *
 * *****************  Version 1  *****************
 * User: Hoffmann     Date: 7.03.97    Time: 8:18
 * Created in $/AIO/Project/INFRA/INC
*
****************************************************************************/

#ifndef  MOD_H_IF_EVTI
#define  MOD_H_IF_EVTI

/*******************************************************************************
* Includes
*******************************************************************************/

#include "BPCI_IF.H"

#ifdef   MOD_EVTI
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
#define NULL_EVT    ((EVENT)ZERO)
/* @END.Hadt
 *  INFO    symbolic name for event of type NIL; used as return value if
 *          creation of a event has failed
 *  ACCESS
 @END**********************************************************************/


/*******************************************************************************
* Typedefs
*******************************************************************************/

/*********************************************************** @adt   @Hadt */
typedef UCHAR   EVENT;
/* @END.Hadt
 *  INFO    event object type
 *  ACCESS
 @END**********************************************************************/


/*******************************************************************************
* Variables EXT CONST or EXT
*******************************************************************************/

/*******************************************************************************
* Macros
*******************************************************************************/

#ifdef CNF_ASSERTING
/********************************************************* @trans @Htrans */
#define EVTI_Create(s,p)    mEVTI_Create(s)
/* @END.Htrans
 *  IN      STRING s:   name of the event
 *          PROCESS p:  owner process of the event
 *  PRE
 *  POST    the control block of the event is initialised
 *  RETURN  ((EVENT)(EventId)) <> NULL_EVT if creation was a success
 *  INFO
 @END**********************************************************************/

#else
#define EVTI_Create(s,p)    mEVTI_Create()
#endif

/*******************************************************************************
* Functionprototypes EXT
*******************************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID    EVTI_Init(VOID);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST    the component is ready for use
 *  RETURN
 *  INFO
 @END**********************************************************************/


#ifdef CNF_ASSERTING
EXT EVENT   mEVTI_Create(STRING s);
#else
EXT EVENT   mEVTI_Create(VOID);
#endif

/********************************************************* @trans @Htrans */
EXT VOID    EVTI_Connect(EVENT e, TASK t);
/* @END.Htrans
 *  IN      EVENT e:    a valid event identification
 *          TASK t:     a valid task identification which is member of the
 *                      event owner process
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID    EVTI_Disconnect(EVENT e, TASK t);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT BOOL    EVTI_Receive(EVENT e);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/

/********************************************************* @trans @Htrans */
EXT VOID    EVTI_Send(EVENT e);
/* @END.Htrans
 *  IN
 *  PRE
 *  POST
 *  RETURN
 *  INFO
 @END**********************************************************************/


// no errorhandling via ERRI
/********************************************************* @trans @Htrans */
EXT VOID __adecl   EVTI_IsrSend(EVENT e);
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
