#include "PID_Algorithm.h"
#include "RC_Receiver.h"
#include <time.h>

float SENS_ROLL = 0.0,  //These are just placeholders for Connor's sensor output
      SENS_PITCH = 0.0, 
      SENS_YAW = 0.0;   

float PID_ROLL,         //
      PID_PITCH,        //
      PID_YAW;          //

float pConst = 1,       //
      iConst = 1,       //
      dConst = 1;       //

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
    rTimeChange = (((float)rNow - (float)rLastTime) / 1000000.0F) * 1000;

    rError = (2 * IC_ROLL) - SENS_ROLL;

    rErrSum += (rError * rTimeChange);

    PID_ROLL = (pConst * rError) + (iConst * rErrSum);

    rLastErr = rError;
    rLastTime = rNow;
}

void setPIDPitch(void)
{
    clock_t pNow = clock();
    float pTimeChange, pError;

    pTimeChange = (((float)pNow - (float)pLastTime) / 1000000.0F) * 1000;

    pError = (2 * IC_PITCH) - SENS_PITCH;

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

    yError = IC_YAW - SENS_YAW;

    yErrSum += (yError * yTimeChange);

    PID_YAW = (pConst * yError) + (iConst * yErrSum);

    yLastErr = yError;
    yLastTime = yNow;
}

/*void __ISR(_TIMER_5_VECTOR) TIMER5Handler(void)
{
    mT5ClearIntFlag();

    getSensorValues();
    computePIDValues();
    adjustOCValues();
}*/

void getSensorValues(void)
{
    SENS_ROLL = 0;
    SENS_PITCH = 0;
    SENS_YAW = 0;
}