/**
* @file: DCM.h
* @brief: Various functions mostly related to DCM oreintation filter theory.
*
* @author: Jordi Munoz and William Premerlani (https://code.google.com/p/ardu-imu/)
* @modifications: Connor Martin.
* @date: Feb 22, 2014
*
* @preconditions:
* @device:
*
* @remarks: Based on work from Mahony and Madgwick.
*  Further credit to Chris Anderson, Doug Weibel, Edwin Babaians , Michel Kogan,
*  and Artem Grigoryev.
*
*/

#ifndef DCM_H
#define	DCM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "hardwareProfile.h"
#include "math.h"
#include "MathFunctions.h"
#include "Orientation.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************

//#define Kp_ROLLPITCH 1.0/GRAVITY
//#define Ki_ROLLPITCH 0.1/GRAVITY
//
//#define Kp_YAW 5.0
//#define Ki_YAW 0.1

//******************************************************************************
//Public Variables
//******************************************************************************

extern float DCM_RollPitch_Kp;
extern float DCM_RollPitch_Ki;
extern float DCM_Yaw_Kp;
extern float DCM_Yaw_Ki;


//******************************************************************************
//Function Prototypes
//******************************************************************************

void DCM_driftCorrection(float* accelVector, float scaledAccelMagnitude, float magneticHeading);
void DCM_matrixUpdate(float timeDiff, float* gyroScaled);
void DCM_normalize(void);
void DCM_eulerAngle(float* roll, float* pitch, float* yaw);




#ifdef	__cplusplus
}
#endif

#endif	/* DCM_H */

