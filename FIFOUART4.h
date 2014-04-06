/**
 * @file: FIFOUART4.h
 * @brief: A 'First In First Out' interrupt driven UART4 communication method
 *
 * @author: Connor Martin
 * @date: Jan 23, 2014
 *
 * @preconditions:
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks:
 *
 */

#ifndef FIFOUART4_H
#define	FIFOUART4_H

#ifdef	__cplusplus
extern "C" {
#endif



#include "hardwareProfile.h"
#include <plib.h>


//******************************************************************************
//Configuration #defines
//******************************************************************************
/**Controls UART4's transfer baud rate.*/
#define FIFOUART4_BAUD_RATE 115200
/**Controls FIFOUART4's tx and rx buffer size.*/
#define FIFOUART4_BUFFERSIZE 256


//******************************************************************************
// Public Variables and Typedefs
//******************************************************************************
/**The index of the next empty RX buffer byte.*/
extern UINT16 FIFOUART4_RxBuffer_Index;


//******************************************************************************
// Public Function Declarations
//******************************************************************************
/**
 * Initializes UART4 to be interrupt driven, 8bit data, no parity and
 * one stop bit.
 *
 * @return void
 */
void FIFOUART4_initialize();
/**
 * Queues a byte to be sent on UART4.
 *
 * @param txBytes: The buffer of bytes to be sent.
 * @param length: How long the buffer of bytes is.
 * @return  A 1 on success. A -1 on an overflow.
 */
int FIFOUART4_pushTxQueue(UINT8 txBytes[], int length);
/**
 * Reads a byte from the Queue containing bytes read from UART4.
 *
 * @param *rxByte: The byte which the data will be transfer to.
 * @return A 2 on sucess and non-empty buffer. A 1 on success and empty buffer.
 * A -1 on an overflow. A -2 if the queue is empty.
 */
int FIFOUART4_popRxQueue(UINT8 * rxByte);




#ifdef	__cplusplus
}
#endif

#endif	/* FIFOUART4_H */

