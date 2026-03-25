#ifndef CLCD_H
#define CLCD_H

// LCD Port Definitions
#define CLCD_PORT               PORTD
#define CLCD_DATA_DIR           TRISD       // Data direction register

#define CLCD_EN                 RC2
#define CLCD_RS                 RC1
#define CLCD_RW                 RC0

#define CLCD_BUSY               RD7         // Busy flag (D7)

#define HI                      1
#define LO                      0

#define INPUT                   0xFF
#define OUTPUT                  0x00

// LCD Command Modes
#define DATA_COMMAND            1
#define INSTRUCTION_COMMAND     0

// DDRAM Display Addresses
#define LINE1(x)                (0x80 + (x))
#define LINE2(x)                (0xC0 + (x))

#define LCD_CMD_FUNCTION_SET_8BIT_2LINE   0x38
#define LCD_CMD_CLEAR_DISPLAY             0x01
#define LCD_CMD_RETURN_HOME               0x02
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF     0x0C

/* Reset sequence value used during power-on initialization */
#define LCD_RESET_SEQ                     0x33

// High-Level Command Helper Macros
#define LCD_CMD_FUNCTION_SET()        clcd_write(LCD_CMD_FUNCTION_SET_8BIT_2LINE, INSTRUCTION_COMMAND)
#define LCD_CMD_CLEAR()               clcd_write(LCD_CMD_CLEAR_DISPLAY, INSTRUCTION_COMMAND)
#define LCD_CMD_CURSOR_HOME()         clcd_write(LCD_CMD_RETURN_HOME, INSTRUCTION_COMMAND)
#define LCD_CMD_DISPLAY_ON()          clcd_write(LCD_CMD_DISPLAY_ON_CURSOR_OFF, INSTRUCTION_COMMAND)

// Clock Frequency for Delays
#define _XTAL_FREQ                    20000000UL

void init_clcd(void);
void clcd_write(unsigned char byte, unsigned char control_bit);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_clear(void);

#endif
