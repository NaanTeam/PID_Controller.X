/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: March 5, 2014                                                  *
 * Description: This program is designed to run the control of the motors for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#ifndef PID_ALGORITHM_H
#define	PID_ALGORITHM_H

#include "hardwareProfile.h"
#include "OrientationLoop.h"
#include "RcRx.h"
#include "OrientationLoop.h"
#include "Calibration.h"
#include "GUI.h"

#define PID_YAW_ROC             .0016

extern float    SENS_ROLL,      //Roll value given by sensors
                SENS_PITCH,     //Pitch value given by sensors
                SENS_YAW;       //Yaw value given by sensors

extern float    PID_ROLL,       //Roll correction value between -100.0 and 100.0
                PID_PITCH,      //Pitch correction value between -100.0 and 100.0
                PID_YAW;        //Yaw correction value between -100.0 and 100.0

extern float    PID_ROLL_pErr,
                PID_ROLL_iErr,
                PID_ROLL_dErr;


extern float    pRollConst,     //Proportional constant
                iRollConst,     //Intergral constant
                dRollConst;     //Derivative constant



extern float pPitchConst,    //Proportional constant
      iPitchConst,    //Intergral constant
      dPitchConst;    //Derivative constant


extern float    pYawConst,      //Proportional constant
                iYawConst,      //Intergral constant
                dYawConst;      //Derivative constant

extern int yawFlag;

void MotorPID_getSensorValues(void);
void MotorPID_setPIDValues(void);
void MotorPID_setPIDRoll(void);
void MotorPID_setPIDPitch(void);
void MotorPID_setPIDYaw(void);
void MotorPID_resetPID(void);

#endif
