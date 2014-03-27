/*******************************************************************************
 * Programmer: Ryan Moniz and Michael Taylor                                   *
 * Class: EE 416                                                               *
 * Lab Project: Motor Controller for PID Controller for Senior Design          *
 * Last Update: March 5, 2014                                                  *
 * Description: This program is designed to run the control of the motors for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#include "RC_Receiver.h"
#include "PID_Algorithm.h"
#include "motorController.h"
#include "CommunicationLoop.h"
#include "ADXL362.h"
#include "L3G4200D.h"
#include "FIFOSPI2.h"
#include "HMC5883L.h"
#include "Orientation.h"
#include "setup.h"
#include "Calibration.h"

#pragma config FNOSC    = PRIPLL    // Oscillator selection
//NOTE: Mike and I had POSCMOD set to EC
#pragma config POSCMOD  = HS        // HS oscillator mode
#pragma config FPLLIDIV = DIV_2     // PLL input divider
#pragma config FPLLMUL  = MUL_20    // PLL multiplier
#pragma config FPLLODIV = DIV_1     // PLL output divider
#pragma config FPBDIV   = DIV_2     // Peripheral bus clock divider (40MHz)
#pragma config FSOSCEN  = OFF       // Secondary oscillator enable

//Connor's Pragma statements
#pragma config CP       = OFF
#pragma config BWP      = OFF
#pragma config PWP      = OFF
#pragma config ICESEL   = ICS_PGx2
#pragma config DEBUG    = OFF
#pragma config FWDTEN   = OFF
#pragma config WDTPS    = PS1
#pragma config FCKSM    = CSDCMD
#pragma config OSCIOFNC = OFF
#pragma config IESO     = OFF

void DelayTime(int inter)
{
    int i = 0;

    while (i < (inter * 200))
    {
        i++;
    }
}

int main (void)
{
    Setup_initializeBoard();

    zeroController();

    Setup_startupCommunications();

    DelayTime(500);

    Calibration_sensorsOffsets();
    Orientation_start();

    enableProps();
    startupMotors();

    //StartSequence()

    //Zero Sensors()

    while (1)
    {
        if (disableProps())
        {
            OC1RS = OC_KILL;
            OC2RS = OC_KILL;
            OC3RS = OC_KILL;
            OC4RS = OC_KILL;

            enableProps();
        }
        
        //Update OCxRS registers with newly calculated values
        OC1RS = OC_ONE;     //Front-Left Motor
        OC2RS = OC_TWO;     //Front-Right Motor
        OC3RS = OC_THREE;   //Back-Right Motor
        OC4RS = OC_FOUR;    //Back-Left Motor

        OC_ONE_PC = (OC_ONE - OC_KILL) / (OC_MAX - OC_KILL);
        OC_TWO_PC = (OC_TWO - OC_KILL) / (OC_MAX - OC_KILL);
        OC_THREE_PC = (OC_THREE - OC_KILL) / (OC_MAX - OC_KILL);
        OC_FOUR_PC = (OC_FOUR - OC_KILL) / (OC_MAX - OC_KILL);
    }

    return 0;
}
