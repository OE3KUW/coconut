/******************************************************************
1995 - 2027


                          < o < o n u t


                                                    қuran Wolfgang
******************************************************************/
#ifndef MOD_H_IF_TIMI
#define MOD_H_IF_TIMI

#include "bpci_if.h"

#ifdef MOD_TIMI
#define EXT
#else
#define EXT extern
#endif

/******************************************************************
 * Types and constants
 ******************************************************************/

typedef UCHAR TIMER;
typedef UINT TIMERDELAY;
typedef UCHAR TIMERMODE;

#define NULL_TMR      ((TIMER)ZERO)
#define TIMI_PERIODIC ((TIMERMODE)0)
#define TIMI_ONESHOT  ((TIMERMODE)1)
#define DEF_TIME      ((TIMERDELAY)1)

/* Number of hardware ticks waiting for timer processing. */
EXT UCHAR __tiny elapsed_ticks;

/******************************************************************
 * Creation
 ******************************************************************/

/* Create a wake-up timer and return its identifier. */
#define TIMI_CreateWakeUpTimer(mode, delay) \
    mTIMI_CreateWakeUpTimer((mode), (delay))

/******************************************************************
 * Public interface
 ******************************************************************/

/******************************************************************
 * PURPOSE: Initialisiert die historische Software-Timerverwaltung.
 * POST: Alle Timer und die Delta-Liste sind zurückgesetzt.
 ******************************************************************/
EXT VOID TIMI_Init(VOID);

/******************************************************************
 * PURPOSE: Aktiviert den historischen Hardware-Timerdienst.
 * POST: Der historische Timerdienst ist aktiviert, sofern unterstützt.
 ******************************************************************/
EXT VOID TIMI_Activate(VOID);

/******************************************************************
 * PURPOSE: Legt einen periodischen oder einmaligen Wake-up-Timer an.
 * IN: mode - TIMI_PERIODIC oder TIMI_ONESHOT.
 * IN: delay - Verzögerung in Coconut-Ticks.
 * OUT: Neue Timer-ID oder NULL_TMR, wenn kein Timer angelegt wurde.
 * POST: Ein neuer Timerkontrollblock ist dauerhaft belegt.
 ******************************************************************/
EXT TIMER mTIMI_CreateWakeUpTimer(TIMERMODE mode, TIMERDELAY delay);

/******************************************************************
 * PURPOSE: Verbindet einen Timer mit dem aufzuweckenden Task.
 * IN: timer - ID des Timers.
 * IN: task - ID des Tasks, der beim Ablauf geweckt wird.
 * POST: Der Timer besitzt dauerhaft den angegebenen Empfänger.
 ******************************************************************/
EXT VOID TIMI_Connect(TIMER timer, TASK task);

/******************************************************************
 * PURPOSE: Trennt einen Timer von seinem bisherigen Empfänger-Task.
 * IN: timer - ID des Timers.
 * IN: task - Erwartete ID des bisherigen Empfänger-Tasks.
 * POST: Die Verbindung ist entfernt, wenn sie zu task gehört hat.
 ******************************************************************/
EXT VOID TIMI_Disconnect(TIMER timer, TASK task);

/******************************************************************
 * PURPOSE: Startet einen Timer oder startet ihn mit voller Dauer neu.
 * IN: timer - ID des zu startenden Timers.
 * POST: Der Timer ist mit seiner vollständigen Verzögerung aktiv.
 ******************************************************************/
EXT VOID TIMI_Start(TIMER timer);

/******************************************************************
 * PURPOSE: Stoppt einen Timer und löscht sein Ablaufereignis.
 * IN: timer - ID des zu stoppenden Timers.
 * POST: Der Timer ist inaktiv und nicht mehr als abgelaufen markiert.
 ******************************************************************/
EXT VOID TIMI_Stop(TIMER timer);

/******************************************************************
 * PURPOSE: Ändert einen Timer zwischen periodisch und einmalig.
 * IN: timer - ID des Timers.
 * IN: mode - TIMI_PERIODIC oder TIMI_ONESHOT.
 * POST: Der Betriebsmodus des Timers ist dauerhaft geändert.
 ******************************************************************/
EXT VOID TIMI_SetMode(TIMER timer, TIMERMODE mode);

/******************************************************************
 * PURPOSE: Ändert die Verzögerung eines Timers.
 * IN: timer - ID des Timers.
 * IN: delay - Neue Verzögerung in Coconut-Ticks.
 * POST: Die Verzögerung ist geändert; ein aktiver Timer wurde neu gestartet.
 ******************************************************************/
EXT VOID TIMI_SetDelay(TIMER timer, TIMERDELAY delay);

/******************************************************************
 * PURPOSE: Prüft, ob ein Timer aktiv oder bereits abgelaufen ist.
 * IN: timer - ID des zu prüfenden Timers.
 * OUT: TRUE bei aktivem oder abgelaufenem Timer, sonst FALSE.
 ******************************************************************/
EXT BOOL TIMI_IsTimer(TIMER timer);

/******************************************************************
 * PURPOSE: Liest und konsumiert das Ablaufereignis eines Timers.
 * IN: timer - ID des abzufragenden Timers.
 * OUT: TRUE, wenn der Timer abgelaufen war, andernfalls FALSE.
 * POST: Das Ablauf-Flag des Timers ist gelöscht.
 ******************************************************************/
EXT BOOL TIMI_Event(TIMER timer);

/******************************************************************
 * PURPOSE: Verarbeitet gesammelte Ticks und fällige Software-Timer.
 * POST: Timerstände, Delta-Liste und Empfänger-Tasks sind aktualisiert.
 ******************************************************************/
EXT VOID TIMI_TaskFunction(VOID);

#undef EXT
#endif
