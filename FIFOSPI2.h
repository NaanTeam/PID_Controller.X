/**
 * @file: FIFOSPI2.h
 * @brief: A 'First In First Out' interrupt driven SPI communication method
 * with this device setup as the master.
 *
 * @author: Connor Martin
 * @date: Nov 26, 2013
 *
 * @preconditions:
 * @device:
 *      -PIC32MX695F512L
 * 
 * @remarks:
 * -Currently only supports 2 SPI devices. Some modifications need in
 * the setup and IRQ needed to support more. (possibly other places as well).
 * 
 * -Second slave select bus line (cs) might have problems. As in it toggles
 *  before the completeion of the tranmission. Or stays on long. Their needs to
 *  be some consistency with it. which thier isn't right now.
 *
 */

#ifndef FIFOSPI2_H
#define	FIFOSPI2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib.h>
#include "hardwareProfile.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************

/**Controls the slave select pin for the first device on SPI-2 line.*/
#define FIFOSPI2_DeviceSSLine1_PortReg PORTEbits.RE0
/**Match this to the corresponding FIFOSPI2_DeviceSSLine1_PortReg.*/
#define FIFOSPI2_DeviceSSLine1_TriState TRISEbits.TRISE0

/**Controls the slave select pin for the second device on SPI-2 line.*/
#define FIFOSPI2_DeviceSSLine2_PortReg PORTEbits.RE1
/**Match this to the corresponding FIFOSPI2_DeviceSSLine2_PortReg.*/
#define FIFOSPI2_DeviceSSLine2_TriState TRISEbits.TRISE1

/**Controls the maximum transmit and receive buffer sizes. Must be greater than 50*/
#define FIFOSPI2_BUFFERSIZE 100



//******************************************************************************
// Public Variables and Typedefs
//******************************************************************************

/**Indicates whether spi-2's IRQ is currently running. 1 it is. 0 it's not.*/
extern UINT8 FIFOSPI2_isRunnning;



//******************************************************************************
// Public Function Declarations
//******************************************************************************

/**
 * Sets up SPI2 in master mode using a FIFO buffer.
 *
 * Operates at PER_FREQ/4 frequency, TX and RX IRQ, Idle Clock polarity and
 * data sampled at end of output time.
 *
 * @return void
 */
void FIFOSPI2_initialize();

/**
 * Adds UINT8 's to the SPI TX buffer.
 *
 * @param data: An array of UINT8 to queue in the TX buffer.
 * @param length: The length of the data array.
 * @param deviceSSLine: The device to write to. Modify #defines for different
 * pins.
 *
 * @return A '1' if the characters where succesfully added. '-1' if the TX
 * buffer is full.
 */
int FIFOSPI2_pushTxQueue(UINT8 data[], int length, int deviceSSLine);

/**
 * Transfers a byte from the SPI RX buffer to a UINT8 .
 *
 * @param readByte: A pointer to a UINT8 variable.
 *
 * @return A '1' if the character was succesfully retrieved.
 * A '0' if the buffer is empty.
 * A '-1' if the receive buffer is overflowing.
 * A '-3' if ti's an unknown error.
*/
int FIFOSPI2_popRxQueue(UINT8 *bytesBuffer);

/**
 * Determines the occupied length of the receive buffer.
 *
 * @return An integer representing the index where the buffer is currently
 * full to.
 */
int FIFOSPI2_rxBufferIndex();


#ifdef	__cplusplus
}
#endif

#endif	/* FIFOSPI2_H */

