/*******************************************************************************
1995 - 2027


                      𝘊 o 𝘊 o n u t   4  𝙴 d u c a t i o n

                                                                     Paul Schenk
                                                                  қuran Wolfgang
*******************************************************************************/
#include "erri_if.h"

#include <avr/interrupt.h>

void ERRI_vErrorHandler(ERRI_Error error, uint8_t location, const void *context)
{
    (void)error;
    (void)location;
    (void)context;
    ERRI_SoftwareReset();
}

void ERRI_SoftwareReset(void)
{
    cli();
    for (;;)
    {
    }
}
