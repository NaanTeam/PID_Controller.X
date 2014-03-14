#include "SensorLoop.h"

//******************************************************************************
//Public Function Definitions
//******************************************************************************
void SensorLoop_SetupAll()
{
    //Setup SPI and I2C
    FIFOSPI2_initialize();
    FIFOI2C2_initialize();

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

////    //Setup Accelerometer
    ADXL362_startMeasurements();
////    //Setup Gyroscope
    L3G4200D_startMeasurements();
////   //Setup 3-axis compass
    HMC5883L_startMeasurements();

    INTDisableInterrupts();


    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);
    //Turn on clock
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 12500); //800hz

}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************

//800 hz
int SensorLoop_ToggleCount = 0;

#define Avg_Count 11
#define alpha 0.05
//Running Average 
int gyroRunning_x[Avg_Count];
int gyroTotal_x = 0;

int accelRunning_x[Avg_Count];
int accelTotal_x = 0;

int output = 0;

//TODO KNOWN BUG: with SPI. Reordering ADXl and L3G will make L3G's reading bad.
void __ISR(_TIMER_1_VECTOR, IPL3AUTO) Timer1Handler(void)
{

    ADXL362_popXYZT();
    L3G4200D_popXYZT();
    if (SensorLoop_ToggleCount == (Avg_Count - 1)) //72.7hz
    {
        HMC5883L_popXZY();
    }


////    //Calculate Running  x
////    gyroTotal_x += L3G4200D_XAngularRate_Raw;
////    gyroTotal_x -= gyroRunning_x[SensorLoop_ToggleCount];
////    gyroRunning_x[SensorLoop_ToggleCount] = L3G4200D_XAngularRate_Raw;
////    L3G4200D_XAngularRate_Raw_Avg = gyroTotal_x/Avg_Count;
    
//    output = (int)(alpha * (float)L3G4200D_XAngularRate_Raw + output * (1.0 - alpha));
//    L3G4200D_XAngularRate_Raw_Avg = output;
    
    output = (L3G4200D_XAngularRate_Raw + 19 * output) / 20; //13
    L3G4200D_XAngularRate_Raw_Avg = output;


    //Convert Raw data into meaningful data(optional and potential optimization)
    ADXL362_convertXYZT();
    L3G4200D_convertXYZT();
    if (SensorLoop_ToggleCount == (Avg_Count - 1)) //72.7hz
    {
        HMC5883L_convertXYZ();
    }
    


    //Que reads to the sensors for next timer tick.
    ADXL362_pushReadXYZT();
    L3G4200D_pushReadXYZT();
    if (SensorLoop_ToggleCount == (Avg_Count - 1)) //72.7hz
    {
        HMC5883L_pushReadXZY();
    }
    

    SensorLoop_ToggleCount++;
    if (SensorLoop_ToggleCount == Avg_Count)
    {
        SensorLoop_ToggleCount = 0;
    }
    

    INTClearFlag(INT_T1);// Be sure to clear the Timer1 interrupt status
}

