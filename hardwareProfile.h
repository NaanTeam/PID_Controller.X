/*******************************************************************************
 * Programmer: Ryan Moniz and Michael Taylor                                   *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: March 1, 2014                                                  *
 * Description: This program is designed to run the control of the motors for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <p32xxxx.h>

#define SYS_CLK     80000000

#define GetSystemClock()           (SYS_CLK)
#define GetInstructionClock()      (SYS_CLK)
#define GetPeripheralClock()       (SYS_CLK / (1 << OSCCONbits.PBDIV))

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned char boolean;

#endif
