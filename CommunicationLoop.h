/**
* @file: CommunicationLoop.h
* @brief: Functions for monitoring UART1 with timer4.
*
* @author: Connor Martin
* @date: Jan 23, 2014
*
* @preconditions:
* @device:
*      -PIC32MX695F512L
*
* @remarks:
*/

#ifndef COMMUNICATIONLOOP_H
#define	COMMUNICATIONLOOP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib.h>
#include "hardwareProfile.h"
#include "FIFOUART1.h"
#include "MsgInterpreter.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************
/** Controls the max size of a message that can be interpreted.*/
#define COMMUNICATIONLOOP_BUFFERSIZE 256

//******************************************************************************
//Public Variable Declarations
//******************************************************************************
/**
 * Initializes the communication loop (which starts timer4) which monitors UART1
 *  for new messages and passes that information to the Msg interpreter.
 */
void CommunicationLoop_initialize();


#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATIONLOOP_H */

