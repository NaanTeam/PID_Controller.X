/**
 * @file: FIFOUART1.h
 * @brief: A 'First In First Out' interrupt driven UART1 communication method
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

#ifndef FIFOUART1_H
#define	FIFOUART1_H

#ifdef	__cplusplus
extern "C" {
#endif



#include "hardwareProfile.h"
#include <plib.h>


//******************************************************************************
//Configuration #defines
//******************************************************************************
/**Controls UART1's transfer baud rate.*/
#define FIFOUART1_BAUD_RATE 115200
/**Controls FIFOUART1's tx and rx buffer size.*/
#define FIFOUART1_BUFFERSIZE 256


//******************************************************************************
// Public Variables and Typedefs
//******************************************************************************
/**The index of the next empty RX buffer byte.*/
extern UINT16 FIFOUART1_RxBuffer_Index;


//******************************************************************************
// Public Function Declarations
//******************************************************************************
/**
 * Initializes UART1 to be interrupt driven, 8bit data, no parity and
 * one stop bit.
 *
 * @return void
 */
void FIFOUART1_initialize();
/**
 * Queues a byte to be sent on UART1.
 *
 * @param txBytes: The buffer of bytes to be sent.
 * @param length: How long the buffer of bytes is.
 * @return  A 1 on success. A -1 on an overflow.
 */
int FIFOUART1_pushTxQueue(UINT8 txBytes[], int length);
/**
 * Reads a byte from the Queue containing bytes read from UART1.
 *
 * @param *rxByte: The byte which the data will be transfer to.
 * @return A 2 on sucess and non-empty buffer. A 1 on success and empty buffer.
 * A -1 on an overflow. A -2 if the queue is empty.
 */
int FIFOUART1_popRxQueue(UINT8 * rxByte);




#ifdef	__cplusplus
}
#endif

#endif	/* FIFOUART1_H */

