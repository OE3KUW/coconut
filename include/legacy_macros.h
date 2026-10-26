#ifndef LEGACY_MACROS_H
#define LEGACY_MACROS_H

#include <avr/interrupt.h>
#include <util/atomic.h>

#define USES_CRITICAL_SECTION
#define ENTER_CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#define LEAVE_CRITICAL_SECTION }
#define DBIT(bit) ((UCHAR)1u << (bit))
#define SetBit(value, bit) ((value) |= (bit))
#define ClrBit(value, bit) ((value) &= (UCHAR)~(bit))
#define ClearBitInByte(value, bit) ClrBit(value, bit)
#define GetBit(value, bit) (((value) & (bit)) != 0u)

#endif
