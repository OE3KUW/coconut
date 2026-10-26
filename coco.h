#ifndef MOD_H_COCO
#define MOD_H_COCO
/****************************************************************************/
/*                                                                          */
/*                       8051 VERSION                                       */
/*                                                                          */
/****************************************************************************/
#ifndef PCV
#include "CNF.H"          // CONFIGURATION
#include "EVTI_IF.H"
#include "MBXI_IF.H"
#include "QUEI_IF.H"
#include "TIMI_IF.H"
#include "BPCI_IF.H"
#include "MALI_IF.H"
#else
/****************************************************************************/
/*                                                                          */
/*                         PC VERSION                                       */
/*                                                                          */
/****************************************************************************/
#include "CNF.H"          // CONFIGURATION
//#include "MACRO.H"
#include "EVTI_IF.H"
#include "MBXI_IF.H"
#include "QUEI_IF.H"
#include "TIMI_IF.H"
#include "BPCI_IF.H"
#include "MALI_IF.H"
#endif  // PCV
#endif  // MOD_COCO

