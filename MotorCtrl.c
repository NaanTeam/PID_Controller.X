/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: RC Reciever for PID Controller for Senior Design               *
 * Last Update: April 23, 2014                                                 *
 * Description: This file deals with the Motor Controller and all items that   *
 *              deal with it, include Output Compares and adjustment functions *
 *              that correct motor speed based on the PID inputs.              *
 ******************************************************************************/
#include "MotorCtrl.h"

unsigned int    MotorCtrl_OC_ONE   = OC_KILL;   //Pulse-width on Left-Front motor from 2000 to 4000
unsigned int    MotorCtrl_OC_TWO   = OC_KILL;   //Pulse-width on Right-Front motor from 2000 to 4000
unsigned int    MotorCtrl_OC_THREE = OC_KILL;   //Pulse-width on Right-Back motor from 2000 to 4000
unsigned int    MotorCtrl_OC_FOUR  = OC_KILL;   //Pulse-width on Left-Back motor from 2000 to 4000

float   MotorCtrl_OC_ONE_PC,    //OC_ONE percentage value for GUI
        MotorCtrl_OC_TWO_PC,    //OC_TWO percentage value for GUI
        MotorCtrl_OC_THREE_PC,  //OC_THREE percentage value for GUI
        MotorCtrl_OC_FOUR_PC;   //OC_FOUR percentage value for GUI

int motorsFlag = 0;     //0-motors off, 1-motors on

unsigned int    OC_THRO = 0;    //Proportionalized IC_THRO so it falls in the
                                //Range of OC_MIN and OC_MAX

/*******************************************************************************
 * Function: MotorCtrl_setupOutputCompares                                     *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: Sets OC(1-4)R and RS register values to the predefined OC_KILL *
 *              value, which represents a 1ms pulse (meaning no sping on the   *
 *              motors). Also sets OCM 1 and 2 to run off timer 2 and OCM 3 and*
 *              4 to run off timer 3.                                          *
 ******************************************************************************/
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

/*******************************************************************************
 * Function: MotorCtrl_startupMotors                                           *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Steadily increases the OCxRS values for each motor until it    *
 *              reaches the predefined minimum OCxRS value for each individual *
 *              motor.                                                         *
 ******************************************************************************/
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

        //While each motor is individually under its predefined minimum value,
        //increase its MotorCtrl_OC_X value
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

/*******************************************************************************
 * Function: MotorCtrl_adjustOCValues                                          *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Will call functions to update values needed to change the OCxRS*
 *              values for each motor based on input parameters noted above.   *
 ******************************************************************************/
void MotorCtrl_adjustOCValues(void)
{
    MotorCtrl_adjustThrust();
    MotorCtrl_adjustRollPitchYaw();
}

/*******************************************************************************
 * Function: MotorCtrl_adjustRollPitchYaw                                      *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Using the input parameters noted, each MotorCtrl_OC_X global is*
 *              updated to reflect changes outputted by the PID algorithm. This*
 *              function also ensures each motor does not go below its minimum *
 *              value and does not exceed its maximum value.                   *
 ******************************************************************************/
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

/*******************************************************************************
 * Function: MotorCtrl_adjustThrust                                            *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Takes the IC_THRO global variable and scales it so that it is a*
 *              value between the predefined OC_MIN and OC_MAX values.         *
 ******************************************************************************/
void MotorCtrl_adjustThrust(void)
{
    //Scales the throttle value so it is between OC_MIN and OC_MAX
    OC_THRO = IC_THRO * ((float)(OC_MAX - OC_MIN) / 100.0);
}

/*******************************************************************************
 * Function: MotorCtrl_shutOffMotors                                           *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Sets all OCxRS and MotorCtrl_OC_X values to OC_KILL to stop the*
 *              motors from spinning.                                          *
 ******************************************************************************/
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

/*******************************************************************************
 * Function: MotorCtrl_idleMotors                                              *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Sets all OCxRS and MotorCtrl_OC_X values to OC_X_MIN to keep   *
 *              the motors spinning, but not enough for the quadcopter to take *
 *              off.                                                           *
 ******************************************************************************/
void MotorCtrl_idleMotors(void)
{
    MotorCtrl_OC_ONE   = OC_ONE_MIN;
    MotorCtrl_OC_TWO   = OC_TWO_MIN;
    MotorCtrl_OC_THREE = OC_THREE_MIN;
    MotorCtrl_OC_FOUR  = OC_FOUR_MIN;
}

/*******************************************************************************
 * Function: MotorCtrl_updateOCRSValues                                        *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Sets all OCxRS values to the global MotorCtrl_OC_X values that *
 *              were updated from the PID algorithm. Also saves these values to*
 *              MotorCtrl_OC_X_PC values for use in the GUI.                   *
 ******************************************************************************/
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

/*******************************************************************************
 * Function: MotorCtrl_killMotorsAndWait                                       *
 * Input parameters: void                                                      *
 * Ouptuts/Returns: void                                                       *
 * Description: Sets the motors to OC_KILL and waits for the startup sequence  *
 *              from the RC controllor so it can re-calculate a zero yaw.      *
 ******************************************************************************/
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
