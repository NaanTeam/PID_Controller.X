
#include "HMC5883L.h"

//******************************************************************************
//Public Variable Declarations
//******************************************************************************

float HMC5883L_XMagneticVector = 0;
float HMC5883L_ZMagneticVector = 0;
float HMC5883L_YMagneticVector = 0;
int16 HMC5883L_XMagneticVector_Raw = 0;
int16 HMC5883L_ZMagneticVector_Raw = 0;
int16 HMC5883L_YMagneticVector_Raw = 0;


//******************************************************************************
//Public Function Definitions
//******************************************************************************

void HMC5883L_startMeasurements()
{
    int i = 0;
    uint8 buffer[10];

    //Config Register A
    //buffer[0] = 0x74; //111 0100  //30hz refresh rate
    buffer[0] = 0x00 |
            (0 << HMC5883L_RegBit_CRA7) |   //Reserver
            (0 << HMC5883L_RegBit_MA0) |    //1 sample averaged
            (4 << HMC5883L_RegBit_DO0) |    //75Hz Data Output rate
            (0 << HMC5883L_RegBit_MS0);     //Normal Measurement Mode
    //Config Register B
   // buffer[1] = 0xA0;
    buffer[1] = 0x00 |
            (1 << HMC5883L_RegBit_GN0);     //Recommended ± 1.3 Ga. Gain: 1090 (default). Resoltuon 0.92mG/LSB

    //Mode Register
    buffer[2] = 0x00 |
            (0 << HMC5883L_RegBit_HS) | //Disabled High Speed I2C (3400kHz).
            (0 << HMC5883L_RegBit_MD0); //Continuous-Measurement Mode.

    FIFOI2C2_pushTxQueue_writeDeviceRegisters(HMC5883L_I2C_DEVICE_NUMBER, HMC5883L_Reg_CFGA, buffer, 3);

}


void HMC5883L_pushReadXZY()
{
    FIFOI2C2_pushTxQueue_readDeviceRegisters(HMC5883L_I2C_DEVICE_NUMBER, HMC5883L_Reg_XMSB, 6);
}


void HMC5883L_popXZY()
{
    uint8 x_msb = 0, x_lsb = 0;
    uint8 z_msb = 0, z_lsb = 0;
    uint8 y_msb = 0, y_lsb = 0;
    short x_tmp = 0, z_tmp = 0, y_tmp= 0;

    x_msb = FIFOI2C2_popRxQueue(0).rx_byte;
    x_lsb = FIFOI2C2_popRxQueue(0).rx_byte;

    z_msb = FIFOI2C2_popRxQueue(0).rx_byte;
    z_lsb = FIFOI2C2_popRxQueue(0).rx_byte;

    y_msb = FIFOI2C2_popRxQueue(0).rx_byte;
    y_lsb = FIFOI2C2_popRxQueue(0).rx_byte;

    x_tmp = (x_msb << 8) | x_lsb;
    z_tmp = (z_msb << 8) | z_lsb;
    y_tmp = (y_msb << 8) | y_lsb;

    HMC5883L_XMagneticVector_Raw = x_tmp;
    HMC5883L_YMagneticVector_Raw = -1*y_tmp;
    HMC5883L_ZMagneticVector_Raw = -1*z_tmp;
}

void HMC5883L_convertXYZ()
{
    float gauss_per_LSB = 0;

    //Based on devie 'Mode Register.' Refer to HMC5883L data sheet.
    gauss_per_LSB = 0.92e-3;

    HMC5883L_XMagneticVector = gauss_per_LSB * (float)HMC5883L_XMagneticVector_Raw;
    HMC5883L_ZMagneticVector = gauss_per_LSB * (float)HMC5883L_ZMagneticVector_Raw;
    HMC5883L_YMagneticVector = gauss_per_LSB * (float)HMC5883L_YMagneticVector_Raw;
}

