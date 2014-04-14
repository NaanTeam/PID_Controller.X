
#include "OrientationLoop.h"

//******************************************************************************
//Local Variable and Typedef Declarations
//******************************************************************************
float accelCorrected[3] = {0, 0, 0};
float gyroCorrected[3] = {0, 0, 0};
float magneticCorrected[3] = {0, 0, 0};

float scaledAccelMagnitude = 0.0;
float gyroScaled[3] = {0, 0, 0};
float Magnetic_Heading = 0.0;

float timer5TimeChange = 20.0;

//******************************************************************************
//Public Variable Declarations
//******************************************************************************

float OrientationLoop_Roll = 0.0;
float OrientationLoop_Pitch = 0.0;
float OrientationLoop_Yaw = 0.0;




//******************************************************************************
//Local Function Declarations
//******************************************************************************

void OrientationLoop_adjustSensorError(float* accelCorrected, float* gyroCorrected, float* magneticCorrected);

float OrientationLoop_calcAccelMag(float* accelVector);
void OrientationLoop_scaleGyro(float* rawGyro, float* scaledGyro);
float OrientationLoop_calcCompassHeading(float* magneticVectors);


//******************************************************************************
//Local Function Definitions
//******************************************************************************

void OrientationLoop_adjustSensorError(float* accelCorrected, float* gyroCorrected, float* magneticCorrected)
{
    //Accel
    accelCorrected[0] = ADXL362_XAcceleration_Raw_Avg;
    accelCorrected[1] = ADXL362_YAcceleration_Raw_Avg;
    accelCorrected[2] = ADXL362_ZAcceleration_Raw_Avg;
    //Gyro
    gyroCorrected[0] = (float)(L3G4200D_XAngularRate_Raw_Avg - Calibration_Gyro_Offset_X);
    gyroCorrected[1] = (float)(L3G4200D_YAngularRate_Raw_Avg - Calibration_Gyro_Offset_Y);
    gyroCorrected[2] = (float)(L3G4200D_ZAngularRate_Raw_Avg - Calibration_Gyro_Offset_Z);
    //Magn
    magneticCorrected[0] = HMC5883L_XMagneticVector_Raw - XMagnetic_Offeset;
    magneticCorrected[1] = HMC5883L_YMagneticVector_Raw - YMagnetic_Offeset;
    magneticCorrected[2] = HMC5883L_ZMagneticVector_Raw - ZMagnetic_Offeset;

}

float OrientationLoop_calcAccelMag(float* accelVector)
{
    float accelMag = 0.0;

    // Calculate the magnitude of the accelerometer vector
    accelMag = sqrt(accelVector[0]*accelVector[0] + accelVector[1]*accelVector[1] + accelVector[2]*accelVector[2]);
    accelMag = accelMag / GRAVITY; // Scale to gravity.


    return accelMag;
}

void OrientationLoop_scaleGyro(float* rawGyro, float* scaledGyro)
{
    scaledGyro[0]=-1.0*Gyro_Scaled_X(rawGyro[0]); //gyro x roll
    scaledGyro[1]=-1.0*Gyro_Scaled_Y(rawGyro[1]); //gyro y pitch
    scaledGyro[2]=-1.0*Gyro_Scaled_Z(rawGyro[2]); //gyro Z yaw
}

float OrientationLoop_calcCompassHeading(float* magneticVectors)
{
    
  float Head_X;
  float Head_Y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;
  float Magnetic_Heading = 0.0;

  cos_roll = cos(OrientationLoop_Roll);
  sin_roll = sin(OrientationLoop_Roll);
  cos_pitch = cos(OrientationLoop_Pitch);
  sin_pitch = sin(OrientationLoop_Pitch);

  // Tilt compensated Magnetic field X component:
  Head_X = magneticVectors[0]*cos_pitch +
          magneticVectors[1]*sin_roll*sin_pitch +
          magneticVectors[2]*cos_roll*sin_pitch;
  
  // Tilt compensated Magnetic field Y component:
  Head_Y = magneticVectors[1]*cos_roll -
          magneticVectors[2]*sin_roll;

  // Magnetic Heading
  Magnetic_Heading = atan2(-Head_Y,Head_X);

  // Declination correction (if supplied)
  if( MAGNETIC_DECLINATION != 0 )
  {
      Magnetic_Heading = Magnetic_Heading + ToRad(MAGNETIC_DECLINATION);
      if (Magnetic_Heading > M_PI)    // Angle normalization (-180 deg, 180 deg)
          Magnetic_Heading -= (2.0 * M_PI);
      else if (Magnetic_Heading < -M_PI)
          Magnetic_Heading += (2.0 * M_PI);
  }

  return Magnetic_Heading;
  //return 0;

}


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void OrientationLoop_start()
{
    int i = 0;
    
    //Setup Timer5
    INTClearFlag(INT_T5);
    INTSetVectorPriority(INT_TIMER_5_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_5_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTEnable(INT_T5, INT_ENABLED);

    //Turn on clock
    OpenTimer5(T5_ON | T5_SOURCE_INT | T1_PS_1_64, 12500);//50hz @ 40MHz

    //gives it time to start up
    while (i < (1400 * 200))
    {
        i++;
    }
}

int i = 0;
void OrientationLoop_update()
{

    float accelCorrected[3];
    float gyroCorrected[3];
    float magneticCorrected[3];

    float scaledAccelMagnitude = 0.0;
    float gyroScaled[3];
    float Magnetic_Heading = 0.0;


    float timeDiff = (12500.0*64.0)/40000000.0;
    
    //Correct for raw sensor error
    OrientationLoop_adjustSensorError(accelCorrected, gyroCorrected, magneticCorrected);


    //Scale Accel Magnitude
    scaledAccelMagnitude = OrientationLoop_calcAccelMag(accelCorrected);
    //Convert Gyro to Rad/sec
    OrientationLoop_scaleGyro(gyroCorrected, gyroScaled);
    //Calculate Magnetic Heading
    Magnetic_Heading =OrientationLoop_calcCompassHeading(magneticCorrected);


    //Update DCM Matrix
    DCM_matrixUpdate(timeDiff, gyroScaled);
    //Renormalize the DCM matrix
    DCM_normalize();

    //Correct for drift
    DCM_driftCorrection(accelCorrected, scaledAccelMagnitude, Magnetic_Heading);
        


    //Convery DCM into euler angles
    DCM_eulerAngle(&OrientationLoop_Roll, &OrientationLoop_Pitch, &OrientationLoop_Yaw);

}

void OrientationLoop_stop()
{
    OpenTimer5(T5_OFF, 12500);
    INTEnable(INT_T5, INT_DISABLED);
    INTClearFlag(INT_T5);
}

void OrientationLoop_reset()
{
    accelCorrected[0] = 0.0;
    accelCorrected[1] = 0.0;
    accelCorrected[2] = 0.0;

    gyroCorrected[0] = 0.0;
    gyroCorrected[1] = 0.0;
    gyroCorrected[2] = 0.0;

    magneticCorrected[0] = 0.0;
    magneticCorrected[1] = 0.0;
    magneticCorrected[2] = 0.0;

    scaledAccelMagnitude = 0.0;

    gyroScaled[0] = 0.0;
    gyroScaled[1] = 0.0;
    gyroScaled[2] = 0.0;

    Magnetic_Heading = 0.0;

    timer5TimeChange = 20.0;

    OrientationLoop_Roll = 0.0;
    OrientationLoop_Pitch = 0.0;
    OrientationLoop_Yaw = 0.0;
}


void __ISR(_TIMER_5_VECTOR, IPL3AUTO) Timer5Handler(void)
{
    OrientationLoop_update();

    INTClearFlag(INT_T5);   // Be sure to clear the Timer1 interrupt status

    MotorPID_getSensorValues();
    MotorPID_determineZeroYaw();
    MotorPID_computePIDValues();
    MotorCtrl_adjustOCValues();

    read_voltage();

    //LATFINV = BIT_1;
}