
#include "DCM.h"

//******************************************************************************
//Local Variable and Typedef Declarations
//******************************************************************************

float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

float errorRollPitch[3]= {0,0,0};
float errorYaw[3]= {0,0,0};
float errorCourse=180;

float DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
};
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here
float Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};


//******************************************************************************
//Public Function Definitions
//******************************************************************************

void DCM_matrixUpdate(float timeDiff, float* gyroRadSec)
{

    int x = 0, y = 0;


    Vector_Add(&Omega[0], &gyroRadSec[0], &Omega_I[0]);  //adding proportional term
    Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term


//    Update_Matrix[0][0]=0;
//    Update_Matrix[0][1]=-timeDiff*gyroRadSec[2];//-z
//    Update_Matrix[0][2]=timeDiff*gyroRadSec[1];//y
//    Update_Matrix[1][0]=timeDiff*gyroRadSec[2];//z
//    Update_Matrix[1][1]=0;
//    Update_Matrix[1][2]=-timeDiff*gyroRadSec[0];//-x
//    Update_Matrix[2][0]=-timeDiff*gyroRadSec[1];//-y
//    Update_Matrix[2][1]=timeDiff*gyroRadSec[0];//x
//    Update_Matrix[2][2]=0;
//
    //drift correction
  Update_Matrix[0][0]=0;
  Update_Matrix[0][1]=-timeDiff*Omega_Vector[2];//-z
  Update_Matrix[0][2]=timeDiff*Omega_Vector[1];//y
  Update_Matrix[1][0]=timeDiff*Omega_Vector[2];//z
  Update_Matrix[1][1]=0;
  Update_Matrix[1][2]=-timeDiff*Omega_Vector[0];//-x
  Update_Matrix[2][0]=-timeDiff*Omega_Vector[1];//-y
  Update_Matrix[2][1]=timeDiff*Omega_Vector[0];//x
  Update_Matrix[2][2]=0;

    Matrix_Multiply(Temporary_Matrix, DCM_Matrix,Update_Matrix); //a*b=c


    while (x < 3)
    {
        y= 0;
     while (y < 3)
     {
         DCM_Matrix[x][y]+=Temporary_Matrix[x][y];
         y++;
     }
     x++;
    }

}

void DCM_normalize(void)
{
  float error=0;
  float temporary[3][3];
  float renorm=0;
  boolean problem=0;

  error= -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19

  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19

  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19

  Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20

  renorm= Vector_Dot_Product(&temporary[0][0],&temporary[0][0]);
  if (renorm < 1.5625f && renorm > 0.64f)
  {
    renorm= .5 * (3-renorm);                                                 //eq.21
  }
  else if (renorm < 100.0f && renorm > 0.01f)
  {
    renorm= 1. / sqrt(renorm);
  }
  else
  {
    problem = 1;
  }

  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);
  renorm= Vector_Dot_Product(&temporary[1][0],&temporary[1][0]);
  if (renorm < 1.5625f && renorm > 0.64f)
  {
    renorm= .5 * (3-renorm);                                                 //eq.21
  }
  else if (renorm < 100.0f && renorm > 0.01f)
  {
    renorm= 1. / sqrt(renorm);

  }
  else
  {
    problem = 1;

  }

  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);
  renorm= Vector_Dot_Product(&temporary[2][0],&temporary[2][0]);
  if (renorm < 1.5625f && renorm > 0.64f)
  {
    renorm= .5 * (3-renorm);                                                 //eq.21
  }
  else if (renorm < 100.0f && renorm > 0.01f)
  {
    renorm= 1. / sqrt(renorm);
  }
  else
  {
    problem = 1;

  }
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);

  if (problem) {                // Our solution is blowing up and we will force back to initial condition.  Hope we are not upside down!
      DCM_Matrix[0][0]= 1.0f;
      DCM_Matrix[0][1]= 0.0f;
      DCM_Matrix[0][2]= 0.0f;
      DCM_Matrix[1][0]= 0.0f;
      DCM_Matrix[1][1]= 1.0f;
      DCM_Matrix[1][2]= 0.0f;
      DCM_Matrix[2][0]= 0.0f;
      DCM_Matrix[2][1]= 0.0f;
      DCM_Matrix[2][2]= 1.0f;
      problem = 0;
  }
}

void DCM_driftCorrection(float* accelVector, float scaledAccelMag, float magneticHeading)
{
    //Compensation the Roll, Pitch and Yaw drift.
    float magneticHeading_X;
    float magneticHeading_Y;
    static float Scaled_Omega_P[3];
    static float Scaled_Omega_I[3];
    float Accel_weight;
    float Integrator_magnitude;
    float tempfloat;


    //*****Roll and Pitch***************
    // Dynamic weighting of accelerometer info (reliability filter)
    // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
    Accel_weight = constrain(1 - 2*abs(1 - scaledAccelMag),0,1);


    Vector_Cross_Product(&errorRollPitch[0],&accelVector[0],&DCM_Matrix[2][0]); //adjust the ground of reference
    Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);

    Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
    Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);


    //*****YAW***************
    //Calculate Heading_X and Heading_Y
    magneticHeading_X = cos(magneticHeading);
    magneticHeading_Y = sin(magneticHeading);

    // We make the gyro YAW drift correction based on compass magnetic heading
    errorCourse=(DCM_Matrix[0][0]*magneticHeading_Y) - (DCM_Matrix[1][0]*magneticHeading_X);  //Calculating YAW error
    Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

    Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);
    Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.

    Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);
    Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I


    //  Here we will place a limit on the integrator so that the integrator cannot ever exceed half the saturation limit of the gyros
    Integrator_magnitude = sqrt(Vector_Dot_Product(Omega_I,Omega_I));
    if (Integrator_magnitude > ToRad(300)) {
    Vector_Scale(Omega_I,Omega_I,0.5f*ToRad(300)/Integrator_magnitude);
    }
}

void DCM_eulerAngle(float* roll, float* pitch, float* yaw)
{
    pitch[0] = -asin(DCM_Matrix[2][0]);
    roll[0] = atan2(DCM_Matrix[2][1],DCM_Matrix[2][2]);
    yaw[0] = atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]);

//    roll = atan2(Accel_Vector[1],Accel_Vector[2]);    // atan2(acc_y,acc_z)
//    pitch = -asin((Accel_Vector[0])/(double)GRAVITY); // asin(acc_x)
//    yaw = 0;
}
