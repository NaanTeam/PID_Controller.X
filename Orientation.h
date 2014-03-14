/**
* @file: ADXL362.h
* @brief: Functions and loop for interfacing with the DCM filter.
*
* @author: Connor Martin
* @date: FEB 13, 2013
*
* @preconditions:
*  -FIFOSPI2 is intialized.
* @device:
*      -PIC32MX695F512L
*
* @remarks:
*
*/

#ifndef ORIENTATION_H
#define	ORIENTATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "DCM.h"
#include "Math.h"
#include "MathFunctions.h"
#include "HMC5883L.h"
#include "L3G4200D.h"
#include "ADXL362.h"


//******************************************************************************
//Configuration #defines
//******************************************************************************
#define GRAVITY 1180 // This equivalent to 1G in the raw data coming from the accelerometer

//L3G4200D Sensitivity at 250dps
#define Gyro_Gain_X 17.5e-3
#define Gyro_Gain_Y 17.5e-3
#define Gyro_Gain_Z 17.5e-3
#define Gyro_Scaled_X(x) x*ToRad(Gyro_Gain_X) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) x*ToRad(Gyro_Gain_Y) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) x*ToRad(Gyro_Gain_Z) //Return the scaled ADC raw data of the gyro in radians for second


#define XAccel_Offeset 0
#define YAccel_Offeset 0
#define ZAccel_Offeset 0

#define XGyro_Offeset 53
#define YGyro_Offeset 29
#define ZGyroc_Offeset 17

#define MAGNETIC_DECLINATION 0
#define XMagnetic_Offeset 0
#define YMagnetic_Offeset 0
#define ZMagnetic_Offeset 0


//******************************************************************************
//Public Variables
//******************************************************************************
extern float Orientation_Roll;
extern float Orientation_Pitch;
extern float Orientation_Yaw;



//******************************************************************************
//Function Prototypes
//******************************************************************************

void Orientation_Update();

void Orientation_setup();



#ifdef	__cplusplus
}
#endif

#endif	/* ORIENTATION_H */

