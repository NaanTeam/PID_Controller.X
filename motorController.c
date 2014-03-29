/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: RC Reciever for PID Controller for Senior Design               *
 * Last Update: March 9, 2014                                                  *
 * Description: This file deals with the Motor Controller and all items that   *
 *              deal with it, include Output Compares and adjustment functions *
 *              that correct motor speed based on the PID inputs.              *
 ******************************************************************************/
#include "motorController.h"
#include "RC_Receiver.h"
#include "PID_Algorithm.h"

unsigned int OC_ONE = OC_MIN,    //Pulse-width on Left-Front motor from 2000 to 4000
             OC_TWO = OC_MIN,    //Pulse-width on Right-Front motor from 2000 to 4000
             OC_THREE = OC_MIN,  //Pulse-width on Right-Back motor from 2000 to 4000
             OC_FOUR = OC_MIN;   //Pulse-width on Left-Back motor from 2000 to 4000

unsigned int OC_ONE_PC,
             OC_TWO_PC,
             OC_THREE_PC,
             OC_FOUR_PC;

unsigned int OC_ADJ = 0;

void setupOutputCompares(void)
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

void startupMotors(void)
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

void adjustOCValues(void)
{
    adjustThrust();
    adjustRoll();
    adjustPitch();
    adjustYaw();
}

/*
For PID output: the output is telling me what is changing in the system, so I
need to compensate the value it gives me. I.e. If it tells me Yaw = 30, I need
to output to my motors a yaw of -30 to correct it.

For the below functions, I am assuming that the output from the PID controller
is a float value ranging from -100 to 100
*/
void adjustThrust(void)
{
    unsigned int OC_VAL = 0;

    if (IC_THRO == 0)
    {
        OC_ONE = OC_KILL;
        OC_TWO = OC_KILL;
        OC_THREE = OC_KILL;
        OC_FOUR = OC_KILL;

        return;
    }

    //Ensures the throttle value is between OC_MIN and OC_MAX
    OC_VAL = IC_THRO * (((float)OC_MAX - (float)OC_MIN) / 100.0) + (float)OC_MIN;

    OC_ONE = OC_VAL;
    OC_TWO = OC_VAL;
    OC_THREE = OC_VAL;
    OC_FOUR = OC_VAL;
}

void adjustRoll(void)
{
    //Adjust motor ratio between motors 1/4 and 2/3
    OC_ONE += PID_ROLL;
    OC_TWO -= PID_ROLL;
    OC_THREE -= PID_ROLL;
    OC_FOUR += PID_ROLL;

    /*
    If OC_ONE exceeds OC_MAX, then we need to ensure that OC_THREE decreases by
    an amount equal to the amount OC_MAX was surpassed in order to get the full
    effect of what the PID values are trying to give to the motors. This math
    exists for adjustRoll, adjustPitch, and adjustYaw, just for the different
    motor configurations required for movement in each of those individually.
    */
    if (OC_ONE > OC_MAX)
    {
        OC_ADJ = OC_ONE - OC_MAX;

        OC_ONE = OC_MAX;
        OC_THREE -= OC_ADJ;

        if (OC_THREE < OC_MIN)
            OC_THREE = OC_MIN;
    }

    /*
    As above, we need to adjust, but this time by the amount that OC_ONE is
    less than OC_MIN. The math here is the same, just inverted.
    */
    else if (OC_ONE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_ONE;

        OC_ONE = OC_MIN;
        OC_THREE += OC_ADJ;

        if (OC_THREE > OC_MAX)
            OC_THREE = OC_MAX;
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
}

void adjustPitch(void)
{
    //Adjust motor ratio between motors 1/2 and 3/4
    OC_ONE += PID_PITCH;
    OC_TWO += PID_PITCH;
    OC_THREE -= PID_PITCH;
    OC_FOUR -= PID_PITCH;

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
}

void adjustYaw(void)
{
    //Adjust motor ratio between motors 1/3 and 2/4
    OC_ONE += PID_YAW;
    OC_TWO -= PID_YAW;
    OC_THREE += PID_YAW;
    OC_FOUR -= PID_YAW;

    if (OC_ONE > OC_MAX)
    {
        OC_ADJ = OC_ONE - OC_MAX;

        OC_ONE = OC_MAX;
        OC_TWO -= OC_ADJ;

        if (OC_TWO < OC_MIN)
            OC_TWO = OC_MIN;
    }

    else if (OC_ONE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_ONE;

        OC_ONE = OC_MIN;
        OC_TWO += OC_ADJ;

        if (OC_TWO > OC_MAX)
            OC_TWO = OC_MAX;
    }

    if (OC_THREE > OC_MAX)
    {
        OC_ADJ = OC_THREE - OC_MAX;

        OC_THREE = OC_MAX;
        OC_FOUR -= OC_ADJ;

        if (OC_FOUR < OC_MIN)
            OC_FOUR = OC_MIN;
    }

    else if (OC_THREE < OC_MIN)
    {
        OC_ADJ = OC_MIN - OC_THREE;

        OC_THREE = OC_MIN;
        OC_FOUR += OC_ADJ;

        if (OC_FOUR > OC_MAX)
            OC_FOUR = OC_MAX;
    }
}
