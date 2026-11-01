/*******************************************************************************
1995 - 2027


                                 ƈ o ƈ o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef MBXI_IF_H
#define MBXI_IF_H

#include "bpci_if.h"

typedef UCHAR MAILBOX;
typedef UCHAR MAIL;

#define NULL_MBX ((MAILBOX)ZERO)

#define MBXI_Create() mMBXI_Create()

/******************************************************************
 * PURPOSE: Initialisiert die Mailboxverwaltung.
 * POST: Alle Mailboxen und darin gespeicherten Nachrichten sind entfernt.
 ******************************************************************/
void MBXI_Init(void);

/******************************************************************
 * PURPOSE: Legt eine neue Mailbox an.
 * OUT: Neue Mailbox-ID oder NULL_MBX, wenn keine Mailbox frei ist.
 * POST: Ein neuer Mailboxkontrollblock ist dauerhaft belegt.
 ******************************************************************/
MAILBOX mMBXI_Create(void);

/******************************************************************
 * PURPOSE: Verbindet eine Mailbox mit ihrem Empfänger-Task.
 * IN: mailbox - ID der zu verbindenden Mailbox.
 * IN: task - ID des Tasks, der bei neuer Post geweckt wird.
 * POST: Die Mailbox besitzt dauerhaft den angegebenen Empfänger.
 ******************************************************************/
void MBXI_Connect(MAILBOX mailbox, TASK task);

/******************************************************************
 * PURPOSE: Trennt eine Mailbox von ihrem bisherigen Empfänger-Task.
 * IN: mailbox - ID der zu trennenden Mailbox.
 * IN: task - Erwartete ID des bisherigen Empfänger-Tasks.
 * POST: Die Verbindung ist entfernt, wenn sie zu task gehört hat.
 ******************************************************************/
void MBXI_Disconnect(MAILBOX mailbox, TASK task);

/******************************************************************
 * PURPOSE: Legt eine Nachricht am Ende einer Mailbox ab und weckt
 *         ihren Empfänger.
 * IN: mailbox - ID der Ziel-Mailbox.
 * IN: value - Zu speichernde Nachricht.
 * OUT: TRUE bei erfolgreichem Senden, FALSE bei voller Nachrichtenqueue.
 * POST: Bei Erfolg ist die Nachricht gespeichert und der Empfänger bereit.
 ******************************************************************/
BOOL MBXI_Send(MAILBOX mailbox, MAIL value);

/******************************************************************
 * PURPOSE: Ermittelt die Anzahl der wartenden Nachrichten einer Mailbox.
 * IN: mailbox - ID der abzufragenden Mailbox.
 * OUT: Anzahl der aktuell gespeicherten Nachrichten.
 ******************************************************************/
UCHAR MBXI_Count(MAILBOX mailbox);

/******************************************************************
 * PURPOSE: Holt die älteste Nachricht aus einer Mailbox.
 * IN: mailbox - ID der abzufragenden Mailbox.
 * IN: value - Zeiger auf den Speicherplatz für die gelesene Nachricht.
 * OUT: TRUE, wenn eine Nachricht gelesen wurde, andernfalls FALSE.
 * POST: Bei Erfolg ist die älteste Nachricht aus der Mailbox entfernt.
 ******************************************************************/
BOOL MBXI_Receive(MAILBOX mailbox, MAIL *value);

#endif
