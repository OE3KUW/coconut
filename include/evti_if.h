/******************************************************************
1995 - 2026


                          < o < o n u t


                                                    қuran Wolfgang
******************************************************************/
#ifndef  MOD_H_IF_EVTI
#define  MOD_H_IF_EVTI

/*******************************************************************************
* Includes
*******************************************************************************/

#include "bpci_if.h"

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

/********************************************************* @trans @Htrans */
#define EVTI_Create()    mEVTI_Create()
/* @END.Htrans
 *  IN      none
 *  PRE
 *  POST    the control block of the event is initialised
 *  RETURN  ((EVENT)(EventId)) <> NULL_EVT if creation was a success
 *  INFO
 @END**********************************************************************/

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


EXT EVENT   mEVTI_Create(VOID);

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
