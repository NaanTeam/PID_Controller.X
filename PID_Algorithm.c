#include "PID_Algorithm.h"
#include "RC_Receiver.h"
#include <time.h>

float SENS_ROLL = 0.0,      //These are just placeholders for Connor's sensor output
      SENS_PITCH = 0.0, 
      SENS_YAW = 0.0;   

float PID_ROLL = 0.0,       //Roll output for motor controller
      PID_PITCH = 0.0,      //Pitch output for motor controller
      PID_YAW = 0.0;        //Yaw output for motor controller

float pConst = 1.0,         //Peripheral constant
      iConst = 1.0,         //Intergral constant
      dConst = 1.0;         //Derivative constant

clock_t rLastTime, pLastTime, yLastTime;

float rErrSum, rLastErr, pErrSum, pLastErr, yErrSum, yLastErr;

void computePIDValues(void)
{
    setPIDRoll();
    setPIDPitch();
    setPIDYaw();
}

void setPIDRoll(void)
{
    clock_t rNow = clock();
    float rTimeChange, rError;

    //time change is saved as milliseconds
    rTimeChange = (((float)rNow - (float)rLastTime) / 1000000.0F) * 1000.0F;

    rError = (2.0F * (float)IC_ROLL) - (float)SENS_ROLL;

    rErrSum += (rError * rTimeChange);

    PID_ROLL = (pConst * rError) + (iConst * rErrSum);

    rLastErr = rError;
    rLastTime = rNow;
}

void setPIDPitch(void)
{
    clock_t pNow = clock();
    float pTimeChange, pError;

    pTimeChange = (((float)pNow - (float)pLastTime) / 1000000.0F) * 1000.0F;

    pError = (2.0F * (float)IC_PITCH) - (float)SENS_PITCH;

    pErrSum += (pError * pTimeChange);

    PID_PITCH = (pConst * pError) + (iConst * pErrSum);

    pLastErr = pError;
    pLastTime = pNow;
}

void setPIDYaw(void)
{
    clock_t yNow = clock();
    float yTimeChange, yError;

    yTimeChange = (((float)yNow - (float)yLastTime) / 1000000.0F) * 1000;

    yError = (float)IC_YAW - (float)SENS_YAW;

    yErrSum += (yError * yTimeChange);

    PID_YAW = (pConst * yError) + (iConst * yErrSum);

    yLastErr = yError;
    yLastTime = yNow;
}

void getSensorValues(void)
{
    SENS_ROLL = 0.0;
    SENS_PITCH = 0.0;
    SENS_YAW = 0.0;
}