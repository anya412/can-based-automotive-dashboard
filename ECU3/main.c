/***********************************************************************
 *  File name   : main.c
 *  Description : Initializes system modules and continuously processes
 *                CAN messages for display and indicator control.
 ***********************************************************************/

#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "msg_handler.h"
#include "timer0.h"

/*---------------------------------------------------------
 * Initialize LED pins
 *  RB2 → Output (Right indicator)
 *  RB3 → Input  (CAN RX)
 *  Others → Output
 *---------------------------------------------------------*/
static void init_leds(void)
{
    TRISB = 0x08;   /* RB3 = 1 (input), others = output */
    PORTB = 0x00;   /* Start with all LEDs off */
}

/*---------------------------------------------------------
 * Initialize all system-level modules:
 *  - LCD
 *  - CAN peripheral
 *  - LED GPIOs
 *  - Timer0
 *  - Interrupt control
 *---------------------------------------------------------*/
static void init_system(void)
{
    init_clcd();
    init_can();
    init_leds();
    init_timer0();

    // Enable global + peripheral interrupts
    PEIE = 1;
    GIE  = 1;
}

void main(void)
{
    init_system();

    // Display static labels once
    display_labels();

    while (1)
    {
        // Read and process CAN data continuously
        process_canbus_data();
    }
}
