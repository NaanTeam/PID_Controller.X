/*******************************************************************************
 * Programmer: Ryan Moniz                                                      *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: April 23, 2014                                                 *
 * Description: This files deals with the PID algorithm itself. Roll, pitch,   *
 *              and yaw values are handled separately and then combined into   *
 *              one calculation per motor to adjust based on RC and SENS values*
 ******************************************************************************/
#include "MotorPID.h"

float SENS_ROLL = 0.0,      //Roll input from sensors
      SENS_PITCH = 0.0,     //Pitch input from sensors
      SENS_YAW = 0.0;       //Yaw input from sensors

float PID_ROLL  = 0.0,      //Roll output for motor controller
      PID_PITCH = 0.0,      //Pitch output for motor controller
      PID_YAW   = 0.0;      //Yaw output for motor controller

float pRollConst = 0.0,     //Proportional constant for ROLL
      iRollConst = 0.0,     //Integral constant for ROLL
      dRollConst = 0.0;     //Derivative constant for ROLL

float pPitchConst = 0.0,    //Proportional constant for PITCH
      iPitchConst = 0.0,    //Integral constant for PITCH
      dPitchConst = 0.0;    //Derivative constant for PITCH

float pYawConst = 0.0,      //Proportional constant for YAW
      iYawConst = 0.0,      //Integral constant for YAW
      dYawConst = 0.0;      //Derivative constant for YAW

float pErrRoll, iErrSumRoll, dErrChngRoll, rLastErr;
float pErrPitch, iErrSumPitch, dErrChngPitch, pLastErr;
float pErrYaw, iErrSumYaw, dErrChngYaw, yLastErr;

float   PID_ROLL_pErr,
        PID_ROLL_iErr,
        PID_ROLL_dErr;

int yawFlag = 0;

/*******************************************************************************
 * Function: MotorPID_computePIDValues                                         *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: Calls functions to set Roll, Pitch, and Yaw values individually*
 ******************************************************************************/
void MotorPID_computePIDValues(void)
{
    MotorPID_setPIDRoll();
    MotorPID_setPIDPitch();
    MotorPID_setPIDYaw();
}

/*******************************************************************************
 * Function: MotorPID_setPIDRoll                                               *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: This function takes in the IC_ROLL/GUI_ROLL value for controls *
 *              and the SENS_ROLL value and creates an error based on their    *
 *              difference. Each part of the PID is calculated based on its own*
 *              specific error calculation.                                    *
 ******************************************************************************/
void MotorPID_setPIDRoll(void)
{
    pErrRoll = IC_ROLL + GUI_ROLL - SENS_ROLL;

    iErrSumRoll += (pErrRoll * timer5TimeChange);
    dErrChngRoll = (pErrRoll - rLastErr) / timer5TimeChange;

    PID_ROLL = (pRollConst * pErrRoll) + (iRollConst * iErrSumRoll) + (dRollConst * dErrChngRoll);

    rLastErr = pErrRoll;

    PID_ROLL_pErr = pErrRoll;
    PID_ROLL_iErr = iErrSumRoll;
    PID_ROLL_dErr = dErrChngRoll;
}

/*******************************************************************************
 * Function: MotorPID_setPIDPitch                                              *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: This function takes in the IC_PITCH/GUI_PITCH value for control*
 *              and the SENS_PITCH value and creates an error based on their   *
 *              difference. Each part of the PID is calculated based on its own*
 *              specific error calculation.                                    *
 ******************************************************************************/
void MotorPID_setPIDPitch(void)
{
    pErrPitch = IC_PITCH + GUI_PITCH - SENS_PITCH;

    iErrSumPitch += (pErrPitch * timer5TimeChange);
    dErrChngPitch = (pErrPitch - pLastErr) / timer5TimeChange;

    PID_PITCH = (pPitchConst * pErrPitch) + (iPitchConst * iErrSumPitch) + (dPitchConst * dErrChngPitch);

    pLastErr = pErrPitch;
}

/*******************************************************************************
 * Function: MotorPID_setPIDYaw                                                *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: This function takes in the Calibration_Yaw_Zero_Point value for*
 *              controls and the SENS_ROLL value and creates an error based on *
 *              their difference. Each part of the PID is calculated based on  *
 *              its own specific error calculation. This function is different *
 *              than roll and pitch since a "zero yaw" is based solely on the  *
 *              direction it faces on startup.                                 *
 ******************************************************************************/
void MotorPID_setPIDYaw(void)
{
    if ((Calibration_Yaw_Zero_Point - SENS_YAW) > PI)
    {
        pErrYaw = Calibration_Yaw_Zero_Point - SENS_YAW - (2.0 * PI);
    }

    else if ((Calibration_Yaw_Zero_Point - SENS_YAW) < (-1.0 * PI))
    {
        pErrYaw = Calibration_Yaw_Zero_Point - SENS_YAW + (2.0 * PI);
    }

    else
    {
        pErrYaw = Calibration_Yaw_Zero_Point - SENS_YAW;
    }

    iErrSumYaw += (pErrYaw * timer5TimeChange);
    dErrChngYaw = (pErrYaw - yLastErr) / timer5TimeChange;

    PID_YAW = (pYawConst * pErrYaw) + (iYawConst * iErrSumYaw) + (dYawConst * dErrChngYaw);

    yLastErr = pErrYaw;
}

/*******************************************************************************
 * Function: MotorPID_getSensorValues                                          *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: Takes the globals given by the sensor orientation loop and     *
 *              saves them into the SENS_X variables for the PID algorithm to  *
 *              do calculations on without disrupting its original values.     *
 ******************************************************************************/
void MotorPID_getSensorValues(void)
{
    SENS_ROLL = OrientationLoop_Roll;
    SENS_PITCH = OrientationLoop_Pitch;
    SENS_YAW = OrientationLoop_Yaw;
}

/*******************************************************************************
 * Function: MotorPID_determineZeroYaw                                         *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: This function calculates a new "zero yaw", which is based on   *
 *              the IC_YAW/GUI_YAW input to the controller. The "zero yaw" is  *
 *              calculated upon startup as the direction the system is facing. *
 ******************************************************************************/
void MotorPID_determineZeroYaw(void)
{
    //Let the sensors sample a few times to get a good starting yaw point
    if (yawFlag < 250)
    {
        yawFlag++;
    }

    //Once delay is done, calibrate the new zero yaw, which takes the current
    //direction and saves it into Calibration_Yaw_Zero_Point
    else if (yawFlag == 250)
    {
        Calibration_zeroYaw();
        yawFlag++;
    }

    //Update the zero yaw based on controller input
    Calibration_Yaw_Zero_Point += ((IC_YAW + GUI_YAW) * PID_YAW_ROC);

    //Fixes the issue that -pi is right next to pi, we were originally seeing
    //large spikes in motor change without this
    if (Calibration_Yaw_Zero_Point > PI)
    {
        Calibration_Yaw_Zero_Point -= (2.0 * PI);
    }

    else if (Calibration_Yaw_Zero_Point < (-1.0 * PI))
    {
        Calibration_Yaw_Zero_Point += (2.0 * PI);
    }
}

/*******************************************************************************
 * Function: MotorPID_resetPID                                                 *
 * Input parameters: void                                                      *
 * Outputs/Returns: void                                                       *
 * Description: Zeroes all values that are changed directly by PID algorithm.  *
 ******************************************************************************/
void MotorPID_resetPID(void)
{
    //Set all error and PID outputs to zero
    PID_ROLL  = 0.0,
    PID_PITCH = 0.0,
    PID_YAW   = 0.0;

    PID_ROLL_pErr = 0.0;
    PID_ROLL_iErr = 0.0;
    PID_ROLL_dErr = 0.0;

    pErrRoll = 0.0,  iErrSumRoll = 0.0,  dErrChngRoll = 0.0,  rLastErr = 0.0;
    pErrPitch = 0.0, iErrSumPitch = 0.0, dErrChngPitch = 0.0, pLastErr = 0.0;
    pErrYaw = 0.0,   iErrSumYaw = 0.0,   dErrChngYaw = 0.0,   yLastErr = 0.0;
}
