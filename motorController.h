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

#define OC_KILL 2500
#define OC_MIN  2750
#define OC_MAX  4000

extern unsigned int OC_ONE,    //Pulse-width on Left-Front motor from 2000 to 4000
                    OC_TWO,    //Pulse-width on Right-Front motor from 2000 to 4000
                    OC_THREE,  //Pulse-width on Right-Back motor from 2000 to 4000
                    OC_FOUR;   //Pulse-width on Left-Back motor from 2000 to 4000

extern unsigned int OC_ONE_PC,
                    OC_TWO_PC,
                    OC_THREE_PC,
                    OC_FOUR_PC;

void setupOutputCompares(void);
void startupMotors(void);
void adjustOCValues(void);
void adjustThrust(void);
void adjustRoll(void);
void adjustPitch(void);
void adjustYaw(void);

#endif
