/**
* @file: ADXL362.h
* @brief: A set of function for interfacing with the L3G4200D Gyroscope
*
* @author: Connor Martin
* @date: Oct 23, 2013
*
* @preconditions:
*  -FIFOSPI2 is intialized.
* @device:
*      -PIC32MX695F512L
*
* @remarks:
*
*/

#ifndef L3G4200D_H
#define	L3G4200D_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "FIFOSPI2.h"
#include "hardwareProfile.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************
/**
 * Controls the slave slect line that will be used for this device.
 * @remarks Modify this further in FIFOSPI2.c
 */
#define L3G4200D_SLAVE_SELECT_LINE 2

//******************************************************************************
//Device #defines
//******************************************************************************
/**Device identification registe.r*/
#define L3G4200D_Reg_WHOAMI  0x0F


    /**Control register one.*/
#define L3G4200D_Reg_CTRLREG1  0x20
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


    /**Control register three.*/
#define L3G4200D_Reg_CTRLREG3  0x22
    /**Interrupt enable on INT1 pin. Default value 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I1_Int1 7
    /**Boot status available on INT1. Default value 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I1_Boot 6
    /**Interrupt active configuration on INT1. Default value 0. (0: High; 1:Low)*/
#define L3G4200D_RegBit_H_Lactive 5
    /**Push- Pull / Open drain. Default value: 0. (0: Push- Pull; 1: Open drain)*/
#define L3G4200D_RegBit_PP_OD 4
    /**Date Ready on DRDY/INT2. Default value 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I2_DRDY 3
    /**FIFO Watermark interrupt on DRDY/INT2. Default value: 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I2_WTM  2
    /**FIFO Overrun interrupt on DRDY/INT2 Default value: 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I2_ORun 1
    /**FIFO Empty interrupt on DRDY/INT2. Default value: 0. (0: Disable; 1: Enable)*/
#define L3G4200D_RegBit_I2_Empty 1


    /**Control register four.*/
#define L3G4200D_Reg_CTRLREG4  0x23
    /**Block Data Update. Default value: 0. (0: continous update; 1: output registers not updated until MSB and LSB reading)*/
#define L3G4200D_RegBit_BDU 7
    /**Big/Little Endian Data Selection. Default value 0. (0: Data LSB @ lower address; 1: Data MSB @ lower address)*/
#define L3G4200D_RegBit_BLE 6
    /**Full Scale selection. Default value: 00. (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)*/
#define L3G4200D_RegBit_FS 4
    /**Self Test Enable. Default value: 00. (00: Self Test Disabled;*/
#define L3G4200D_RegBit_ST 1
    /**SPI Serial Interface Mode selection. Default value: 0. (0: 4-wire interface; 1: 3-wire interface).*/
#define L3G4200D_RegBit_SIM 0


    /**Control register five.*/
#define L3G4200D_Reg_CTRLREG5  0x24
    /**Reboot memory content. Default value: 0. (0: normal mode; 1: reboot memory content)*/
#define L3G4200D_RegBit_BOOT 7
    /**FIFO enable. Default value: 0. (0: FIFO disable; 1: FIFO Enable) */
#define L3G4200D_RegBit_FIFO_EN 6
    /**High Pass filter Enable. Default value: 0. (0: HPF disabled; 1: HPF enabled.)*/
#define L3G4200D_RegBit_HPen 4
    /**INT1 selection configuration. Default value: 0*/
#define L3G4200D_RegBit_INT1_Sel 2
    /**Out selection configuration. Default value: 0*/
#define L3G4200D_RegBit_Out_Sel 0


    /**Reference value register for Interrupt generation.*/
#define L3G4200D_Reg_REF  0x25
    /**Temperature data register.*/
#define L3G4200D_Reg_OUTTEMP  0x26
#define L3G4200D_Reg_STATUSREG  0x27
    /** Lower X-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTXL  0x28
    /** Higher X-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTXH  0x29
    /** Lower Y-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTYL  0x2A
    /** Higher Y-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTYH  0x2B
    /** Lower Z-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTZL  0x2C
    /** Higher Z-axis angular rate data. The value is expressed as two?s complement.*/
#define L3G4200D_Reg_OUTZH  0x2D
    /**FIFO buffer control register.*/
#define L3G4200D_Reg_FIFOCTRLREG  0x2E
//... Additional interrupt registers that I didn't include.

//******************************************************************************
//Public Variables Declarations
//******************************************************************************
/** Contains the L3G4200D's X angular rate reading in degrees per second*/
extern float L3G4200D_XAngularRate;
/** Contains the L3G4200D's Y angular rate reading in degrees per second*/
extern float L3G4200D_YAngularRate;
/** Contains the L3G4200D's Z angular rate reading in degrees per second*/
extern float L3G4200D_ZAngularRate;
/** Contains the L3G4200D's temperature reading in celcius*/
extern float L3G4200D_Temperature;

/** Contains the L3G4200D's raw X angular rate reading*/
extern int16 L3G4200D_XAngularRate_Raw;
/** Contains the L3G4200D's raw Y angular rate reading*/
extern int16 L3G4200D_YAngularRate_Raw;
/** Contains the L3G4200D's raw Z angular rate reading*/
extern int16 L3G4200D_ZAngularRate_Raw;
/** Contains the L3G4200D's raw temperature reading*/
extern int16 L3G4200D_Temperature_Raw;

/** Contains the L3G4200D's running average of raw X angular rate readings*/
extern int16 L3G4200D_XAngularRate_Raw_Avg;

//******************************************************************************
//Function Prototypes
//******************************************************************************

/**
 * Sets up and turns on the L3G4200D for measurement reading.
 * @return -1 if the device is not commuicating properly.
 */
int L3G4200D_startMeasurements();

/**
 * Queues a write-to-register command for the L3G4200D in the FIFOSPI.c buffer.
 * @param reg: The register to write to.
 * @param value: The value to write to the register.
 */
void L3G4200D_pushWriteRegister(uint8 reg, uint8 value);
/**
 * Queues a read-register command for the L3G4200D in the FIFOSPI buffer.
 * @param reg: The register to write to.
 */
void L3G4200D_pushReadRegister(uint8 reg);

/**
 * Queues a write-register command for the L3G4200D in the FIFOSPI buffer
 * then waits for the transmission to complete.
 * @param reg: The register to write to.
 * @param value: The value to write to the register.
 */
void L3G4200D_writeRegister_Blocking(uint8 reg, uint8 value);
/**
 * Queues a read-register command for the L3G4200D in the FIFOSPI buffer
 * then waits for the transmission to complete.
 * @param reg: The register to write to.
 * @param value: The value to write to the register.
 * @return The read unsigned character.
 */
uint8 L3G4200D_readRegister_Blocking(uint8 reg);

/**
 * Queues a read X, Y, Z and temperature data register command for the
 * L3G4200D in the FIFOSPI buffer.
 */
void L3G4200D_pushReadXYZT();
/**
 * Pops the read X, Y, Z and temperature data register command for
 * the L3G4200D from the FIFOSPI buffer into the global raw variables.
 */
void L3G4200D_popXYZT();

/**
 * Converts from raw gyro XYZT values to global variables.
 */
void L3G4200D_convertXYZT();


#ifdef	__cplusplus
}
#endif

#endif	/* L3G4200D_H */

