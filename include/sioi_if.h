/*******************************************************************************
1995 - 2027


                      𝘊 o 𝘊 o n u t   4  𝙴 d u c a t i o n

                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef SIOI_IF_H
#define SIOI_IF_H

#include <stdint.h>

/******************************************************************
 * PURPOSE: Initialisiert UART, Empfangspuffer und die serielle Zeiterfassung.
 * POST: Die serielle Schnittstelle ist betriebsbereit und ihre Zähler sind
 *       zurückgesetzt.
 ******************************************************************/
void SIOI_Init(void);

/******************************************************************
 * PURPOSE: Liest ein Zeichen ohne auf neue serielle Daten zu warten.
 * OUT: Zeichenwert von 0 bis 255 oder -1, wenn kein Zeichen bereitsteht.
 * POST: Ein zurückgegebenes Zeichen ist aus dem Empfangspuffer entfernt.
 ******************************************************************/
int SIOI_ReadChar(void);

/******************************************************************
 * PURPOSE: Liefert die seit der Initialisierung vergangene Laufzeit.
 * OUT: Vollständig vergangene Sekunden.
 ******************************************************************/
uint32_t SIOI_Seconds(void);

/******************************************************************
 * PURPOSE: Übernimmt einen Hardware-Tick in die serielle Zeiterfassung.
 * POST: Tick- und gegebenenfalls Sekundenzähler sind fortgeschrieben.
 ******************************************************************/
void SIOI_TickFromISR(void);

#endif
