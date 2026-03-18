#include "sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"

uint16_t get_rpm()
{
    //Implement the rpm function
    int rpm;
    rpm = read_adc(CHANNEL4) * 5.8651;
    return rpm;
}

// uint16_t get_engine_temp()
// {
//     Implement the engine temperature function
// }

IndicatorStatus process_indicator()
{
    //Implement the indicator function
    static int indicator = 0x00, key;
    key = read_digital_keypad(STATE_CHANGE);
    if(key != ALL_RELEASED)
    {
        if(key == SWITCH1)
            indicator = e_ind_left;
        else if(key == SWITCH2)
            indicator = e_ind_right;
        else if(key == SWITCH3)
            indicator = e_ind_hazard;
        else if(key == SWITCH4)
            indicator = e_ind_off;

    }
    return indicator;
}
