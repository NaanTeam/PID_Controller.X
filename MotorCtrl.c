/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: RC Reciever for PID Controller for Senior Design               *
 * Last Update: March 9, 2014                                                  *
 * Description: This file deals with the Motor Controller and all items that   *
 *              deal with it, include Output Compares and adjustment functions *
 *              that correct motor speed based on the PID inputs.              *
 ******************************************************************************/
#include "MotorCtrl.h"

unsigned int    MotorCtrl_OC_ONE   = OC_KILL;    //Pulse-width on Left-Front motor from 2000 to 4000
unsigned int    MotorCtrl_OC_TWO   = OC_KILL;    //Pulse-width on Right-Front motor from 2000 to 4000
unsigned int    MotorCtrl_OC_THREE = OC_KILL;  //Pulse-width on Right-Back motor from 2000 to 4000
unsigned int    MotorCtrl_OC_FOUR  = OC_KILL;   //Pulse-width on Left-Back motor from 2000 to 4000

float   MotorCtrl_OC_ONE_PC,
        MotorCtrl_OC_TWO_PC,
        MotorCtrl_OC_THREE_PC,
        MotorCtrl_OC_FOUR_PC;

int motorsFlag = 0;     //0-motors off, 1-motors on

unsigned int    OC_THRO = 0;

void MotorCtrl_setupOutputCompares(void)
{
    //Clear all output compare modules
    OC1CONCLR = 0xFFFF;
    OC2CONCLR = 0xFFFF;
    OC3CONCLR = 0xFFFF;
    OC4CONCLR = 0xFFFF;

    //Set output compare R/RS values to 2000, representing 1ms pulses, which
    //corresponds to 0 throttle and initializes the motors
    OC1R = OC_KILL;
    OC1RS = OC_KILL;
    OC2R = OC_KILL;
    OC2RS = OC_KILL;
    OC3R = OC_KILL;
    OC3RS = OC_KILL;
    OC4R = OC_KILL;
    OC4RS = OC_KILL;

    //enable output compares, two per timer
    OC1CONSET = 0x8006;
    OC2CONSET = 0x8006;
    OC3CONSET = 0x800E;
    OC4CONSET = 0x800E;
}

void MotorCtrl_startupMotors(void)
{
    int ii = 0;

    while ((MotorCtrl_OC_ONE < OC_ONE_MIN) || (MotorCtrl_OC_TWO < OC_TWO_MIN) \
      || (MotorCtrl_OC_THREE < OC_THREE_MIN) || (MotorCtrl_OC_FOUR < OC_FOUR_MIN))
    {
        while (ii < 500)
        {
            ii++;
        }
        ii = 0;
        
        if (MotorCtrl_OC_ONE < (OC_ONE_MIN))
            MotorCtrl_OC_ONE++;

        if (MotorCtrl_OC_TWO < (OC_TWO_MIN))
            MotorCtrl_OC_TWO++;

        if (MotorCtrl_OC_THREE < (OC_THREE_MIN))
            MotorCtrl_OC_THREE++;

        if (MotorCtrl_OC_FOUR < (OC_FOUR_MIN))
            MotorCtrl_OC_FOUR++;

        OC1RS = MotorCtrl_OC_ONE;
        OC2RS = MotorCtrl_OC_TWO;
        OC3RS = MotorCtrl_OC_THREE;
        OC4RS = MotorCtrl_OC_FOUR;
    }
    motorsFlag = 1;
}

void MotorCtrl_adjustOCValues(void)
{
    MotorCtrl_adjustThrust();
    MotorCtrl_adjustRollPitchYaw();
}

void MotorCtrl_adjustRollPitchYaw(void)
{
    MotorCtrl_OC_ONE   = OC_ONE_MIN + OC_THRO - PID_ROLL - PID_PITCH - PID_YAW;
    MotorCtrl_OC_TWO   = OC_TWO_MIN + OC_THRO + PID_ROLL - PID_PITCH + PID_YAW;
    MotorCtrl_OC_THREE = OC_THREE_MIN + OC_THRO + PID_ROLL + PID_PITCH - PID_YAW;
    MotorCtrl_OC_FOUR  = OC_FOUR_MIN + OC_THRO - PID_ROLL + PID_PITCH + PID_YAW;

    if (MotorCtrl_OC_ONE > OC_ONE_MAX)
        MotorCtrl_OC_ONE = OC_ONE_MAX;

    else if (MotorCtrl_OC_ONE < OC_ONE_MIN)
        MotorCtrl_OC_ONE = OC_ONE_MIN;

    if (MotorCtrl_OC_TWO > OC_TWO_MAX)
        MotorCtrl_OC_TWO = OC_TWO_MAX;

    else if (MotorCtrl_OC_TWO < OC_TWO_MIN)
        MotorCtrl_OC_TWO = OC_TWO_MIN;

    if (MotorCtrl_OC_THREE > OC_THREE_MAX)
        MotorCtrl_OC_THREE = OC_THREE_MAX;

    else if (MotorCtrl_OC_THREE < OC_THREE_MIN)
        MotorCtrl_OC_THREE = OC_THREE_MIN;

    if (MotorCtrl_OC_FOUR > OC_FOUR_MAX)
        MotorCtrl_OC_FOUR = OC_FOUR_MAX;

    else if (MotorCtrl_OC_FOUR < OC_FOUR_MIN)
        MotorCtrl_OC_FOUR = OC_FOUR_MIN;
}

void MotorCtrl_adjustThrust(void)
{
    //Scales the throttle value so it is between OC_MIN and OC_MAX
    //OC_THRO = (IC_THRO + GUI_THROTTLE) * (((float)OC_MAX - (float)OC_MIN) / 100.0) + (float)OC_MIN;
    OC_THRO = IC_THRO * ((float)(OC_MAX - OC_MIN) / 100.0);
}

void MotorCtrl_shutOffMotors(void)
{
    MotorCtrl_OC_ONE   = OC_KILL;
    MotorCtrl_OC_TWO   = OC_KILL;
    MotorCtrl_OC_THREE = OC_KILL;
    MotorCtrl_OC_FOUR  = OC_KILL;
}

void MotorCtrl_idleMotors(void)
{
    MotorCtrl_OC_ONE   = OC_ONE_MIN;
    MotorCtrl_OC_TWO   = OC_TWO_MIN;
    MotorCtrl_OC_THREE = OC_THREE_MIN;
    MotorCtrl_OC_FOUR  = OC_FOUR_MIN;
}
