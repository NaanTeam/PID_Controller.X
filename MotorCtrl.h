/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: February 25, 2014                                              *
 * Description: This program is designed to run the control of the motors for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#ifndef MOTORCONTROLLER_H
#define	MOTORCONTROLLER_H

#include "hardwareProfile.h"
#include "RcRx.h"
#include "MotorPID.h"

#define OC_KILL         2500
#define OC_MIN          3000
#define OC_MAX          3250
//#define OC_PID_SCALE    100

extern unsigned int MotorCtrl_OC_ONE,    //Pulse-width on Left-Front motor from 2000 to 4000
                    MotorCtrl_OC_TWO,    //Pulse-width on Right-Front motor from 2000 to 4000
                    MotorCtrl_OC_THREE,  //Pulse-width on Right-Back motor from 2000 to 4000
                    MotorCtrl_OC_FOUR;   //Pulse-width on Left-Back motor from 2000 to 4000

extern  float MotorCtrl_OC_ONE_PC,
              MotorCtrl_OC_TWO_PC,
              MotorCtrl_OC_THREE_PC,
              MotorCtrl_OC_FOUR_PC;

void MotorCtrl_setupOutputCompares(void);
void MotorCtrl_startupMotors(void);
void MotorCtrl_adjustOCValues(void);
void MotorCtrl_adjustThrust(void);
//void adjustRoll(void);
//void adjustPitch(void);
//void adjustYaw(void);
void MotorCtrl_adjustRollPitchYaw(void);
void MotorCtrl_shutOffMotors(void);
void MotorCtrl_idleMotors(void);

#endif
