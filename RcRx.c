/*******************************************************************************
 * Programmer: Michael Taylor                                                  *
 * Class: EE 416                                                               *
 * Lab Project: RC Reciever for PID Controller for Senior Design               *
 * Last Update: February 28, 2014                                              *
 * Description: This program is designed to interpret RC reciever signals for  *
 *              Team Naan's senior design project, which is to impelement a PID*
 *              algorithm to control four motors (quadcopter).                 *
 ******************************************************************************/
#include "RcRx.h"

#define THRO_H          5114.0
#define YAW_H           5116.0
#define PITCH_H         5115.0
#define ROLL_H          5116.0

#define MID_THRESH_H    3815.0
#define MID_THRESH_L    3785.0

#define THRO_L          2479.0
#define YAW_L           2476.0
#define PITCH_L         2477.0
#define ROLL_L          2477.0

unsigned int IC1_CT_Rise, IC1_CT_Fall, period1,
             IC2_CT_Rise, IC2_CT_Fall, period2, ym,
             IC3_CT_Rise, IC3_CT_Fall, period3, pm,
             IC4_CT_Rise, IC4_CT_Fall, period4, rm,
             IC5_CT_Rise, IC5_CT_Fall, period5;

float IC_THRO  = 0,             //Throttle percentage 0.0 thru 100.0%
      IC_YAW   = 0,             //Yaw angle -180.0 thru 180.0 degrees
      IC_PITCH = 0,             //Pitch angle -90.0 thru 90.0 degrees
      IC_ROLL  = 0;             //Roll angle -90.0 thru 90.0 degrees

unsigned int IC_AUX =  0;       //Aux switch 0 or 1
unsigned int PROPS_ENABLE = 0;  //Props enabled flag 0 or 1

unsigned int garbage[16];       //Extra buffer value go here
unsigned int zero_mode = 1;     //Mode to zero out controller thumbsticks

float yaw_pc    = 0,            //Percentage values of yaw, pitch, and roll
      pitch_pc  = 0,
      roll_pc   = 0;


void RcRx_zeroController(void)
{
    ym, pm, rm = 0;
    int i, j, k;

    while(zero_mode == 1)
    {
        for(i = 0; i < 50; i++)
        {
            if(period2 > MID_THRESH_L && period2 < MID_THRESH_H)
                ym = ym + period2;
            else
                i--;
        }
        ym = ym / 50;

        for(j = 0; j < 50; j++)
        {
            if(period3 > MID_THRESH_L && period3 < MID_THRESH_H)
                pm = pm + period3;
            else
                j--;
        }
        pm = pm / 50;

        for(k = 0; k < 50; k++)
        {
            if(period4 > MID_THRESH_L && period4 < MID_THRESH_H)
                rm = rm + period4;
            else
                k--;
        }
        rm = rm / 50;

        if(ym > MID_THRESH_L && ym < MID_THRESH_H &&
           pm > MID_THRESH_L && pm < MID_THRESH_H &&
           rm > MID_THRESH_L && rm < MID_THRESH_H)
        {
            zero_mode = 0;
        }
    }
}

//Both sticks down and in
void RcRx_enableProps(void)
{
    while(IC_THRO > 0.25 || IC_YAW > (-1.0*(PI-0.2)) || IC_PITCH < ((PI/2.0)-0.1) || IC_ROLL < ((PI/2.0)-0.1)){}
    while(IC_THRO > 0.25 || IC_YAW != 0.0 || IC_PITCH != 0.0 || IC_ROLL != 0.0){}
    PROPS_ENABLE = 1;
}

//Both sticks down and out
int RcRx_disableProps(void)
{
    if(IC_AUX == 100)
    {
        PROPS_ENABLE = 0;
        return 1;
    }

    if(IC_THRO < 0.25 && IC_YAW > (PI-0.2) && IC_PITCH > ((PI/2)-0.1) && IC_ROLL < -(PI/2)-0.2)
    {
        PROPS_ENABLE = 0;
        return 1;
    }
    
    return 0;
}

void RcRx_setupInputCaptures(void)
{
    //clear control registers
    IC1CONCLR = 0xFFFF;
    IC2CONCLR = 0xFFFF;
    IC3CONCLR = 0xFFFF;
    IC4CONCLR = 0xFFFF;
    IC5CONCLR = 0xFFFF;

    //set control registers
    IC1CONSET = 0x82A6;
    IC2CONSET = 0x82A6;
    IC3CONSET = 0x8226;
    IC4CONSET = 0x8226;
    IC5CONSET = 0x8226;
}

void __ISR(_INPUT_CAPTURE_1_VECTOR) INT_IC1_Handler(void)
{
    mIC1ClearIntFlag();

    //Read captured timer values
    if(mIC1CaptureReady())
    {
        IC1_CT_Rise = mIC1ReadCapture();
        IC1_CT_Fall = mIC1ReadCapture();
    }

    //Determine period
    if(IC1_CT_Rise > IC1_CT_Fall)
        period1 = (IC1_CT_Fall + PER_REG) - IC1_CT_Rise;
    else
        period1 = IC1_CT_Fall - IC1_CT_Rise;

    //Correct period if out of range
    if(period1 > THRO_H - 2)
        period1 = THRO_H;
    else if(period1 < THRO_L + 5)
        period1 = THRO_L;

    //Turn period into Throttle percentage 0.0 thru 100.0%
    IC_THRO = ((period1 - THRO_L) / (THRO_H-THRO_L)) * 100.0;

    ReadCapture1(garbage); //clear any remaining capture values
}

void __ISR(_INPUT_CAPTURE_2_VECTOR) INT_IC2_Handler(void)
{
    mIC2ClearIntFlag();

    //Read captured timer values
    if(mIC2CaptureReady())
    {
        IC2_CT_Rise = mIC2ReadCapture();
        IC2_CT_Fall = mIC2ReadCapture();
    }

    if(mIC2CaptureReady())
        ReadCapture2(garbage); //clear any remaining capture values

    //Determine period
    if(IC2_CT_Rise > IC2_CT_Fall)
        period2 = (IC2_CT_Fall + PER_REG) - IC2_CT_Rise;
    else
        period2 = IC2_CT_Fall - IC2_CT_Rise;

    //Correct period if out of range
    if(period2 > YAW_H - 3)
        period2 = YAW_H;
    else if(period2 < YAW_L + 3)
        period2 = YAW_L;

    //Turn period into a percentage 0.0 thru 100.0% and
    //a Yaw angle -180.0 thru 180.0 degrees
    if(zero_mode == 0)
    {
        if(period2 > ym + 5)
        {
            yaw_pc = ((YAW_H + period2 - (2 * ym)) / (YAW_H - ym)) * 50.0;
            IC_YAW = 2 * PI * ((yaw_pc - 50) / 100.0);
        }
        
        else if(period2 < ym - 5)
        {
            yaw_pc = ((period2 - YAW_L) / (ym - YAW_L)) * 50.0;
            IC_YAW = -2.0 * PI * ((50.0 - yaw_pc) / 100.0);
        }
        
        else
        {
            yaw_pc = 50.0;
            IC_YAW = 0.0;
        }
    }
}

void __ISR(_INPUT_CAPTURE_3_VECTOR) INT_IC3_Handler(void)
{
    mIC3ClearIntFlag();

    //Read captured timer values
    if(mIC3CaptureReady())
    {
        IC3_CT_Rise = mIC3ReadCapture();
        IC3_CT_Fall = mIC3ReadCapture();
    }

    //Determine period
    if(IC3_CT_Rise > IC3_CT_Fall)
        period3 = (IC3_CT_Fall + PER_REG) - IC3_CT_Rise;
    else
        period3 = IC3_CT_Fall - IC3_CT_Rise;

    //Correct period if out of range
    if(period3 > PITCH_H - 3)
        period3 = PITCH_H;
    else if(period3 < PITCH_L + 3)
        period3 = PITCH_L;

    //Turn period into a percentage 0.0 thru 100.0% and
    //a Pitch angle -90.0 thru 90.0 degrees
    if(zero_mode == 0)
    {
        if(period3 > pm + 5)
        {
            pitch_pc = ((PITCH_H + period3 - (2 * pm)) / (PITCH_H - pm)) * 50.0;
            IC_PITCH = -1.0 * PI * ((pitch_pc - 50) / 100.0);
        }
        
        else if(period3 < pm - 5)
        {
            pitch_pc = ((period3 - PITCH_L) / (pm - PITCH_L)) * 50.0;
            IC_PITCH = PI * ((50.0 - pitch_pc) / 100.0);
        }
        
        else
        {
            pitch_pc = 50.0;
            IC_PITCH = 0.0;
        }
    }

    ReadCapture3(garbage); //clear any remaining capture values
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) INT_IC4_Handler(void)
{
    mIC4ClearIntFlag();

    //Read captured timer values
    if(mIC4CaptureReady())
    {
        IC4_CT_Rise = mIC4ReadCapture();
        IC4_CT_Fall = mIC4ReadCapture();
    }

    //Determine period
    if(IC4_CT_Rise > IC4_CT_Fall)
        period4 = (IC4_CT_Fall + PER_REG) - IC4_CT_Rise;
    else
        period4 = IC4_CT_Fall - IC4_CT_Rise;

    //Correct period if out of range
    if(period4 > ROLL_H - 3)
        period4 = ROLL_H;
    else if(period4 < ROLL_L + 3)
        period4 = ROLL_L;

    //Turn period into a percentage 0.0 thru 100.0% and
    //a Roll angle -90.0 thru 90.0 degrees
    if(zero_mode == 0)
    {
        if(period4 > rm + 5)
        {
            roll_pc = ((ROLL_H + period4 - (2 * rm)) / (ROLL_H - rm)) * 50.0;
            IC_ROLL = PI * ((roll_pc - 50) / 100.0);
        }
        
        else if(period4 < rm - 5)
        {
            roll_pc = ((period4 - ROLL_L) / (rm-ROLL_L)) * 50.0;
            IC_ROLL = -1.0 * PI * ((50.0 - roll_pc) / 100.0);
        }
        
        else
        {
            roll_pc = 50.0;
            IC_ROLL = 0.0;
        }
    }

    ReadCapture4(garbage); //clear any remaining capture values
}

void __ISR(_INPUT_CAPTURE_5_VECTOR) INT_IC5_Handler(void)
{
    mIC5ClearIntFlag();

    //Read captured timer values
    if(mIC5CaptureReady())
    {
        IC5_CT_Rise = mIC5ReadCapture();
        IC5_CT_Fall = mIC5ReadCapture();
    }

    //Determine period
    if(IC5_CT_Rise > IC5_CT_Fall)
        period5 = (IC5_CT_Fall + PER_REG) - IC5_CT_Rise;
    else
        period5 = IC5_CT_Fall - IC5_CT_Rise;

    //Turn period into Aux percentage 0.0 or 100.0%
    if(period5 > (int)MID_THRESH_L)
        IC_AUX = 100;
    else if(period5 < (int)MID_THRESH_L)
        IC_AUX = 0;

    ReadCapture5(garbage); //clear any remaining capture values
}
