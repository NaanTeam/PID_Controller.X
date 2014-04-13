/*
 * File:   RC_Reciever.h
 * Author: Mike
 *
 * Created on February 25, 2014, 2:02 PM
 */

#ifndef RC_RECIEVER_H
#define	RC_RECIEVER_H

#include "hardwareProfile.h"

#define PER_REG     55000
#define MAX_PER     20000       //Used for Timer5 to control the PID Algorithm

//Shared variables
extern float IC_THRO,               //Throttle percentage 0.0 thru 100.0%
             IC_YAW,                //Yaw angle -180.0 thru 180.0 degrees
             IC_PITCH,              //Pitch angle -90.0 thru 90.0 degrees
             IC_ROLL;               //Roll angle -90.0 thru 90.0 degrees

extern unsigned int IC_AUX;         //Aux switch 0 or 1
extern unsigned int PROPS_ENABLED;  //Props enabled flag 0 or 1

//Function prototypes
void RcRx_setupInputCaptures(void);
void RcRx_zeroController(void);
void RcRx_enableProps(void);
int RcRx_disableProps(void);

#endif	/* RC_RECIEVER_H */
