
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
                return;

            //Accelerometer
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


            //Gyroscope
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
                break;
            case SERIALCOMM_REGISTER_YAngularRate_Raw_Avg:
                temp = (UINT8 *)(&L3G4200D_YAngularRate_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;
            case SERIALCOMM_REGISTER_ZAngularRate_Raw_Avg:
                temp = (UINT8 *)(&L3G4200D_ZAngularRate_Raw_Avg);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                break;


            //3-axis Compass
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



            case SERIALCOMM_REGISTER_Pitch:
                temp = (UINT8 *)(&Orientation_Pitch);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Yaw:
                temp = (UINT8 *)(&Orientation_Yaw);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Roll:
                temp = (UINT8 *)(&Orientation_Roll);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            case SERIALCOMM_REGISTER_Motor1:
                temp = (UINT8 *)(&OC_ONE_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor2:
                temp = (UINT8 *)(&OC_TWO_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor3:
                temp = (UINT8 *)(&OC_THREE_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_Motor4:
                temp = (UINT8 *)(&OC_FOUR_PC);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;


          case SERIALCOMM_CONFIG_RollPitch_Kp:
                temp = (UINT8 *)(&DCM_RollPitch_Kp);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_RollPitch_Ki:
                temp = (UINT8 *)(&DCM_RollPitch_Ki);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_Yaw_Kp:
                temp = (UINT8 *)(&DCM_Yaw_Kp);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_CONFIG_Yaw_Ki:
                temp = (UINT8 *)(&DCM_Yaw_Ki);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
        }
    }

    buffer[0] = buffer_len;
    FIFOUART1_pushTxQueue(buffer, buffer_len);
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
            case SERIALCOMM_CONFIG_RollPitch_Kp:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_RollPitch_Kp = *((float*)buffer);
                break;
            case SERIALCOMM_CONFIG_RollPitch_Ki:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_RollPitch_Ki = *((float*)buffer);

                break;
            case SERIALCOMM_CONFIG_Yaw_Kp:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_Yaw_Kp = *((float*)buffer);
                break;
            case SERIALCOMM_CONFIG_Yaw_Ki:
                buffer[0] = message[MsgInterpreter_Index++];
                buffer[1] = message[MsgInterpreter_Index++];
                buffer[2] = message[MsgInterpreter_Index++];
                buffer[3] = message[MsgInterpreter_Index++];
                DCM_Yaw_Ki = *((float*)buffer);
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
