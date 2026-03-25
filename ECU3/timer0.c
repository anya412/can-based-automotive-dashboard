#include <xc.h>
#include "timer0.h"

/***********************************************************
 * Function : init_timer0
 * Description :
 *    Configures Timer 0 for periodic interrupts.
 *
 *    - 8-bit mode
 *    - Internal clock (Fosc / 4)
 *    - Prescaler disabled
 *    - Preload = 6
 *    - Overflow interrupt enabled
 **********************************************************/
void init_timer0(void)
{
    // Select 8-bit Timer 0
    T08BIT = 1;

    // Clock source = internal instruction cycle clock (Fosc/4)
    T0CS = 0;

    // Disable prescaler
    PSA = 1;

    // Preload initial value
    TMR0 = 6;

    // Clear interrupt flag
    TMR0IF = 0;

    // Enable Timer 0 interrupt
    TMR0IE = 1;

    // Start Timer 0
    TMR0ON = 1;
}
