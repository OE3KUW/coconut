#ifndef COCO_TYPES_H
#define COCO_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef void VOID;
typedef bool BOOL;
typedef uint8_t BIT;
typedef uint8_t UCHAR;
typedef uint16_t UINT;
typedef uint32_t ULONG;
typedef const char *STRING;
typedef VOID (*PPV)(VOID);

#define TRUE true
#define FALSE false
#define ZERO 0
#define __tiny
#define __near
#define __adecl

#endif
