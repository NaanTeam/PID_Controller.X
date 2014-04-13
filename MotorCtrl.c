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


unsigned int MotorCtrl_OC_ONE = OC_MIN,    //Pulse-width on Left-Front motor from 2000 to 4000
             MotorCtrl_OC_TWO = OC_MIN,    //Pulse-width on Right-Front motor from 2000 to 4000
             MotorCtrl_OC_THREE = OC_MIN,  //Pulse-width on Right-Back motor from 2000 to 4000
             MotorCtrl_OC_FOUR = OC_MIN;   //Pulse-width on Left-Back motor from 2000 to 4000

float   MotorCtrl_OC_ONE_PC,
        MotorCtrl_OC_TWO_PC,
        MotorCtrl_OC_THREE_PC,
        MotorCtrl_OC_FOUR_PC;

unsigned int OC_ADJ = 0;

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

    while (OC1RS < OC_MIN)
    {
        while (ii < 500)
        {
            ii++;
        }

        ii = 0;
        OC1RS++;
        OC2RS++;
        OC3RS++;
        OC4RS++;
    }
}

void MotorCtrl_adjustOCValues(void)
{
    MotorCtrl_adjustThrust();
    //adjustRoll();
    //adjustPitch();
    //adjustYaw();
    MotorCtrl_adjustRollPitchYaw();
}

void MotorCtrl_adjustRollPitchYaw(void)
{
    MotorCtrl_OC_ONE = MotorCtrl_OC_ONE + PID_ROLL + PID_PITCH + PID_YAW;
    MotorCtrl_OC_TWO = MotorCtrl_OC_TWO - PID_ROLL + PID_PITCH - PID_YAW;
    MotorCtrl_OC_THREE = MotorCtrl_OC_THREE - PID_ROLL - PID_PITCH + PID_YAW;
    MotorCtrl_OC_FOUR = MotorCtrl_OC_FOUR + PID_ROLL - PID_PITCH - PID_YAW;

    if (MotorCtrl_OC_ONE > OC_MAX)
        MotorCtrl_OC_ONE = OC_MAX;

    else if (MotorCtrl_OC_ONE < OC_MIN)
        MotorCtrl_OC_ONE = OC_MIN;

    if (MotorCtrl_OC_TWO > OC_MAX)
        MotorCtrl_OC_TWO = OC_MAX;

    else if (MotorCtrl_OC_TWO < OC_MIN)
        MotorCtrl_OC_TWO = OC_MIN;

    if (MotorCtrl_OC_THREE > OC_MAX)
        MotorCtrl_OC_THREE = OC_MAX;

    else if (MotorCtrl_OC_THREE < OC_MIN)
        MotorCtrl_OC_THREE = OC_MIN;

    if (MotorCtrl_OC_FOUR > OC_MAX)
        MotorCtrl_OC_FOUR = OC_MAX;

    else if (MotorCtrl_OC_FOUR < OC_MIN)
        MotorCtrl_OC_FOUR = OC_MIN;
}

/*
For PID output: the output is telling me what is changing in the system, so I
need to compensate the value it gives me. I.e. If it tells me Yaw = 30, I need
to output to my motors a yaw of -30 to correct it.

For the below functions, I am assuming that the output from the PID controller
is a float value ranging from -100 to 100
*/
void MotorCtrl_adjustThrust(void)
{
    unsigned int OC_VAL = 0;

    //Ensures the throttle value is between OC_MIN and OC_MAX
    OC_VAL = IC_THRO * (((float)OC_MAX - (float)OC_MIN) / 100.0) + (float)OC_MIN;

    MotorCtrl_OC_ONE = OC_VAL;
    MotorCtrl_OC_TWO = OC_VAL;
    MotorCtrl_OC_THREE = OC_VAL;
    MotorCtrl_OC_FOUR = OC_VAL;
}

/*void adjustRoll(void)
{
    unsigned int adjustedRoll = OC_PID_SCALE * PID_ROLL;

    //Adjust motor ratio between motors 1/4 and 2/3
    OC_ONE += adjustedRoll;
    OC_TWO -= adjustedRoll;
    OC_THREE -= adjustedRoll;
    OC_FOUR += adjustedRoll;

    
//    If OC_ONE exceeds OC_MAX, then we need to ensure that OC_THREE decreases by
//    an amount equal to the amount OC_MAX was surpassed in order to get the full
//    effect of what the PID values are trying to give to the motors. This math
//    exists for adjustRoll, adjustPitch, and adjustYaw, just for the different
//    motor configurations required for movement in each of those individually.
    
    if (OC_ONE > OC_MAX)
    {
        OC_ADJ = OC_ONE - OC_MAX;

        OC_ONE = OC_MAX;
        OC_THREE -= OC_ADJ;

        if (OC_THREE < OC_MIN)
            OC_THREE = OC_MIN;
    }

    
    //As above, we need to adjust, but this time by the amount that OC_ONE is
    //less than OC_MIN. The math here is the same, just inverted.
    
    else if (OC_ONE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_ONE;

        OC_ONE = OC_MIN;
        OC_THREE += OC_ADJ;

        if (OC_THREE > OC_MAX)
            OC_THREE = OC_MAX;
    }

    if (OC_THREE > OC_MAX)
    {
        OC_ADJ = OC_THREE - OC_MAX;

        OC_THREE = OC_MAX;
        OC_ONE -= OC_ADJ;

        if (OC_ONE < OC_MIN)
            OC_ONE = OC_MIN;
    }

    else if (OC_THREE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_THREE;

        OC_THREE = OC_MIN;
        OC_ONE += OC_ADJ;

        if (OC_ONE > OC_MAX)
            OC_ONE = OC_MAX;
    }

    //We need to check likewise with OC_FOUR and OC_TWO
    if (OC_FOUR > OC_MAX)
    {
        OC_ADJ = OC_FOUR - OC_MAX;

        OC_FOUR = OC_MAX;
        OC_TWO -= OC_ADJ;

        if (OC_TWO < OC_MIN)
            OC_TWO = OC_MIN;
    }

    else if (OC_FOUR < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_FOUR;

        OC_FOUR = OC_MIN;
        OC_TWO += OC_ADJ;

        if (OC_TWO > OC_MAX)
            OC_TWO = OC_MAX;
    }

    if (OC_TWO > OC_MAX)
    {
        OC_ADJ = OC_TWO - OC_MAX;

        OC_TWO = OC_MAX;
        OC_FOUR -= OC_ADJ;

        if (OC_FOUR < OC_MIN)
            OC_FOUR = OC_MIN;
    }

    else if (OC_TWO < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_TWO;

        OC_TWO = OC_MIN;
        OC_FOUR += OC_ADJ;

        if (OC_FOUR > OC_MAX)
            OC_FOUR = OC_MAX;
    }
}

void adjustPitch(void)
{
    unsigned int adjustedPitch = OC_PID_SCALE * PID_PITCH;

    //Adjust motor ratio between motors 1/2 and 3/4
    OC_ONE += adjustedPitch;
    OC_TWO += adjustedPitch;
    OC_THREE -= adjustedPitch;
    OC_FOUR -= adjustedPitch;

    if (OC_ONE > OC_MAX)
    {
        OC_ADJ = OC_ONE - OC_MAX;

        OC_ONE = OC_MAX;
        OC_FOUR -= OC_ADJ;

        if (OC_FOUR < OC_MIN)
            OC_FOUR = OC_MIN;
    }

    else if (OC_ONE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_ONE;

        OC_ONE = OC_MIN;
        OC_FOUR += OC_ADJ;

        if (OC_FOUR > OC_MAX)
            OC_FOUR = OC_MAX;
    }

    if (OC_FOUR > OC_MAX)
    {
        OC_ADJ = OC_FOUR - OC_MAX;

        OC_FOUR = OC_MAX;
        OC_ONE -= OC_ADJ;

        if (OC_ONE < OC_MIN)
            OC_ONE = OC_MIN;
    }

    else if (OC_FOUR < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_FOUR;

        OC_FOUR = OC_MIN;
        OC_ONE += OC_ADJ;

        if (OC_ONE > OC_MAX)
            OC_ONE = OC_MAX;
    }

    if (OC_TWO > OC_MAX)
    {
        OC_ADJ = OC_TWO - OC_MAX;

        OC_TWO = OC_MAX;
        OC_THREE -= OC_ADJ;

        if (OC_THREE < OC_MIN)
            OC_THREE = OC_MIN;
    }

    else if (OC_TWO < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_TWO;

        OC_TWO = OC_MIN;
        OC_THREE += OC_ADJ;

        if (OC_THREE > OC_MAX)
            OC_THREE = OC_MAX;
    }

    if (OC_THREE > OC_MAX)
    {
        OC_ADJ = OC_THREE - OC_MAX;

        OC_THREE = OC_MAX;
        OC_TWO -= OC_ADJ;

        if (OC_TWO < OC_MIN)
            OC_TWO = OC_MIN;
    }

    else if (OC_THREE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_THREE;

        OC_THREE = OC_MIN;
        OC_TWO += OC_ADJ;

        if (OC_TWO > OC_MAX)
            OC_TWO = OC_MAX;
    }
}

void adjustYaw(void)
{
    float OC_PERC;
    unsigned int adjustedYaw = OC_PID_SCALE * PID_YAW;

    //Adjust motor ratio between motors 1/3 and 2/4
    OC_ONE += adjustedYaw;
    OC_TWO -= adjustedYaw;
    OC_THREE += adjustedYaw;
    OC_FOUR -= adjustedYaw;

    if ((OC_ONE > OC_MAX) && (OC_TWO > OC_MAX))
    {
        if (OC_ONE > OC_TWO)
        {
            OC_PERC = (float)OC_TWO / (float)OC_ONE;
            OC_ONE = OC_MAX;
            OC_TWO = (float)OC_MAX * OC_PERC;
        }

        else
        {
            OC_PERC = (float)OC_ONE / (float)OC_TWO;
            OC_TWO = OC_MAX;
            OC_ONE = (float)OC_MAX * OC_PERC;
        }
    }

    if ((OC_THREE > OC_MAX) && (OC_FOUR > OC_MAX))
    {
        if (OC_THREE > OC_FOUR)
        {
            OC_PERC = (float)OC_FOUR / (float)OC_THREE;
            OC_THREE = OC_MAX;
            OC_FOUR = (float)OC_MAX * OC_PERC;
        }

        else
        {
            OC_PERC = (float)OC_THREE / (float)OC_FOUR;
            OC_FOUR = OC_MAX;
            OC_THREE = (float)OC_MAX * OC_PERC;
        }
    }

    if (OC_ONE > OC_MAX)
    {
        OC_ADJ = OC_ONE - OC_MAX;

        OC_ONE = OC_MAX;
        OC_TWO -= OC_ADJ;

        if (OC_TWO < OC_MIN)
            OC_TWO = OC_MIN;

        else if (OC_TWO > OC_MAX)
            OC_TWO = OC_MAX;
    }

    else if (OC_ONE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_ONE;

        OC_ONE = OC_MIN;
        OC_TWO += OC_ADJ;

        if (OC_TWO > OC_MAX)
            OC_TWO = OC_MAX;

        else if (OC_TWO < OC_MIN)
            OC_TWO = OC_MIN;
    }

    if (OC_TWO > OC_MAX)
    {
        OC_ADJ = OC_TWO - OC_MAX;

        OC_TWO = OC_MAX;
        OC_ONE -= OC_ADJ;

        if (OC_ONE < OC_MIN)
            OC_ONE = OC_MIN;

        else if (OC_ONE > OC_MAX)
            OC_ONE = OC_MAX;
    }

    else if (OC_TWO < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_TWO;

        OC_TWO = OC_MIN;
        OC_ONE += OC_ADJ;

        if (OC_ONE > OC_MAX)
            OC_ONE = OC_MAX;

        else if (OC_ONE < OC_MIN)
            OC_ONE = OC_MIN;
    }

    if (OC_THREE > OC_MAX)
    {
        OC_ADJ = OC_THREE - OC_MAX;

        OC_THREE = OC_MAX;
        OC_FOUR -= OC_ADJ;

        if (OC_FOUR < OC_MIN)
            OC_FOUR = OC_MIN;

        else if (OC_FOUR > OC_MAX)
            OC_FOUR = OC_MAX;
    }

    else if (OC_THREE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_THREE;

        OC_THREE = OC_MIN;
        OC_FOUR += OC_ADJ;

        if (OC_FOUR > OC_MAX)
            OC_FOUR = OC_MAX;

        else if (OC_FOUR < OC_MIN)
            OC_FOUR = OC_MIN;
    }

    if (OC_FOUR > OC_MAX)
    {
        OC_ADJ = OC_FOUR - OC_MAX;

        OC_FOUR = OC_MAX;
        OC_THREE -= OC_ADJ;

        if (OC_THREE < OC_MIN)
            OC_THREE = OC_MIN;

        else if (OC_THREE > OC_MAX)
            OC_THREE = OC_MAX;
    }

    else if (OC_FOUR < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_FOUR;

        OC_FOUR = OC_MIN;
        OC_THREE += OC_ADJ;

        if (OC_THREE > OC_MAX)
            OC_THREE = OC_MAX;

        else if (OC_THREE < OC_MIN)
            OC_THREE = OC_MIN;
    }
}
*/