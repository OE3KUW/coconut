/*******************************************************************************
1995 - 2027


                                 ƈ o ƈ o n u t


                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef QUEI_IF_H
#define QUEI_IF_H

#include "types.h"

typedef uint8_t QUEUE;
typedef UCHAR QOBJECT;

/******************************************************************
 * PURPOSE: Initialisiert den gemeinsamen Speicher der verketteten Queues.
 * POST: Alle Queue-Zellen sind frei und alle bisherigen Inhalte verworfen.
 ******************************************************************/
void QUEI_Init(void);

/******************************************************************
 * PURPOSE: Hängt ein Objekt an das Ende einer Queue an.
 * IN: queue - Zeiger auf die zu erweiternde Queue.
 * IN: value - Anzuhängendes Objekt.
 * OUT: TRUE bei Erfolg, FALSE wenn keine Queue-Zelle frei ist.
 * POST: Bei Erfolg enthält die Queue das Objekt als letzten Eintrag.
 ******************************************************************/
BOOL QUEI_Put(QUEUE *queue, QOBJECT value);

/******************************************************************
 * PURPOSE: Entnimmt das älteste Objekt aus einer Queue.
 * IN: queue - Zeiger auf die auszulesende Queue.
 * IN: value - Zeiger auf den Speicherplatz für das entnommene Objekt.
 * POST: Der erste Eintrag ist entfernt und seine Queue-Zelle wieder frei.
 ******************************************************************/
void QUEI_Get(QUEUE *queue, QOBJECT *value);

/******************************************************************
 * PURPOSE: Hängt den vollständigen Inhalt einer Queue an eine andere an.
 * IN: queue - Zeiger auf die Ziel-Queue.
 * IN: other - Zeiger auf die anzuhängende Quell-Queue.
 * POST: Alle Einträge aus other gehören zu queue; other ist danach leer.
 ******************************************************************/
void QUEI_Append(QUEUE *queue, QUEUE *other);

/******************************************************************
 * PURPOSE: Ermittelt die Anzahl der Objekte in einer Queue.
 * IN: queue - ID der abzufragenden Queue.
 * OUT: Anzahl der aktuell enthaltenen Objekte.
 ******************************************************************/
UCHAR QUEI_Len(QUEUE queue);

#endif
