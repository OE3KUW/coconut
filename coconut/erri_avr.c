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
    for (;;) {
    }
}
