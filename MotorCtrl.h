/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: April 19, 2014                                                 *
 * Description: This program is designed to run the control of the motors for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#ifndef MOTORCONTROLLER_H
#define	MOTORCONTROLLER_H

#include "hardwareProfile.h"
#include "RcRx.h"
#include "MotorPID.h"

#define OC_KILL         2500    //Absolute min (1ms pulse)
#define OC_MIN          3000    //Idle
#define OC_MAX          3250    //Absolute max 5000 (2ms pulse)

#define OC_ONE_OFFSET   0
#define OC_TWO_OFFSET   8
#define OC_THREE_OFFSET 8
#define OC_FOUR_OFFSET  0

#define OC_ONE_MIN      (OC_MIN + OC_ONE_OFFSET)
#define OC_TWO_MIN      (OC_MIN + OC_TWO_OFFSET)
#define OC_THREE_MIN    (OC_MIN + OC_THREE_OFFSET)
#define OC_FOUR_MIN     (OC_MIN + OC_FOUR_OFFSET)

#define OC_ONE_MAX      (OC_MAX + OC_ONE_OFFSET)
#define OC_TWO_MAX      (OC_MAX + OC_TWO_OFFSET)
#define OC_THREE_MAX    (OC_MAX + OC_THREE_OFFSET)
#define OC_FOUR_MAX     (OC_MAX + OC_FOUR_OFFSET)

extern unsigned int MotorCtrl_OC_ONE,   //Pulse-width on Left-Front motor from 2500 to 5000
                    MotorCtrl_OC_TWO,   //Pulse-width on Right-Front motor from 2500 to 5000
                    MotorCtrl_OC_THREE, //Pulse-width on Right-Back motor from 2500 to 5000
                    MotorCtrl_OC_FOUR;  //Pulse-width on Left-Back motor from 2500 to 5000

extern  float   MotorCtrl_OC_ONE_PC,    //Percentage of Motor One value for GUI
                MotorCtrl_OC_TWO_PC,    //Percentage of Motor Two value for GUI
                MotorCtrl_OC_THREE_PC,  //Percentage of Motor Three value for GUI
                MotorCtrl_OC_FOUR_PC;   //Percentage of Motor Four value for GUI

extern int  motorsFlag;

void MotorCtrl_setupOutputCompares(void);
void MotorCtrl_startupMotors(void);
void MotorCtrl_adjustOCValues(void);
void MotorCtrl_adjustThrust(void);
void MotorCtrl_adjustRollPitchYaw(void);
void MotorCtrl_shutOffMotors(void);
void MotorCtrl_idleMotors(void);
void MotorCtrl_updateOCRSValues(void);
void MotorCtrl_killMotorsAndWait(void);

#endif
