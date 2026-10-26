#ifndef QUEI_IF_H
#define QUEI_IF_H

#include "coco_types.h"

typedef uint8_t QUEUE;
typedef UCHAR QOBJECT;

void QUEI_Init(void);
BOOL QUEI_Put(QUEUE *queue, QOBJECT value);
void QUEI_Get(QUEUE *queue, QOBJECT *value);
void QUEI_Append(QUEUE *queue, QUEUE *other);
UCHAR QUEI_Len(QUEUE queue);

#endif
