/***********************************************************************
 *  File name   : can.h
 *  Description : Header file for CAN driver.
 *                Contains macro definitions for ECAN operation modes,
 *                FIFO status helpers, and CAN API prototypes.
 ***********************************************************************/

#ifndef CAN_H
#define CAN_H
#include <stdint.h>

/*---------------------------------------------------------
 *  CAN Operation Mode Values
 *---------------------------------------------------------*/
#define CAN_OPMODE_NORMAL   0x00
#define CAN_OPMODE_LOOP     0x40
#define CAN_OPMODE_CONFIG   0x80

/*---------------------------------------------------------
 *  ECAN FIFO status flags
 *  Note: ECAN_FIFO2_FULL to ECAN_FIFO7_FULL are only valid
 *        in ECAN Mode 1 or Mode 2. Do not use in Legacy Mode 0.
 *---------------------------------------------------------*/
#define ECAN_MODE_BITS      (CANCON & 0x0F)
#define ECAN_FIFO0_FULL     RXB0CONbits.RXFUL
#define ECAN_FIFO1_FULL     RXB1CONbits.RXFUL
#define ECAN_FIFO2_FULL     B0CONbits.RXFUL
#define ECAN_FIFO3_FULL     B1CONbits.RXFUL
#define ECAN_FIFO4_FULL     B2CONbits.RXFUL
#define ECAN_FIFO5_FULL     B3CONbits.RXFUL
#define ECAN_FIFO6_FULL     B4CONbits.RXFUL
#define ECAN_FIFO7_FULL     B5CONbits.RXFUL

/*---------------------------------------------------------
 *  Set CAN mode (no wait)
 *---------------------------------------------------------*/
#define CAN_SET_OPERATION_MODE_NO_WAIT(mode)   \
do {                                           \
    CANCON &= 0x1F;     /* clear old mode */   \
    CANCON |= (mode);   /* set new mode */     \
} while(0)

/*---------------------------------------------------------
 *  Function Prototypes
 *---------------------------------------------------------*/
/* Initialize the CAN peripheral */
void init_can(void);
/* Send a CAN message */
void can_transmit(uint16_t msg_id,
                  const uint8_t *data,
                  uint8_t len);
/* Receive CAN message (len = 0 if none) */
void can_receive(uint16_t *msg_id,
                 uint8_t *data,
                 uint8_t *len);

#endif /* CAN_H */
