/**
 * @file: HMC5883L.h
 * @brief: A set of function for interfacing with the HMC5883L Compass.
 *
 * @author: Connor Martin
 * @date: Oct 23, 2013
 *
 * @preconditions:
  *  -FIFOI2C2 is intialized.
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks:
 *
 */

#ifndef HMC5883L_H
#define	HMC5883L_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "FIFOI2C2.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************
/**
 * Controls the slave slect line that will be used for this device.
 * @remarks Modify this further in FIFOSPI2.c
 */
#define HMC5883L_I2C_DEVICE_NUMBER 0

//******************************************************************************
//Device #defines
//******************************************************************************
/**Device identification registe.r*/
#define L3G4200D_Reg_WHOAMI  0x0F


    /**Configuration Register A.*/
#define HMC5883L_Reg_CFGA  0x00
    /**Reserved. Must be 0.*/
    #define HMC5883L_RegBit_CRA7 7
    /**Select number of samples averaged (1 to 8) per measurement output. (00 = 1(Default); 01 = 2; 10 = 4; 11 = 8)*/
    #define HMC5883L_RegBit_MA0 5
    /**Data Output Rate Bits. These bits set the rate at which data is written to all three data output registers.*/
    #define HMC5883L_RegBit_DO0 2
    /**Measurement Configuration Bits. These bits define the measurement flow of the device,
    * specifically whether or not to incorporate an applied bias into the measurement.*/
    #define HMC5883L_RegBit_MS0 0


    /**Configuration Register B.*/
#define HMC5883L_Reg_CFGB   0x01
    /**Gain Configuration Bits. These bits configure the gain for the device. The gain configuration is common for all channels.*/
    #define HMC5883L_RegBit_GN0 5


    /**Mode Register.*/
#define HMC5883L_Reg_MODE   0x02
    /**Set this pin to enable High Speed I2C, 3400kHz.*/
    #define HMC5883L_RegBit_HS 7
    /**Mode Select Bits. These bits select the operation mode of this device.*/
    #define HMC5883L_RegBit_MD0 0

    /**Data Output X MSB Register.*/
#define HMC5883L_Reg_XMSB   0x03
    /**Data Output X LSB Register.*/
#define HMC5883L_Reg_XLSB   0x04
    /**Data Output Z MSB Register.*/
#define HMC5883L_Reg_ZMSB   0x05
    /**Data Output Z LSB Register.*/
#define HMC5883L_Reg_ZLSB   0x06
    /**Data Output Y MSB Register.*/
#define HMC5883L_Reg_YMSB   0x07
    /**Data Output Y LSB Register.*/
#define HMC5883L_Reg_YLSB   0x08
    /**Status Register.*/
#define HMC5883L_Reg_STATUS 0x09
    /**Identification Register A.*/
#define HMC5883L_Reg_IDA    0x0A
    /**Identification Register B.*/
#define HMC5883L_Reg_IDB    0x0B
    /**Identification Register C.*/
#define HMC5883L_Reg_IDC    0x0C




    /**Output Data Rate selection.*/
#define L3G4200D_RegBit_DR 6 //2 bits
    /**Bandwidth selection.*/
#define L3G4200D_RegBit_BW 4 //2 bits
    /**Power down mode enable.*/
#define L3G4200D_RegBit_PD 3
    /**Z axis enable.*/
#define L3G4200D_RegBit_Zen 2
    /**Y axis enable.*/
#define L3G4200D_RegBit_Yen 1
    /**x axis enable.*/
#define L3G4200D_RegBit_Xen 0


    /**Control register two.*/
#define L3G4200D_Reg_CTRLREG2  0x21
    /**High Pass filter Mode Selection.*/
#define L3G4200D_RegBit_HPM 4
    /**High Pass filter Cut Off frequency selection.*/
#define L3G4200D_RegBit_HPCF 0


//******************************************************************************
//Public Variables Declarations
//******************************************************************************
/** Contains the HMC5883L's X magnetic reading in gauss*/
extern float HMC5883L_XMagneticVector;
/** Contains the HMC5883L's Z magnetic reading in gauss*/
extern float HMC5883L_ZMagneticVector;
/** Contains the HMC5883L's Y magnetic reading in gauss*/
extern float HMC5883L_YMagneticVector;

/** Contains the HMC5883L's raw X magnetic reading*/
extern int16 HMC5883L_XMagneticVector_Raw;
/** Contains the HMC5883L's raw Z magnetic reading*/
extern int16 HMC5883L_ZMagneticVector_Raw;
/** Contains the HMC5883L's raw Y magnetic reading*/
extern int16 HMC5883L_YMagneticVector_Raw;


//******************************************************************************
//Function Prototypes
//******************************************************************************
/**
 * Turns on the HMC5883L device and has it begin measuring data.
 */
void HMC5883L_startMeasurements();
/**
 * Pushes a request to read X, Y and Z data from an HMC5883L onto
 * the SPI2's TX buffer.
 */
void HMC5883L_pushReadXZY();
/**
 * Pops X, Y, and Z values off the SPI2 stack and stores them
 * in the global raw HMC5883L variables.
 */
void HMC5883L_popXZY();
/**
 * Converts the global raw HMC5883L variables into the global float HMC5883L variables.
 */
void HMC5883L_convertXYZ();




#ifdef	__cplusplus
}
#endif

#endif	/* HMC5883L_H */

