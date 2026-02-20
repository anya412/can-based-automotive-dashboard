/***********************************************************************
 *  File name   : can.c
 *  Description : ECAN driver for PIC18 microcontroller.
 *                Provides initialization, message transmit and receive
 *                interfaces using Standard Identifier (11-bit).
 *
 *  API:
 *      - init_can()
 *      - can_transmit()
 *      - can_receive()
 *
 ***********************************************************************/

#include <xc.h>
#include <stdint.h>
#include "can.h"

/*---------------------------------------------------------
 *  Local Helper : Read Standard ID from RX Buffer 0
 *---------------------------------------------------------*/
static uint16_t can_read_standard_id(void)
{
    uint16_t std_id = 0;

    /* Extract bits from SIDH and SIDL */
    std_id = ((RXB0SIDL >> 5) & 0x07) | (RXB0SIDH << 3);

    return std_id;
}

/*---------------------------------------------------------
 *  Local Helper : Write Standard ID into TX Buffer 0
 *---------------------------------------------------------*/
static void can_write_standard_id(uint16_t msg_id)
{
    TXB0SIDL = (msg_id & 0x07) << 5;
    TXB0SIDH = (msg_id >> 3);
}

/*---------------------------------------------------------
 *  Function : init_can
 *  Description :
 *      Initializes the ECAN peripheral.
 *      Sets CAN TX/RX pins, config mode, timing parameters,
 *      receive filters, and enters normal mode.
 *---------------------------------------------------------*/
void init_can(void)
{
    /* CAN_TX = RB2 (output), CAN_RX = RB3 (input) */
    TRISB2 = 0;
    TRISB3 = 1;

    /* Enter configuration mode */
    CAN_SET_OPERATION_MODE_NO_WAIT(CAN_OPMODE_CONFIG);

    /* Wait until ECAN enters config mode */
    while (CANSTAT != CAN_OPMODE_CONFIG);

    /* Select ECAN Legacy Mode */
    ECANCON = 0x00;

    /* Bit Timing for 8 MHz oscillator */
    BRGCON1 = 0xE1;   /* SJW = 4 TQ, BRP = 4 */
    BRGCON2 = 0x1B;   /* PS1 = 4 TQ, Propagation = 4 TQ */
    BRGCON3 = 0x03;   /* PS2 = 4 TQ */

    /* Enable Filter Control (Filter 0 enabled) */
    RXFCON0 = 0x00;

    /* Enter Normal Mode */
    CAN_SET_OPERATION_MODE_NO_WAIT(CAN_OPMODE_NORMAL);

    /* Receive all valid messages */
    RXB0CON = 0x00;
    RXB0CONbits.RXM0 = 1;     /* Accept all messages */
    RXB0CONbits.RXM1 = 1;
}

/*---------------------------------------------------------
 *  Function : can_transmit
 *  Description :
 *      Sends a CAN message using TX Buffer 0.
 *
 *      Parameters:
 *          msg_id  - Standard Identifier (11-bit)
 *          data    - Pointer to payload data bytes
 *          len     - Number of data bytes (0–8)
 *---------------------------------------------------------*/
void can_transmit(uint16_t msg_id, const uint8_t *data, uint8_t len)
{
    uint8_t *tx_buffer;

    /* Extended ID disabled */
    TXB0EIDH = 0x00;
    TXB0EIDL = 0x00;

    /* Load Standard ID */
    can_write_standard_id(msg_id);

    /* Set Data Length Code */
    TXB0DLC = len;

    /* Load payload bytes */
    tx_buffer = (uint8_t *)&TXB0D0;

    for (uint8_t i = 0; i < len; i++)
    {
        tx_buffer[i] = data[i];
    }

    /* Request message transmission */
    TXB0REQ = 1;
}

/*---------------------------------------------------------
 *  Function : can_receive
 *  Description :
 *      Reads a message from RX Buffer 0.
 *
 *      Parameters:
 *          msg_id  - pointer to store received Standard ID
 *          data    - pointer to store payload bytes
 *          len     - pointer to data length
 *
 *      Note:
 *          If no message is available:
 *              *len = 0
 *---------------------------------------------------------*/
void can_receive(uint16_t *msg_id, uint8_t *data, uint8_t *len)
{
    uint8_t *rx_buffer;

    /* Check if RX buffer has data */
    if (RXB0FUL)
    {
        /* Read Identifier */
        *msg_id = can_read_standard_id();

        /* Read Data Length */
        *len = RXB0DLC;

        /* Read Payload */
        rx_buffer = (uint8_t *)&RXB0D0;

        for (uint8_t i = 0; i < *len; i++)
        {
            data[i] = rx_buffer[i];
        }

        /* Clear buffer flags */
        RXB0FUL = 0;
        RXB0IF  = 0;

        return;
    }

    /* No message received */
    *len = 0;
}
