#include <xc.h>

// External timing counter
extern unsigned long int timer_count;

// Timer 0 ISR
void __interrupt() isr(void)
{
    // Timer 0 overflow interrupt
    if (TMR0IF)
    {
        // Reload value
        TMR0 = TMR0 + 9;
        
        // 20,000 ticks rollover
        if (timer_count++ == 20000)
        {
            timer_count = 0;
        }

        // Clear interrupt flag
        TMR0IF = 0;                         
    }
}
