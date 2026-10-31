/******************************************************************
1995 - 2027


                          < o < o n u t


                                                    қuran Wolfgang
******************************************************************/
#ifndef ERRI_IF_H
#define ERRI_IF_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    ERR_BAD_STACK,
    ERR_INVAL_TASK,
    ERR_NO_MORE_TASKS,
    ERR_NO_MORE_EVENTS,
    ERR_INVAL_EVENT_CONNECT,
    ERR_INVAL_EVENT_SEND,
    ERR_INVAL_EVENT_RECEIVE,
    ERR_NO_MORE_MAILBOXES,
    ERR_INVAL_MBX_CONNECT,
    ERR_INVAL_MBX_SEND,
    ERR_INVAL_MBX_RECEIVE,
    ERR_NO_MORE_TIMERS,
    ERR_INVAL_TIMER_CONNECT,
    ERR_INVAL_TIMER_START,
    ERR_INVAL_TIMER_STOP,
    ERR_INVAL_TIMER_EVENT,
    ERR_INVAL_TIMER_MODE,
    ERR_INVAL_TIMER_DELAY
} ERRI_Error;

/******************************************************************
 * PURPOSE: Meldet einen internen Coconut-Fehler und löst die
 *         vorgesehene Fehlerreaktion aus.
 * IN: error - Art des erkannten Fehlers.
 * IN: location - Kennung der Fehlerstelle innerhalb des Moduls.
 * IN: context - Optionaler Zeiger auf ergänzende Fehlerinformationen.
 * POST: Die Fehlerbehandlung wurde ausgelöst; auf dem AVR kehrt die
 *       Funktion nicht zurück.
 ******************************************************************/
void ERRI_vErrorHandler(ERRI_Error error, uint8_t location, const void *context);

/******************************************************************
 * PURPOSE: Versetzt den Mikrocontroller in den sicheren Stoppzustand.
 * POST: Interrupts sind gesperrt und das Programm verbleibt in einer
 *       Endlosschleife.
 ******************************************************************/
void ERRI_SoftwareReset(void);

#define BPCI_LOCATION_0 0
#define BPCI_LOCATION_1 1
#define BPCI_LOCATION_2 2
#define BPCI_LOCATION_3 3
#define BPCI_LOCATION_4 4
#define EVTI_LOCATION_1 1
#define EVTI_LOCATION_2 2
#define EVTI_LOCATION_3 3
#define EVTI_LOCATION_4 4
#define MBXI_LOCATION_1 1
#define MBXI_LOCATION_2 2
#define MBXI_LOCATION_3 3
#define MBXI_LOCATION_4 4
#define TIMI_LOCATION_1 1
#define TIMI_LOCATION_2 2
#define TIMI_LOCATION_3 3
#define TIMI_LOCATION_4 4
#define TIMI_LOCATION_5 5
#define TIMI_LOCATION_6 6
#define TIMI_LOCATION_7 7

#endif
