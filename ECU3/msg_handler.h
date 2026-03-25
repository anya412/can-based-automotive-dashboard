#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include <stdint.h>

/*---------------------------------------------------------
 * Indicator LED Control (PORTB Bit Manipulation)
 * 
 *  Left Indicators  : RB0, RB1
 *  Right Indicators : RB6, RB7
 *---------------------------------------------------------*/
#define RIGHT_IND_ON()              (PORTB |=  0xC0)
#define RIGHT_IND_OFF()             (PORTB &= ~0xC0)

#define LEFT_IND_ON()               (PORTB |=  0x03)
#define LEFT_IND_OFF()              (PORTB &= ~0x03)

// Indicator Status Enumeration
typedef enum
{
    e_ind_off = 0,
    e_ind_left,
    e_ind_right,
    e_ind_hazard
} IndicatorStatus;

// Application UI & CAN Message Handlers
void display_labels(void);
void process_canbus_data(void);

void handle_speed_data(uint8_t *data, uint8_t len);
void handle_gear_data(uint8_t *data, uint8_t len);
void handle_rpm_data(uint8_t *data, uint8_t len);
void handle_engine_temp_data(uint8_t *data, uint8_t len);
void handle_indicator_data(uint8_t *data, uint8_t len);

#endif
