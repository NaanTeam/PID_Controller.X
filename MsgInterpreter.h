/**
* @file: MsgInterpreter.h
* @brief: Functions for interpreting protocol messages and executing commands based
 * on them.
*
* @author: Connor Martin
* @date: Jan 31, 2014
*
* @preconditions:
* @device:
*      -PIC32MX695F512L
*
* @remarks:
*  --Message Protocol--
 * Format:
 * <msg> [Message length byte] [protocol command] [command instruction] [...] <\msg>
 * 
 * Example: 
 * //This reads the accelereometer's x, y, z data.
 * {5, 0x02, 0x10, 0x11, 0x12}
 *
 * Note: The message length byte should count itself. Also when passing the message
 *  to the MsgInterpreter_interpret function, the message length byte should be
 *  excluded.
 * 
*/

#ifndef MSGINTERPRETER_H
#define	MSGINTERPRETER_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <plib.h>

#include "hardwareProfile.h"
#include "ADXL362.h"
#include "L3G4200D.h"
#include "HMC5883L.h"
#include "FIFOUART1.h"
#include "FIFOUART4.h" 
#include "DCM.h"
#include "OrientationLoop.h"
#include "MotorCtrl.h"
#include "MotorPID.h"
#include "RcRx.h"
#include "battery_voltage.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************
/** Controls the max size of message that can be sent.*/
#define SERIALCOMM_MAX_TX_BUFFER 1024

//******************************************************************************
//Device #defines
//******************************************************************************
//Message protocol commands

#define SERIALCOMM_START_TOKEN      0x01
#define SERIALCOMM_READ_REGISTER    0x02
#define SERIALCOMM_WRITE_REGISTER   0x03
#define SERIALCOMM_MOTOR1_ON        0x10
#define SERIALCOMM_END_TOKEN        0xFF

//Message protocol command instruction

#define SERIALCOMM_REGISTER_XAcceleration       0x10
#define SERIALCOMM_REGISTER_YAcceleration       0x11
#define SERIALCOMM_REGISTER_ZAcceleration       0x12
#define SERIALCOMM_REGISTER_AcceloTemperature   0x13
#define SERIALCOMM_REGISTER_XAcceleration_Raw       0x14
#define SERIALCOMM_REGISTER_YAcceleration_Raw       0x15
#define SERIALCOMM_REGISTER_ZAcceleration_Raw       0x16
#define SERIALCOMM_REGISTER_AcceloTemperature_Raw   0x17
#define SERIALCOMM_REGISTER_XAcceleration_Raw_Avg       0x18
#define SERIALCOMM_REGISTER_YAcceleration_Raw_Avg       0x19
#define SERIALCOMM_REGISTER_ZAcceleration_Raw_Avg       0x1A

#define SERIALCOMM_REGISTER_XAngularRate        0x20
#define SERIALCOMM_REGISTER_YAngularRate        0x21
#define SERIALCOMM_REGISTER_ZAngularRate        0x22
#define SERIALCOMM_REGISTER_GyroTemperature     0x23
#define SERIALCOMM_REGISTER_XAngularRate_Raw        0x24
#define SERIALCOMM_REGISTER_YAngularRate_Raw        0x25
#define SERIALCOMM_REGISTER_ZAngularRate_Raw        0x26
#define SERIALCOMM_REGISTER_GyroTemperature_Raw     0x27
#define SERIALCOMM_REGISTER_XAngularRate_Raw_Avg    0x28
#define SERIALCOMM_REGISTER_YAngularRate_Raw_Avg    0x29
#define SERIALCOMM_REGISTER_ZAngularRate_Raw_Avg    0x2A

#define SERIALCOMM_REGISTER_XMagneticVector     0x30
#define SERIALCOMM_REGISTER_YMagneticVector     0x31
#define SERIALCOMM_REGISTER_ZMagneticVector     0x32
#define SERIALCOMM_REGISTER_XMagneticVector_Raw     0x33
#define SERIALCOMM_REGISTER_YMagneticVector_Raw     0x34
#define SERIALCOMM_REGISTER_ZMagneticVector_Raw     0x35

#define SERIALCOMM_REGISTER_Pitch               0x40
#define SERIALCOMM_REGISTER_Yaw                 0x41
#define SERIALCOMM_REGISTER_Roll                0x42
#define SERIALCOMM_REGISTER_ScaledYaw           0x43
#define SERIALCOMM_REGISTER_DesiredPitch        0x44
#define SERIALCOMM_REGISTER_DesiredYaw          0x45
#define SERIALCOMM_REGISTER_DesiredRoll         0x46
#define SERIALCOMM_REGISTER_DesiredThrottle     0x47

#define SERIALCOMM_REGISTER_Motor1              0x50
#define SERIALCOMM_REGISTER_Motor2              0x51
#define SERIALCOMM_REGISTER_Motor3              0x52
#define SERIALCOMM_REGISTER_Motor4              0x53
#define SERIALCOMM_CONFIG_IdleMotor             0x54
#define SERIALCOMM_CONFIG_ShutoffMotor          0x55


#define SERIALCOMM_REGISTER_VBatt             0x60
    


#define SERIALCOMM_CONFIG_Filter_RollPitch_P        0x80
#define SERIALCOMM_CONFIG_Filter_RollPitch_I        0x81
#define SERIALCOMM_CONFIG_Filter_Yaw_P              0x82
#define SERIALCOMM_CONFIG_Filter_Yaw_I              0x83
#define SERIALCOMM_CONFIG_Filter_Reset              0x84

#define SERIALCOMM_CONFIG_PID_Roll_P        0x90
#define SERIALCOMM_CONFIG_PID_Roll_I        0x91
#define SERIALCOMM_CONFIG_PID_Roll_D        0x92
#define SERIALCOMM_CONFIG_PID_Pitch_P       0x93
#define SERIALCOMM_CONFIG_PID_Pitch_I       0x94
#define SERIALCOMM_CONFIG_PID_Pitch_D       0x95
#define SERIALCOMM_CONFIG_PID_Yaw_P         0x96
#define SERIALCOMM_CONFIG_PID_Yaw_I         0x97
#define SERIALCOMM_CONFIG_PID_Yaw_D         0x98
#define SERIALCOMM_CONFIG_PID_Reset         0x99

#define SERIALCOMM_CONFIG_SoftRestart         0xA0





//******************************************************************************
//Function Prototypes
//******************************************************************************
/**
 * Interprets a message and executes commands based on it. The protocol is 
 * described in MSGInterpreter.h
 * @param message: An array of messages. Note: the message length byte is excluded.
 * @param length: The length of the array.
 * @return This function returns 0 currently.
 */
int MsgInterpreter_interpret(UINT8 message[], UINT16 length);



#ifdef	__cplusplus
}
#endif

#endif	/* MSGINTERPRETER_H */

