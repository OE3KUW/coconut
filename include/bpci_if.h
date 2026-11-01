/*******************************************************************************
1995 - 2027


                      𝘊 o 𝘊 o n u t   4  𝙴 d u c a t i o n

                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef MOD_H_IF_BPCI
#define MOD_H_IF_BPCI

#include "cnf.h"
#include "types.h"

#ifdef MOD_BPCI
#define EXT
#else
#define EXT extern
#endif

/******************************************************************
 * Types
 ******************************************************************/

/* Process types are retained for compatibility with the original API. */
typedef UCHAR PROCESS;
typedef UCHAR PROCESS_PRIORITY;

typedef UCHAR TASK;
typedef UCHAR TASK_PRIORITY;
typedef UCHAR FLAG;
typedef VOID (*PPV)(VOID);

#define NULL_TSK ((TASK)ZERO)
#define NULL_PRO ((PROCESS)ZERO)

/******************************************************************
 * Task control block
 ******************************************************************/

struct TCBlock
{
    PPV action;
    TASK_PRIORITY priority;
    FLAG flag;
    TASK next;
    TASK prev;
};

/* Internal module state. Access it through the macros and functions below. */
EXT struct TCBlock __near bpc_t_base[CNF_MAX_NR_TASKS];
EXT TASK __tiny t_first;
EXT TASK __tiny t_last;
EXT TASK __tiny t_self;
EXT BIT __tiny system_is_running;
EXT TASK __tiny nr_tasks;

/******************************************************************
 * State access
 ******************************************************************/

#define BPCI_GetCurrentProcess() ((PROCESS)ZERO)
#define BPCI_GetCurrentTask()    t_self
#define BPCI_IsStarted()         system_is_running
#define SYSI_IsStarted()         system_is_running

#define BPC_T_FIELD(task, field) (bpc_t_base[(task) - 1].field)
#define BPC_T_ACTION(task)       BPC_T_FIELD(task, action)
#define BPC_T_PRIORITY(task)     BPC_T_FIELD(task, priority)
#define BPC_T_FLAG(task)         BPC_T_FIELD(task, flag)
#define BPC_T_NEXT(task)         BPC_T_FIELD(task, next)
#define BPC_T_PREV(task)         BPC_T_FIELD(task, prev)

/******************************************************************
 * Compatibility macros
 ******************************************************************/

/* The AVR port does not require additional interrupt bookkeeping. */
#define BPCI_INTERRUPT_STORE
#define BPCI_EnterInterrupt(process)
#define BPCI_LeaveInterrupt()

/* The original process layer is not used by this port. */
#define BPCI_CreateInterruptProcess(name) ((PROCESS)ZERO)
#define BPCI_CreateTask(action, priority) \
    mBPCI_CreateTask((action), (priority))

/******************************************************************
 * Public interface
 ******************************************************************/

/******************************************************************
 * PURPOSE: Initialisiert die historische BPCI-Taskverwaltung.
 * POST: Alle Taskkontrollblöcke und Ready-Listen sind zurückgesetzt.
 ******************************************************************/
EXT VOID BPCI_Init(VOID);

/******************************************************************
 * PURPOSE: Legt einen kooperativen BPCI-Task an.
 * IN: action - Taskfunktion, die der Scheduler aufrufen soll.
 * IN: priority - Numerische Priorität des Tasks.
 * OUT: Neue Task-ID oder NULL_TSK, wenn kein Task angelegt wurde.
 * POST: Ein neuer Taskkontrollblock ist dauerhaft belegt.
 ******************************************************************/
EXT TASK mBPCI_CreateTask(PPV action, TASK_PRIORITY priority);

/******************************************************************
 * PURPOSE: Markiert einen Task als bereit und reiht ihn nach Priorität ein.
 * IN: task - ID des aufzuweckenden Tasks.
 * POST: Der Task steht einmal in der Ready-Liste.
 ******************************************************************/
EXT VOID BPCI_WakeUpTask(TASK task);

/******************************************************************
 * PURPOSE: Interrupt-sichere Variante von BPCI_WakeUpTask().
 * IN: task - ID des aus einem Interrupt aufzuweckenden Tasks.
 * POST: Der Task steht einmal in der Ready-Liste.
 ******************************************************************/
EXT VOID BPCI_IsrWakeUpTask(TASK task);

#undef EXT
#endif
