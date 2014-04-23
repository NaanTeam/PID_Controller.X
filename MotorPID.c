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

float rErrSum, rLastErr, pErrSum, pLastErr, yErrSum, yLastErr;
float rError, dRollErr, pError, dPitchErr, yError, dYawErr;

float PID_ROLL_rError,
        PID_ROLL_rErrSum,
        PID_ROLL_dRollErr;

int yawFlag = 0;

void MotorPID_computePIDValues(void)
{
    MotorPID_setPIDRoll();
    MotorPID_setPIDPitch();
    MotorPID_setPIDYaw();
}

void MotorPID_setPIDRoll(void)
{
    PID_ROLL_rError = IC_ROLL + GUI_ROLL - SENS_ROLL;

    PID_ROLL_rErrSum += (PID_ROLL_rError * timer5TimeChange);
    PID_ROLL_dRollErr = (PID_ROLL_rError - rLastErr) / timer5TimeChange;

    PID_ROLL = (pRollConst * PID_ROLL_rError) + (iRollConst * PID_ROLL_rErrSum) + (dRollConst * PID_ROLL_dRollErr);

    rLastErr = PID_ROLL_rError;
}

void MotorPID_setPIDPitch(void)
{
    pError = IC_PITCH + GUI_PITCH - SENS_PITCH;

    pErrSum += (pError * timer5TimeChange);
    dPitchErr = (pError - pLastErr) / timer5TimeChange;

    PID_PITCH = (pPitchConst * pError) + (iPitchConst * pErrSum) + (dPitchConst * dPitchErr);

    pLastErr = pError;
}

void MotorPID_setPIDYaw(void)
{
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

    yErrSum += (yError * timer5TimeChange);
    dYawErr = (yError - yLastErr) / timer5TimeChange;

    PID_YAW = (pYawConst * yError) + (iYawConst * yErrSum) + (dYawConst * dYawErr);

    yLastErr = yError;
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

    PID_ROLL_rError = 0.0;
    PID_ROLL_rErrSum = 0.0;
    PID_ROLL_dRollErr = 0.0;

    rErrSum = 0.0,   rLastErr  = 0.0,
    pErrSum = 0.0,   pLastErr  = 0.0,
    yErrSum = 0.0,   yLastErr  = 0.0,
    rError  = 0.0,   dRollErr  = 0.0,
    pError  = 0.0,   dPitchErr = 0.0,
    yError  = 0.0,   dYawErr   = 0.0;
}