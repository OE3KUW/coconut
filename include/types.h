/*******************************************************************************
1995 - 2027


                                 𝘊 o 𝘊 o n u t

                             CoConut for Education

                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#ifndef COCONUT_TYPES_H
#define COCONUT_TYPES_H

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/atomic.h>

/******************************************************************
 * Coconut data types
 ******************************************************************/

typedef void VOID;
typedef bool BOOL;
typedef uint8_t BIT;
typedef uint8_t UCHAR;
typedef uint16_t UINT;
typedef uint32_t ULONG;
typedef VOID (*PPV)(VOID);

/******************************************************************
 * Common constants
 ******************************************************************/

#define TRUE true
#define FALSE false
#define ZERO 0

/******************************************************************
 * Compatibility with the original Coconut compiler
 ******************************************************************/

#define __tiny
#define __near
#define __adecl

/******************************************************************
 * Critical sections
 ******************************************************************/

#define USES_CRITICAL_SECTION
#define ENTER_CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#define LEAVE_CRITICAL_SECTION }

/******************************************************************
 * Bit operations
 ******************************************************************/

#define DBIT(bit) ((UCHAR)1 << (bit))
#define SetBit(value, bit) ((value) |= (bit))
#define ClrBit(value, bit) ((value) &= (UCHAR)~(bit))
#define ClearBitInByte(value, bit) ClrBit(value, bit)
#define GetBit(value, bit) (((value) & (bit)) != 0)

#endif
