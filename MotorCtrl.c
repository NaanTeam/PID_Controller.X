/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: RC Reciever for PID Controller for Senior Design               *
 * Last Update: April 19, 2014                                                 *
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

    //Set output compare R/RS values to OC_KILL, representing 1ms pulses
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

    //Continue to increase individual motor speed until it reaches the MIN value
    //for each individual motor
    while ((MotorCtrl_OC_ONE < OC_ONE_MIN) || (MotorCtrl_OC_TWO < OC_TWO_MIN) \
      || (MotorCtrl_OC_THREE < OC_THREE_MIN) || (MotorCtrl_OC_FOUR < OC_FOUR_MIN))
    {
        //Slight delay to gradually increase motors
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

        //Save OC_X values to OCxRS registers
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
    //Adjust Roll, Pitch, and Yaw for each motor separately
    MotorCtrl_OC_ONE   = OC_ONE_MIN + OC_THRO + PID_ROLL - PID_PITCH - PID_YAW;
    MotorCtrl_OC_TWO   = OC_TWO_MIN + OC_THRO - PID_ROLL - PID_PITCH + PID_YAW;
    MotorCtrl_OC_THREE = OC_THREE_MIN + OC_THRO - PID_ROLL + PID_PITCH - PID_YAW;
    MotorCtrl_OC_FOUR  = OC_FOUR_MIN + OC_THRO + PID_ROLL + PID_PITCH + PID_YAW;

    //Ensure motor one stays within its MAX and MIN values
    if (MotorCtrl_OC_ONE > OC_ONE_MAX)
        MotorCtrl_OC_ONE = OC_ONE_MAX;

    else if (MotorCtrl_OC_ONE < OC_ONE_MIN)
        MotorCtrl_OC_ONE = OC_ONE_MIN;

    //Ensure motor two stays within its MAX and MIN values
    if (MotorCtrl_OC_TWO > OC_TWO_MAX)
        MotorCtrl_OC_TWO = OC_TWO_MAX;

    else if (MotorCtrl_OC_TWO < OC_TWO_MIN)
        MotorCtrl_OC_TWO = OC_TWO_MIN;

    //Ensure motor three stays within its MAX and MIN values
    if (MotorCtrl_OC_THREE > OC_THREE_MAX)
        MotorCtrl_OC_THREE = OC_THREE_MAX;

    else if (MotorCtrl_OC_THREE < OC_THREE_MIN)
        MotorCtrl_OC_THREE = OC_THREE_MIN;

    //Ensure motor four stays within its MAX and MIN values
    if (MotorCtrl_OC_FOUR > OC_FOUR_MAX)
        MotorCtrl_OC_FOUR = OC_FOUR_MAX;

    else if (MotorCtrl_OC_FOUR < OC_FOUR_MIN)
        MotorCtrl_OC_FOUR = OC_FOUR_MIN;
}

void MotorCtrl_adjustThrust(void)
{
    //Scales the throttle value so it is between OC_MIN and OC_MAX
    OC_THRO = IC_THRO * ((float)(OC_MAX - OC_MIN) / 100.0);
}

void MotorCtrl_shutOffMotors(void)
{
    OC1RS = OC_KILL;
    OC2RS = OC_KILL;
    OC3RS = OC_KILL;
    OC4RS = OC_KILL;

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

void MotorCtrl_updateOCRSValues(void)
{
    //Update OCxRS registers with newly calculated values
    OC1RS = MotorCtrl_OC_ONE;     //Front-Left Motor
    OC2RS = MotorCtrl_OC_TWO;     //Front-Right Motor
    OC3RS = MotorCtrl_OC_THREE;   //Back-Right Motor
    OC4RS = MotorCtrl_OC_FOUR;    //Back-Left Motor

    //PC values to be used for visual purposes on the GUI
    MotorCtrl_OC_ONE_PC = MotorCtrl_OC_ONE;
    MotorCtrl_OC_TWO_PC = MotorCtrl_OC_TWO;
    MotorCtrl_OC_THREE_PC = MotorCtrl_OC_THREE;
    MotorCtrl_OC_FOUR_PC = MotorCtrl_OC_FOUR;
}

void MotorCtrl_killMotorsAndWait(void)
{
    //Stop all motor control
    motorsFlag = 0;

    //Sets OC_X and OCxRS to OC_KILL
    MotorCtrl_shutOffMotors();

    //Also stop Timer 5, which controls the PID algorithm
    T5CONCLR = 0x8000;

    //Wait till startup sequence has been achieved
    RcRx_enableProps();

    //Restart Timer 5
    T5CONSET = 0x8000;

    //Re-calculate zero yaw point
    yawFlag = 0;
    MotorPID_determineZeroYaw();

    //Restart motors
    MotorCtrl_startupMotors();
}
