#include "GUI.h"

float GUI_ROLL = 0.0,
      GUI_PITCH = 0.0,
      GUI_YAW = 0.0,
      GUI_THROTTLE = 0.0;

int GUI_EnableFlightFlag = 0;

void GUI_adjustRoll(float offset)
{
    GUI_ROLL += offset;
}

void GUI_adjustPitch(float offset)
{
    GUI_PITCH += offset;
}

void GUI_adjustYaw(float offset)
{
    GUI_YAW += offset;
}

void GUI_adjustThrottle(float offset)
{
    GUI_THROTTLE += offset;
}

void GUI_SoftRestart()
{
    SoftReset();
}

void GUI_EnableFlight()
{
    GUI_EnableFlightFlag = 1;
}

void GUI_DisableFlight()
{
    GUI_EnableFlightFlag = 0;
}
