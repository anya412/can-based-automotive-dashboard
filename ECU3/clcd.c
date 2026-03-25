/***********************************************************************
 *  File name   : clcd.c
 *  Description : Driver for Character LCD (CLCD) in 8-bit mode.
 *                Handles instruction/data writes, initialization,
 *                character output, string output, and display clear.
 *
 *                Functions:
 *                - clcd_write()
 *                - init_clcd()
 *                - clcd_print()
 *                - clcd_putch()
 *                - clcd_clear()
 ***********************************************************************/

#include <xc.h>
#include "clcd.h"

// TRIS D for data direction
#define CLCD_DATA_DIR   TRISD
#define CLCD_CTRL_DIR   TRISC

/*----------------------------------------------------------------------
 *  Function : clcd_write
 *  Description :
 *      Sends one byte (command or data) to the CLCD controller.
 *      Automatically waits for the busy flag before returning.
 *
 *      value       → Byte to send (command/data)
 *      control_bit → INSTRUCTION_COMMAND or DATA_COMMAND
 *----------------------------------------------------------------------*/
void clcd_write(unsigned char value, unsigned char control_bit)
{
    // Select Command/Data register
    CLCD_RS = control_bit;

    // Place value on Port D
    CLCD_PORT = value;

    // Enable strobe (min 200ns)
    CLCD_EN = HI;
    CLCD_EN = LO;

    /* Configure for reading busy flag */
    CLCD_DATA_DIR = INPUT;         // PortD = input
    CLCD_RW       = HI;            // Read mode
    CLCD_RS       = INSTRUCTION_COMMAND;

    // Poll busy flag
    do
    {
        CLCD_EN = HI;
        CLCD_EN = LO;
    }
    while (CLCD_BUSY);

    // Restore to write mode
    CLCD_RW       = LO;
    CLCD_DATA_DIR = OUTPUT;
}

/*----------------------------------------------------------------------
 *  Function : init_clcd
 *  Description :
 *      Initializes CLCD controller.
 *      Executes reset sequence, function set, display control,
 *      and clearing operations.
 *----------------------------------------------------------------------*/
void init_clcd(void)
{
    // Configure ports
    CLCD_DATA_DIR = OUTPUT;      // Port D = data output
    CLCD_CTRL_DIR &= 0xF8;       // RC2 (EN), RC1 (RS), RC0 (RW) as output

    CLCD_RW = LO;

    /* Power-on delay (minimum 15ms required) */
    __delay_ms(30);

    // Reset sequence for LCD
    clcd_write(LCD_RESET_SEQ, INSTRUCTION_COMMAND);
    __delay_us(4100);

    clcd_write(LCD_RESET_SEQ, INSTRUCTION_COMMAND);
    __delay_us(100);

    clcd_write(LCD_RESET_SEQ, INSTRUCTION_COMMAND);
    __delay_us(1);

    /* Function set: 8-bit, 2 line, 5x8 font */
    LCD_CMD_FUNCTION_SET();
    __delay_us(100);

    // Clear display
    LCD_CMD_CLEAR();
    __delay_us(500);

    // Display ON, Cursor OFF
    LCD_CMD_DISPLAY_ON();
    __delay_us(100);

    // Cursor home
    LCD_CMD_CURSOR_HOME();
    __delay_us(100);
}

/*----------------------------------------------------------------------
 *  Function : clcd_print
 *  Description :
 *      Prints a null-terminated string to the CLCD.
 *
 *      str  → pointer to string
 *      addr → display address (LINE1(x), LINE2(x))
 *----------------------------------------------------------------------*/
void clcd_print(const unsigned char *str, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);

    while (*str != '\0')
    {
        clcd_write(*str++, DATA_COMMAND);
    }
}

/*----------------------------------------------------------------------
 *  Function : clcd_putch
 *  Description :
 *      Prints a single character at given LCD address.
 *
 *      ch   → character to display
 *      addr → display address
 *----------------------------------------------------------------------*/
void clcd_putch(const unsigned char ch, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);
    clcd_write(ch,  DATA_COMMAND);
}

/*----------------------------------------------------------------------
 *  Function : clcd_clear
 *  Description :
 *      Clears screen and returns cursor to home position.
 *----------------------------------------------------------------------*/
void clcd_clear(void)
{
    LCD_CMD_CLEAR();
}
