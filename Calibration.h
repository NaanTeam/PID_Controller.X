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
//Device #defines
//******************************************************************************
#define Calibration_AccelX_Top      505
#define Calibration_AccelX_Center   -45
#define Calibration_AccelX_Bottom   -585

#define Calibration_AccelY_Top      560
#define Calibration_AccelY_Center     -5
#define Calibration_AccelY_Bottom   -570

#define Calibration_AccelZ_Top      595
#define Calibration_AccelZ_Center      80
#define Calibration_AccelZ_Bottom   -445


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
void Calibration_offsetAcceleration();

#endif	/* CALIBRATION_H */
