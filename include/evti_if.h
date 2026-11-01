/*******************************************************************************
1995 - 2027


                                 𝒞 o 𝒞 o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef MOD_H_IF_EVTI
#define MOD_H_IF_EVTI

#include "bpci_if.h"

#ifdef MOD_EVTI
#define EXT
#else
#define EXT extern
#endif

/******************************************************************
 * Types and constants
 ******************************************************************/

typedef UCHAR EVENT;

#define NULL_EVT ((EVENT)ZERO)

/******************************************************************
 * Creation
 ******************************************************************/

/* Create an event and return its identifier. */
#define EVTI_Create() mEVTI_Create()

/******************************************************************
 * Public interface
 ******************************************************************/

/******************************************************************
 * PURPOSE: Initialisiert die historische Eventverwaltung.
 * POST: Alle Events sind entfernt und kein Event ist pending.
 ******************************************************************/
EXT VOID EVTI_Init(VOID);

/******************************************************************
 * PURPOSE: Legt ein neues binäres Event an.
 * OUT: Neue Event-ID oder NULL_EVT, wenn kein Event angelegt wurde.
 * POST: Ein neuer Eventkontrollblock ist dauerhaft belegt.
 ******************************************************************/
EXT EVENT mEVTI_Create(VOID);

/******************************************************************
 * PURPOSE: Verbindet ein Event mit seinem Empfänger-Task.
 * IN: event - ID des zu verbindenden Events.
 * IN: task - ID des Tasks, der bei diesem Event geweckt wird.
 * POST: Das Event besitzt dauerhaft den angegebenen Empfänger.
 ******************************************************************/
EXT VOID EVTI_Connect(EVENT event, TASK task);

/******************************************************************
 * PURPOSE: Trennt ein Event von seinem bisherigen Empfänger-Task.
 * IN: event - ID des zu trennenden Events.
 * IN: task - Erwartete ID des bisherigen Empfänger-Tasks.
 * POST: Die Verbindung ist entfernt, wenn sie zu task gehört hat.
 ******************************************************************/
EXT VOID EVTI_Disconnect(EVENT event, TASK task);

/******************************************************************
 * PURPOSE: Prüft und konsumiert ein anstehendes Event.
 * IN: event - ID des abzufragenden Events.
 * OUT: TRUE, wenn das Event pending war, andernfalls FALSE.
 * POST: Das Pending-Flag des Events ist gelöscht.
 ******************************************************************/
EXT BOOL EVTI_Receive(EVENT event);

/******************************************************************
 * PURPOSE: Setzt ein Event auf pending und weckt seinen Empfänger.
 * IN: event - ID des zu sendenden Events.
 * POST: Das Event ist pending und der Empfänger steht in der Ready-Liste.
 ******************************************************************/
EXT VOID EVTI_Send(EVENT event);

/******************************************************************
 * PURPOSE: Interrupt-sichere Variante von EVTI_Send().
 * IN: event - ID des aus einem Interrupt zu sendenden Events.
 * POST: Das Event ist pending und der Empfänger steht in der Ready-Liste.
 ******************************************************************/
EXT VOID __adecl EVTI_IsrSend(EVENT event);

#undef EXT
#endif
