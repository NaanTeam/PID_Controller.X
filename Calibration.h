/* 
 * File:   Calibration.h
 * Author: Connor
 *
 * Created on March 22, 2014, 10:26 PM
 */

#ifndef CALIBRATION_H
#define	CALIBRATION_H

#include "L3G4200D.h"
#include "ADXL362.h"
#include "HMC5883L.h"
#include "OrientationLoop.h"
#include "MotorPID.h"


//******************************************************************************
//Public Variables
//******************************************************************************
extern INT16 Calibration_Gyro_Offset_X;
extern INT16 Calibration_Gyro_Offset_Y;
extern INT16 Calibration_Gyro_Offset_Z;

extern float Calibration_Yaw_Zero_Point;

//******************************************************************************
//Function Prototypes
//******************************************************************************
void Calibration_sensorsOffsets();
void Calibration_zeroYaw();

#endif	/* CALIBRATION_H */
