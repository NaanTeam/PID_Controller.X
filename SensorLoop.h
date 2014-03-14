/**
 * @file: SensorLoop.h
 * @brief: A interrupt driven timer loop that queues and interprets
 * data from the different sensors.
 *
 * @author: Connor Martin
 * @date: Oct 23, 2013
 *
 * @preconditions:
 *
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks:
*/

#ifndef SENSORLOOP_H
#define	SENSORLOOP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib.h>

#include "FIFOSPI2.h"
#include "FIFOI2C2.h"

#include "ADXL362.h"
#include "L3G4200D.h"
#include "HMC5883L.h"


//******************************************************************************
//Function Prototypes
//******************************************************************************
/**
 * Sets up the IRQ overflow for Timer1 to run at div 8 and a counter of 0xFFFF.
 */
void SensorLoop_SetupAll();




#ifdef	__cplusplus
}
#endif

#endif	/* TIMER1_LOOP_H */

