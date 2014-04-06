#include "PID_Algorithm.h"
#include "RC_Receiver.h"
#include "Orientation.h"
#include "Calibration.h"
#include <time.h>

float SENS_ROLL = 0.0,      //These are just placeholders for Connor's sensor output
      SENS_PITCH = 0.0, 
      SENS_YAW = 0.0;   

float PID_ROLL = 0.0,       //Roll output for motor controller
      PID_PITCH = 0.0,      //Pitch output for motor controller
      PID_YAW = 0.0;        //Yaw output for motor controller

float pRollConst = 0.0,     //Peripheral constant
      iRollConst = 0.0,     //Intergral constant
      dRollConst = 0.0;     //Derivative constant

float pPitchConst = 0.0,    //Peripheral constant
      iPitchConst = 0.0,    //Intergral constant
      dPitchConst = 0.0;    //Derivative constant

float pYawConst = 0.0,     //Peripheral constant
      iYawConst = 0.0,      //Intergral constant
      dYawConst = 0.0;      //Derivative constant

float scaledOrientationYaw;

unsigned int rLastTime, pLastTime, yLastTime;

float rErrSum, rLastErr, pErrSum, pLastErr, yErrSum, yLastErr;

int yawFlag = 0;

void computePIDValues(void)
{
    setPIDRoll();
    setPIDPitch();
    setPIDYaw();
}

void setPIDRoll(void)
{
    //clock_t rNow = clock();
    //float rTimeChange, rError;
    float rError, dRollErr;

    //time change is saved as milliseconds
    //rTimeChange = (((float)rNow - (float)rLastTime) / 1000000.0F) * 1000.0F;

    rError = (2.0F * (float)IC_ROLL) - (float)SENS_ROLL;

    //rErrSum += (rError * rTimeChange);
    rErrSum += (rError * timer5TimeChange);
    dRollErr = (rError - rLastErr) / timer5TimeChange;

    PID_ROLL = (pRollConst * rError) + (iRollConst * rErrSum) + (dRollConst * dRollErr);

    rLastErr = rError;
    //rLastTime = rNow;
}

void setPIDPitch(void)
{
    //clock_t pNow = clock();
    //float pTimeChange, pError;
    float pError, dPitchErr;

    //pTimeChange = (((float)pNow - (float)pLastTime) / 1000000.0F) * 1000.0F;

    pError = (2.0F * (float)IC_PITCH) - (float)SENS_PITCH;

    //pErrSum += (pError * pTimeChange);
    pErrSum += (pError * timer5TimeChange);
    dPitchErr = (pError - pLastErr) / timer5TimeChange;

    PID_PITCH = (pPitchConst * pError) + (iPitchConst * pErrSum) + (dPitchConst * dPitchErr);

    pLastErr = pError;
    //pLastTime = pNow;
}

void setPIDYaw(void)
{
    //unsigned int yNow = TMR5;
    //float yTimeChange, yError;
    float yError, dYawErr;

    //yTimeChange = (((float)yNow - (float)yLastTime) / 1000000.0F) * 1000;

    yError = Calibration_Yaw_Zero_Point - (float)SENS_YAW;

    //yErrSum += (yError * yTimeChange);
    yErrSum += (yError * timer5TimeChange);
    dYawErr = (yError - yLastErr) / timer5TimeChange;

    PID_YAW = (pYawConst * yError) + (iYawConst * yErrSum) + (dYawConst * dYawErr);

    yLastErr = yError;
    //yLastTime = yNow;
}

void getSensorValues(void)
{
    SENS_ROLL = Orientation_Roll;
    SENS_PITCH = Orientation_Pitch;

    if (Orientation_Yaw > 0)
    {
        scaledOrientationYaw = Orientation_Yaw / 2.0;
    }

    else if (Orientation_Yaw > -3.15 && Orientation_Yaw <= -2.10)
    {
        scaledOrientationYaw = (Orientation_Yaw * 1.5) + 6.283;
    }

    else if (Orientation_Yaw <= 0 && Orientation_Yaw > -2.10)
    {
        scaledOrientationYaw = Orientation_Yaw * 1.5;
    }

    else
    {
        scaledOrientationYaw = Orientation_Yaw;
    }

    SENS_YAW = scaledOrientationYaw;
}

void determineZeroYaw(void)
{
    if (yawFlag == 0)
    {
        Calibration_zeroYaw();

        yawFlag = 1;
    }

    Calibration_Yaw_Zero_Point += (IC_YAW * PID_YAW_ROC);
}