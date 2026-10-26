/************************************************************************************
                                  <o<onut
************************************************************************************/
 #ifndef MOD_CNF
 #define MOD_CNF

 #include <stdint.h>

 #define CNF_MAX_NR_TASKS        16
 #define CNF_MAX_NR_EVENTS       8
 #define CNF_MAX_NR_MAILBOXES    8
 #define CNF_MAX_NR_TIMERS       8
 #define CNF_MAX_NR_QUEUE_CELLS  32
 #define CNF_MAX_TASK_PRIORITY   255
 #define CNF_MAX_TIMERDELAY      65535

 #define OFF 0
 #define ON  1

 #define MSEC / 8

 #define CNF_ASSERTING

 #ifdef CNF_ASSERTING
 #define ASSERT(cond) do { if (!(cond)) __builtin_trap(); } while (0)
 #else
 #define ASSERT(cond) do { (void)sizeof(cond); } while (0)
 #endif

 #endif
