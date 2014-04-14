#include "SensorLoop.h"

//******************************************************************************
//Public Function Definitions
//******************************************************************************
void SensorLoop_start()
{

    //Setup Accelerometer
    ADXL362_startMeasurements();
    //Setup Gyroscope
    L3G4200D_startMeasurements();
    //Setup 3-axis compass
    HMC5883L_startMeasurements();


    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);
    //Turn on clock
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, 3125); //200hz //6250);//100hz @ 40MHz //2*6250);//400hz @ 40MHz  //6250); //800hz @ 40MHz (T1_PS_1_8)

    //T1_PS_1_64, 12500);//50hz @ 40MHz

}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************

//800 hz
int SensorLoop_ToggleCount = 0;

#define Avg_Count 6
#define alpha 0.05
//Running Average 
int gyroRunning_x[Avg_Count];
int gyroTotal_x = 0;

int accelRunning_x[Avg_Count];
int accelTotal_x = 0;

//TODO KNOWN BUG: with SPI. Reordering ADXl and L3G will make L3G's reading bad.
void __ISR(_TIMER_1_VECTOR, IPL3AUTO) Timer1Handler(void)
{

    ADXL362_popXYZT();
    L3G4200D_popXYZT();
    HMC5883L_popXZY();

    //Calibrate Raws
    Calibration_offsetAcceleration();

    ADXL362_XAcceleration_Raw_Avg = (0.007*ADXL362_XAcceleration_Raw +
        0.993 * ADXL362_XAcceleration_Raw_Avg);
    ADXL362_YAcceleration_Raw_Avg = (0.007*ADXL362_YAcceleration_Raw +
        0.993 * ADXL362_YAcceleration_Raw_Avg);
    ADXL362_ZAcceleration_Raw_Avg = (0.007*ADXL362_ZAcceleration_Raw +
        0.993 * ADXL362_ZAcceleration_Raw_Avg);


    L3G4200D_XAngularRate_Raw_Avg = (0.30*L3G4200D_XAngularRate_Raw +
        0.70 * L3G4200D_XAngularRate_Raw_Avg);
    L3G4200D_YAngularRate_Raw_Avg = (0.30*L3G4200D_YAngularRate_Raw +
        0.70 * L3G4200D_YAngularRate_Raw_Avg);
    L3G4200D_ZAngularRate_Raw_Avg = (0.30*L3G4200D_ZAngularRate_Raw +
        0.70 * L3G4200D_ZAngularRate_Raw_Avg);

    ADXL362_convertXYZT();
    L3G4200D_convertXYZT();
    HMC5883L_convertXYZ();

    ADXL362_pushReadXYZT();
    L3G4200D_pushReadXYZT();
    HMC5883L_pushReadXZY();

    INTClearFlag(INT_T1);// Be sure to clear the Timer1 interrupt status
}

