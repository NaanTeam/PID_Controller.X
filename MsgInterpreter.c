
#include "MsgInterpreter.h"

//******************************************************************************
// Local Variables and Typedefs
//******************************************************************************
UINT16 MsgInterpreter_Index = 0;
UINT16 MsgInterpreter_Length = 0;

//******************************************************************************
//Local Function Declarations
//******************************************************************************
inline int MsgInterpreter_interpret_readRegisters(UINT8 message[]);
inline int MsgInterpreter_interpret_writeRegisters(UINT8 message[]);


//******************************************************************************
//Local Function Definitions
//******************************************************************************
inline int MsgInterpreter_interpret_readRegisters(UINT8 message[])
{
    UINT8 * temp;
    UINT8 buffer[SERIALCOMM_MAX_TX_BUFFER];
    int buffer_len = 0;
    buffer[buffer_len++] = 0;

    while(MsgInterpreter_Index < MsgInterpreter_Length)
    {

        switch(message[MsgInterpreter_Index++])
        {
            default:
                return;
            case SERIALCOMM_END_TOKEN:
                if (MsgInterpreter_Index == MsgInterpreter_Length) //Should be last msg
                {
                    break;
                }
                else
                {
                    return; //error
                }
           /******************************************************************************
             * Sensor - Accelerometer
            ******************************************************************************/
            case SERIALCOMM_REGISTER_XAcceleration:
                temp = (UINT8 *)(&ADXL362_XAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAcceleration:
                temp = (UINT8 *)(&ADXL362_YAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAcceleration:
                temp = (UINT8 *)(&ADXL362_ZAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_AcceloTemperature:
                temp = (UINT8 *)(&ADXL362_Temperature);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            case SERIALCOMM_REGISTER_XAcceleration_Raw:
                temp = (UINT8 *)(&ADXL362_XAcceleration_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_YAcceleration_Raw:
                temp = (UINT8 *)(&ADXL362_YAcceleration_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_ZAcceleration_Raw:
                temp = (UINT8 *)(&ADXL362_ZAcceleration_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_AcceloTemperature_Raw:
                temp = (UINT8 *)(&ADXL362_Temperature_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_XAcceleration_Raw_Avg:
                temp = (UINT8 *)(&ADXL362_XAcceleration_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAcceleration_Raw_Avg:
                temp = (UINT8 *)(&ADXL362_YAcceleration_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAcceleration_Raw_Avg:
                temp = (UINT8 *)(&ADXL362_ZAcceleration_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

           /******************************************************************************
             * Sensor - Gyroscope
            ******************************************************************************/
            case SERIALCOMM_REGISTER_XAngularRate:
                temp = (UINT8 *)(&L3G4200D_XAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAngularRate:
                temp = (UINT8 *)(&L3G4200D_YAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAngularRate:
                temp = (UINT8 *)(&L3G4200D_ZAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_GyroTemperature:
                temp = (UINT8 *)(&L3G4200D_Temperature);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            case SERIALCOMM_REGISTER_XAngularRate_Raw:
                temp = (UINT8 *)(&L3G4200D_XAngularRate_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_YAngularRate_Raw:
                temp = (UINT8 *)(&L3G4200D_YAngularRate_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_ZAngularRate_Raw:
                temp = (UINT8 *)(&L3G4200D_ZAngularRate_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_GyroTemperature_Raw:
                temp = (UINT8 *)(&L3G4200D_Temperature_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;

            case SERIALCOMM_REGISTER_XAngularRate_Raw_Avg:
                temp = (UINT8 *)(&L3G4200D_XAngularRate_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAngularRate_Raw_Avg:
                temp = (UINT8 *)(&L3G4200D_YAngularRate_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAngularRate_Raw_Avg:
                temp = (UINT8 *)(&L3G4200D_ZAngularRate_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

           /******************************************************************************
             * Sensor - Magtometer
            ******************************************************************************/
            case SERIALCOMM_REGISTER_XMagneticVector:
                temp = (UINT8 *)(&HMC5883L_XMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YMagneticVector:
                temp = (UINT8 *)(&HMC5883L_YMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZMagneticVector:
                temp = (UINT8 *)(&HMC5883L_ZMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

           case SERIALCOMM_REGISTER_XMagneticVector_Raw:
                temp = (UINT8 *)(&HMC5883L_XMagneticVector_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_YMagneticVector_Raw:
                temp = (UINT8 *)(&HMC5883L_YMagneticVector_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_ZMagneticVector_Raw:
                temp = (UINT8 *)(&HMC5883L_ZMagneticVector_Raw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;

           /******************************************************************************
             * Perceived - Roll Pitch Yaw
            ******************************************************************************/
            case SERIALCOMM_REGISTER_Pitch:
                temp = (UINT8 *)(&OrientationLoop_Pitch);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Yaw:
                temp = (UINT8 *)(&scaledOrientationYaw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Roll:
                temp = (UINT8 *)(&OrientationLoop_Roll);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ScaledYaw:
                temp = (UINT8 *)(&scaledOrientationYaw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

           /******************************************************************************
             * Desired - Roll Pitch Yaw
            ******************************************************************************/
            case SERIALCOMM_REGISTER_DesiredPitch:
                temp = (UINT8 *)(&IC_PITCH);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_DesiredYaw:
                temp = (UINT8 *)(&Calibration_Yaw_Zero_Point);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_DesiredRoll:
                temp = (UINT8 *)(&IC_ROLL);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_DesiredThrottle:
                temp = (UINT8 *)(&IC_THRO);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;


           /******************************************************************************
             * Motors
            ******************************************************************************/
            case SERIALCOMM_REGISTER_Motor1:
                temp = (UINT8 *)(&MotorCtrl_OC_ONE_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor2:
                temp = (UINT8 *)(&MotorCtrl_OC_TWO_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor3:
                temp = (UINT8 *)(&MotorCtrl_OC_THREE_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor4:
                temp = (UINT8 *)(&MotorCtrl_OC_FOUR_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            /******************************************************************************
             * Filter - Constants
            ******************************************************************************/
            case SERIALCOMM_CONFIG_Filter_RollPitch_P:
                temp = (UINT8 *)(&DCM_RollPitch_Kp);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_Filter_RollPitch_I:
                temp = (UINT8 *)(&DCM_RollPitch_Ki);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_Filter_Yaw_P:
                temp = (UINT8 *)(&DCM_Yaw_Kp);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_Filter_Yaw_I:
                temp = (UINT8 *)(&DCM_Yaw_Ki);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            /******************************************************************************
             * Misc
            ******************************************************************************/
            case SERIALCOMM_REGISTER_VBatt:
                temp = (UINT8 *)(&v_batt);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

           /******************************************************************************
             * PID - Constants
            ******************************************************************************/
          case SERIALCOMM_CONFIG_PID_Roll_P:
                temp = (UINT8 *)(&pRollConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Roll_I:
                temp = (UINT8 *)(&iRollConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Roll_D:
                temp = (UINT8 *)(&dRollConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

          case SERIALCOMM_CONFIG_PID_Pitch_P:
                temp = (UINT8 *)(&pPitchConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Pitch_I:
                temp = (UINT8 *)(&iPitchConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Pitch_D:
                temp = (UINT8 *)(&dPitchConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

          case SERIALCOMM_CONFIG_PID_Yaw_P:
                temp = (UINT8 *)(&pYawConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Yaw_I:
                temp = (UINT8 *)(&iYawConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
          case SERIALCOMM_CONFIG_PID_Yaw_D:
                temp = (UINT8 *)(&dYawConst);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

        }
    }

    buffer[0] = buffer_len;
    FIFOUART1_pushTxQueue(buffer, buffer_len);
    FIFOUART4_pushTxQueue(buffer, buffer_len);  //Maher
    return 0;
}

inline int MsgInterpreter_interpret_writeRegisters(UINT8 message[])
{
    UINT8 * temp;
    UINT8 buffer[32];

    while(MsgInterpreter_Index < MsgInterpreter_Length)
    {
        switch (message[MsgInterpreter_Index++])
        {
          /******************************************************************************
             * Motors
            ******************************************************************************/
            case SERIALCOMM_CONFIG_IdleMotor:
                MotorCtrl_idleMotors();
                break;
            case SERIALCOMM_CONFIG_ShutoffMotor:
                MotorCtrl_shutOffMotors();
                break;

            /******************************************************************************
             * Filter
            ******************************************************************************/
            case SERIALCOMM_CONFIG_Filter_RollPitch_P:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_RollPitch_Kp = *((float*)buffer);
                break;
            case SERIALCOMM_CONFIG_Filter_RollPitch_I:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_RollPitch_Ki = *((float*)buffer);

                break;
            case SERIALCOMM_CONFIG_Filter_Yaw_P:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_Yaw_Kp = *((float*)buffer);
                break;
            case SERIALCOMM_CONFIG_Filter_Yaw_I:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_Yaw_Ki = *((float*)buffer);
                break;
            case SERIALCOMM_CONFIG_Filter_Reset:
                OrientationLoop_reset();
                DCM_reset();
                break;

           /******************************************************************************
             * PID
            ******************************************************************************/
         case SERIALCOMM_CONFIG_PID_Reset:
                MotorPID_resetPID();
                break;

          case SERIALCOMM_CONFIG_PID_Roll_P:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                pRollConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Roll_I:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                iRollConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Roll_D:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                dRollConst = *((float*)buffer);
                break;

          case SERIALCOMM_CONFIG_PID_Pitch_P:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                pPitchConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Pitch_I:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                iPitchConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Pitch_D:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                dPitchConst = *((float*)buffer);
                break;

          case SERIALCOMM_CONFIG_PID_Yaw_P:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                pYawConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Yaw_I:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                iYawConst = *((float*)buffer);
                break;
          case SERIALCOMM_CONFIG_PID_Yaw_D:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                dYawConst = *((float*)buffer);
                break;
        }
    }

    return 0;
}


//******************************************************************************
//Public Function Definitions
//******************************************************************************

int MsgInterpreter_interpret(UINT8 message[], UINT16 length)
{
    MsgInterpreter_Index = 0;
    MsgInterpreter_Length = length;
    
    switch (message[MsgInterpreter_Index++])
    {
        case SERIALCOMM_READ_REGISTER:
            MsgInterpreter_interpret_readRegisters(message);
            break;
        case SERIALCOMM_WRITE_REGISTER:
            MsgInterpreter_interpret_writeRegisters(message);
            break;
        default:
            break;
    }

    return 0;
}
