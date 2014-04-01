
#include "Orientation.h"

//******************************************************************************
//Local Variable and Typedef Declarations
//******************************************************************************
UINT8 Orientation_loopToggle = 1;

float accelCorrected[3];
float gyroCorrected[3];
float magneticCorrected[3];

float scaledAccelMagnitude = 0.0;
float gyroScaled[3];
float Magnetic_Heading = 0.0;


float timeDiff = 0.01638;
    

//******************************************************************************
//Public Variable Declarations
//******************************************************************************

float Orientation_Roll = 0.0;
float Orientation_Pitch = 0.0;
float Orientation_Yaw = 0.0;




//******************************************************************************
//Local Function Declarations
//******************************************************************************

void Orientation_adjustSensorError(float* accelCorrected, float* gyroCorrected, float* magneticCorrected);

float Orientation_calcAccelMag(float* accelVector);
void Orientation_scaleGyro(float* rawGyro, float* scaledGyro);
float Orientation_calcCompassHeading(float* magneticVectors);


//******************************************************************************
//Local Function Definitions
//******************************************************************************

void Orientation_adjustSensorError(float* accelCorrected, float* gyroCorrected, float* magneticCorrected)
{
    //Accel
    accelCorrected[0] = ADXL362_XAcceleration_Raw_Avg - XAccel_Offeset;
    accelCorrected[1] = ADXL362_YAcceleration_Raw_Avg - YAccel_Offeset;
    accelCorrected[2] = ADXL362_ZAcceleration_Raw_Avg - ZAccel_Offeset;
    //Gyro
    gyroCorrected[0] = (float)(L3G4200D_XAngularRate_Raw_Avg - Calibration_Gyro_Offset_X);
    gyroCorrected[1] = (float)(L3G4200D_YAngularRate_Raw_Avg - Calibration_Gyro_Offset_Y);
    gyroCorrected[2] = (float)(L3G4200D_ZAngularRate_Raw_Avg - Calibration_Gyro_Offset_Z);
    //Magn
    magneticCorrected[0] = HMC5883L_XMagneticVector_Raw - XMagnetic_Offeset;
    magneticCorrected[1] = HMC5883L_YMagneticVector_Raw - YMagnetic_Offeset;
    magneticCorrected[2] = HMC5883L_ZMagneticVector_Raw - ZMagnetic_Offeset;

}

float Orientation_calcAccelMag(float* accelVector)
{
    float accelMag = 0.0;

    // Calculate the magnitude of the accelerometer vector
    accelMag = sqrt(accelVector[0]*accelVector[0] + accelVector[1]*accelVector[1] + accelVector[2]*accelVector[2]);
    accelMag = accelMag / GRAVITY; // Scale to gravity.


    return accelMag;
}

void Orientation_scaleGyro(float* rawGyro, float* scaledGyro)
{
    scaledGyro[0]=-1.0*Gyro_Scaled_X(rawGyro[0]); //gyro x roll
    scaledGyro[1]=-1.0*Gyro_Scaled_Y(rawGyro[1]); //gyro y pitch
    scaledGyro[2]=-1.0*Gyro_Scaled_Z(rawGyro[2]); //gyro Z yaw
}

float Orientation_calcCompassHeading(float* magneticVectors)
{
    
  float Head_X;
  float Head_Y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;
  float Magnetic_Heading = 0.0;

  cos_roll = cos(Orientation_Roll);
  sin_roll = sin(Orientation_Roll);
  cos_pitch = cos(Orientation_Pitch);
  sin_pitch = sin(Orientation_Pitch);

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

}


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void Orientation_start()
{
    //Setup Timer5
    INTClearFlag(INT_T5);
    INTSetVectorPriority(INT_TIMER_5_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_5_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTEnable(INT_T5, INT_ENABLED);

    //Turn on clock
    OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_32, 16666); //75hz @ 40MHz
    //OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_32, 3333); //375hz @ 40MHz  (0.0026664 sec)
}


void Orientation_Update()
{

    float accelCorrected[3];
    float gyroCorrected[3];
    float magneticCorrected[3];

    float scaledAccelMagnitude = 0.0;
    float gyroScaled[3];
    float Magnetic_Heading = 0.0;


    float timeDiff = 1/75.0;
    
    //Correct for raw sensor error
    Orientation_adjustSensorError(accelCorrected, gyroCorrected, magneticCorrected);


    //Scale Accel Magnitude
    scaledAccelMagnitude = Orientation_calcAccelMag(accelCorrected);
    //Convert Gyro to Rad/sec
    Orientation_scaleGyro(gyroCorrected, gyroScaled);
    //Calculate Magnetic Heading
    Magnetic_Heading = Orientation_calcCompassHeading(magneticCorrected);


    //Update DCM Matrix
    DCM_matrixUpdate(timeDiff, gyroScaled);
    //Renormalize the DCM matrix
    DCM_normalize();
    //Calculate drift correction
    DCM_driftCorrection(accelCorrected, scaledAccelMagnitude, Magnetic_Heading);


    //Convery DCM into euler angles
    DCM_eulerAngle(&Orientation_Roll, &Orientation_Pitch, &Orientation_Yaw);

}


void __ISR(_TIMER_5_VECTOR, IPL3AUTO) Timer5Handler(void) //75hz
{

    Orientation_Update();

    INTClearFlag(INT_T5);// Be sure to clear the Timer1 interrupt status

//    getSensorValues();
//    computePIDValues();
//    adjustOCValues();
}