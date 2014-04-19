#include "MotorPID.h"


float SENS_ROLL = 0.0,      //These are just placeholders for Connor's sensor output
      SENS_PITCH = 0.0, 
      SENS_YAW = 0.0;   

float PID_ROLL  = 0.0,      //Roll output for motor controller
      PID_PITCH = 0.0,      //Pitch output for motor controller
      PID_YAW   = 0.0;      //Yaw output for motor controller

float pRollConst = 0.0,    //Proportional constant
      iRollConst = 0.0,   //Integral constant
      dRollConst = 0.0;     //Derivative constant

float pPitchConst = 0.0,   //Proportional constant
      iPitchConst = 0.0,  //Integral constant
      dPitchConst = 0.0;    //Derivative constant

float pYawConst = 0.0,      //Proportional constant
      iYawConst = 0.0,      //Integral constant
      dYawConst = 0.0;      //Derivative constant

float scaledOrientationYaw;

float rErrSum, rLastErr, pErrSum, pLastErr, yErrSum, yLastErr;
float rError, dRollErr, pError, dPitchErr, yError, dYawErr;

int yawFlag = 0;

void MotorPID_computePIDValues(void)
{
    MotorPID_setPIDRoll();
    MotorPID_setPIDPitch();
    MotorPID_setPIDYaw();
}

void MotorPID_setPIDRoll(void)
{
    //clock_t rNow = clock();
    //float rTimeChange, rError;

    //time change is saved as milliseconds
    //rTimeChange = (((float)rNow - (float)rLastTime) / 1000000.0F) * 1000.0F;

    //rError = (2.0 * (float)IC_ROLL) - (float)SENS_ROLL;
    rError = IC_ROLL + GUI_ROLL - SENS_ROLL;

    //rErrSum += (rError * rTimeChange);
    rErrSum += (rError * timer5TimeChange);
    dRollErr = (rError - rLastErr) / timer5TimeChange;

    PID_ROLL = (pRollConst * rError) + (iRollConst * rErrSum) + (dRollConst * dRollErr);

    //PID_ROLL = pRollConst * rError; //Used just to get a number on the GUI (garbage code)

    rLastErr = rError;
    //rLastTime = rNow;
}

void MotorPID_setPIDPitch(void)
{
    //clock_t pNow = clock();
    //float pTimeChange, pError;

    //pTimeChange = (((float)pNow - (float)pLastTime) / 1000000.0F) * 1000.0F;

    //pError = (2.0 * (float)IC_PITCH) - (float)SENS_PITCH;
    pError = IC_PITCH + GUI_PITCH - SENS_PITCH;

    //pErrSum += (pError * pTimeChange);
    pErrSum += (pError * timer5TimeChange);
    dPitchErr = (pError - pLastErr) / timer5TimeChange;

    PID_PITCH = (pPitchConst * pError) + (iPitchConst * pErrSum) + (dPitchConst * dPitchErr);

    //PID_PITCH = iRollConst * rErrSum; //Used just to get a number on the GUI (garbage code)

    pLastErr = pError;
    //pLastTime = pNow;
}

void MotorPID_setPIDYaw(void)
{
    //unsigned int yNow = TMR5;
    //float yTimeChange, yError;

    //yTimeChange = (((float)yNow - (float)yLastTime) / 1000000.0F) * 1000;
    
    //This if-else block corrects for -PI to +PI jump
    /*if ((((-1.0 * PI) < SENS_YAW) && (SENS_YAW < (-1.0 * PI / 2.0))) && \
       (((PI / 2.0) < Calibration_Yaw_Zero_Point) && (Calibration_Yaw_Zero_Point < PI)))
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW - (2.0 * PI);
    }

    else if ((((PI / 2.0) < SENS_YAW) && (SENS_YAW < PI)) && \
            (((-1.0 * PI) < Calibration_Yaw_Zero_Point) && (Calibration_Yaw_Zero_Point < (-1.0 * PI / 2.0))))
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW + (2.0 * PI);
    }

    else
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW;
    }*/

    if ((Calibration_Yaw_Zero_Point - SENS_YAW) > PI)
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW - (2.0 * PI);
    }

    else if ((Calibration_Yaw_Zero_Point - SENS_YAW) < (-1.0 * PI))
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW + (2.0 * PI);
    }

    else
    {
        yError = Calibration_Yaw_Zero_Point - SENS_YAW;
    }

    //yErrSum += (yError * yTimeChange);
    yErrSum += (yError * timer5TimeChange);
    dYawErr = (yError - yLastErr) / timer5TimeChange;

    PID_YAW = (pYawConst * yError) + (iYawConst * yErrSum) + (dYawConst * dYawErr);

    yLastErr = yError;
    //yLastTime = yNow;
}

void MotorPID_getSensorValues(void)
{
    SENS_ROLL = OrientationLoop_Roll;
    SENS_PITCH = OrientationLoop_Pitch;
    SENS_YAW = OrientationLoop_Yaw;
}

void MotorPID_determineZeroYaw(void)
{
    if (yawFlag < 250)
    {
        yawFlag++;
    }

    else if (yawFlag == 250)
    {
        Calibration_zeroYaw();
        yawFlag++;
    }

    Calibration_Yaw_Zero_Point +=  ((IC_YAW + GUI_YAW) * PID_YAW_ROC);

    if (Calibration_Yaw_Zero_Point > PI)
    {
        Calibration_Yaw_Zero_Point -= (2.0 * PI);
    }

    else if (Calibration_Yaw_Zero_Point < (-1.0 * PI))
    {
        Calibration_Yaw_Zero_Point += (2.0 * PI);
    }
}

void MotorPID_resetPID(void)
{
    //Set all error and PID outputs to zero
    PID_ROLL  = 0.0,
    PID_PITCH = 0.0,
    PID_YAW   = 0.0,

    rErrSum = 0.0,   rLastErr  = 0.0,
    pErrSum = 0.0,   pLastErr  = 0.0,
    yErrSum = 0.0,   yLastErr  = 0.0,
    rError  = 0.0,   dRollErr  = 0.0,
    pError  = 0.0,   dPitchErr = 0.0,
    yError  = 0.0,   dYawErr   = 0.0;
}