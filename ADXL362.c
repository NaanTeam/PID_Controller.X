
#include "ADXL362.h"

//******************************************************************************
//Public Variable Declarations
//******************************************************************************
float ADXL362_XAcceleration = 0;
float ADXL362_YAcceleration = 0;
float ADXL362_ZAcceleration = 0;
float ADXL362_Temperature = 0;

int16 ADXL362_XAcceleration_Raw = 0;
int16 ADXL362_YAcceleration_Raw = 0;
int16 ADXL362_ZAcceleration_Raw = 0;
int16 ADXL362_Temperature_Raw = 0;


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void ADXL362_startMeasurements()
{
    int i = 0;
    uint8 func_rslt, read_rslt1;
    uint8 command[3];


    //Reset the Device
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_SOFT_RESET;
    command[2] = ADXL362_SOFT_RESET_KEY; //Represents the letter r
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
//    //Wait for the device to reset
//    while (i < (1000))
//    {
//        i++;
//    }
//    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
//    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
//    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);


    //Configure Activity Threshold 8 LSB, 3 MSB
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_THRESH_ACTL;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_THRESH_ACTH;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Activity Time Threshold 8 LSB
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_TIME_ACT;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Inactivity Threshold 8 LSB, 3 MSB
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_THRESH_INACTL;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_THRESH_INACTH;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Inactivity Time 8 LSB, 8 MSB
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_TIME_INACTL;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_TIME_INACTH;
    command[2] = 0;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Activity Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_ACT_INACT_CTL;
    command[2] = 0x00 |
            ADXL362_ACT_DISABLE | ADXL362_ACT_DC | ADXL362_INACT_DISABLE |
            ADXL362_INACT_DC | ADXL362_ACT_INACT_DEFAULT;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure FIFIO Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_FIFO_CONTROL;
    command[2] = 0x00 |
            ADXL362_FIFO_MODE_OFF;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    //Configure FIFIO Samples Register (max # of values to store in FIFO)
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_FIFO_SAMPLES;
    command[2] = 0x80;
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure INT1 Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_INTMAP1;
    command[2] = 0x00 ;
//            (ADXL362_INT_DATA_READY) | (ADXL362_INT_FIFO_READY) | 
//            (ADXL362_INT_FIFO_WATERMARK) | (ADXL362_INT_FIFO_OVERRUN) | 
//            (ADXL362_INT_ACT) | (ADXL362_INT_INACT) | 
//            (ADXL362_INT_AWAKE) | (ADXL362_INT_LOW);
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    //Configure INT2 Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_INTMAP2;
    command[2] = 0x00 ;
//            (ADXL362_INT_DATA_READY) | (ADXL362_INT_FIFO_READY) | 
//            (ADXL362_INT_FIFO_WATERMARK) | (ADXL362_INT_FIFO_OVERRUN) | 
//            (ADXL362_INT_ACT) | (ADXL362_INT_INACT) | 
//            (ADXL362_INT_AWAKE) | (ADXL362_INT_LOW);
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Filter Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_FILTER_CTL;
    command[2] = 0x00 |
            ADXL362_RANGE_4G |
            (0<<4) | //0.5 bandwidth (rather than 0.25 bandwidth)
//          ADXL362_EXT_TRIGGER | 
            ADXL362_RATE_400;  //ODR = 400hz
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);
    
    
    //Configure Power Control Register
    command[0] = ADXL362_REG_WRITE;
    command[1] = ADXL362_POWER_CTL;
    command[2] = 0x00 |
//          ADXL362_EXT_CLOCK |
            ADXL362_LOW_NOISE2 | //Ultra low noise mode
//          ADXL362_SLEEP |
//          ADXL362_AUTO_SLEEP |
            ADXL362_MEASURE_3D; //Turn on Measurement mode
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);


    //Read X register
    command[0] = ADXL362_REG_READ; //Read
    command[1] = ADXL362_XDATA8; //read X-reg
    command[2] = 0x00; //fluff for the read
    FIFOSPI2_pushTxQueue(command, 3, ADXL362_SLAVE_SELECT_LINE);

    //Wait for all data to be sent
    while (FIFOSPI2_rxBufferIndex() < (3*16)) {}

    //pop all data from stack
    i = 0;
    while (i < (3*16))
    {
        func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
        i++;
    }
}

void ADXL362_pushReadXYZT()
{
    //Burst read
    uint8 read[10];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_XDATAL; //read X-reg
    read[2] = 0x00; //ADXL362_XDATAL
    read[3] = 0x00; //ADXL362_XDATAH
    read[4] = 0x00; //ADXL362_YDATAL
    read[5] = 0x00; //ADXL362_YDATAH
    read[6] = 0x00; //ADXL362_ZDATAL
    read[7] = 0x00; //ADXL362_ZDATAH
    read[8] = 0x00; //ADXL362_TEMPL
    read[9] = 0x00; //ADXL362_TEMPH
    FIFOSPI2_pushTxQueue(read, 10, ADXL362_SLAVE_SELECT_LINE);
}
void ADXL362_popXYZT()
{    
    uint8 func_rslt, fluff;
    
    uint8 x_msb = 0, x_lsb = 0;
    uint8 y_msb = 0, y_lsb = 0;    
    uint8 z_msb = 0, z_lsb = 0;
    uint8 temp_msb = 0, temp_lsb = 0;
    
    short x_16b = 0, y_16b = 0, z_16b= 0, temp_16b = 0;




    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff

    func_rslt = FIFOSPI2_popRxQueue(&x_lsb); //ADXL362_XDATAL
    func_rslt = FIFOSPI2_popRxQueue(&x_msb); //ADXL362_XDATAH

    func_rslt = FIFOSPI2_popRxQueue(&y_lsb); //ADXL362_YDATAL
    func_rslt = FIFOSPI2_popRxQueue(&y_msb); //ADXL362_YDATAH

    func_rslt = FIFOSPI2_popRxQueue(&z_lsb); //ADXL362_ZDATAL
    func_rslt = FIFOSPI2_popRxQueue(&z_msb); //ADXL362_ZDATAH

    func_rslt = FIFOSPI2_popRxQueue(&temp_lsb); //ADXL362_TEMPL
    func_rslt = FIFOSPI2_popRxQueue(&temp_msb); //ADXL362_TEMPH
    
    
    x_16b = (x_msb << 8) | x_lsb;
    y_16b = (y_msb << 8) | y_lsb;
    z_16b = (z_msb << 8) | z_lsb;
    temp_16b = (temp_msb << 8) | temp_lsb;

    ADXL362_XAcceleration_Raw = -1*x_16b  ;
    ADXL362_YAcceleration_Raw = y_16b;
    ADXL362_ZAcceleration_Raw = z_16b;
    ADXL362_Temperature_Raw = temp_16b;
    
}
void ADXL362_convertXYZT()
{
    float G_per_LSB = 0;
    float C_per_LSB = 0;

    //gravitiys per least significant bit (Check ADXL362 datasheet)
    G_per_LSB = 2e-3; //For the +-4g range
    //celsius per least significant bit (Check ADXL362 datasheet)
    C_per_LSB = 0.065;

    ADXL362_XAcceleration = G_per_LSB * (float)ADXL362_XAcceleration_Raw;
    ADXL362_YAcceleration = G_per_LSB * (float)ADXL362_YAcceleration_Raw;
    ADXL362_ZAcceleration = G_per_LSB * (float)ADXL362_ZAcceleration_Raw;
    ADXL362_Temperature = C_per_LSB * (float)ADXL362_Temperature_Raw;
}

//
//void ADXL362_queueReadMSBX()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_XDATA8; //read X-reg
//    read[2] = 0x00; //fluff for read (8 MSB of X accel reg)
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_interpretMSBX()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_XDATA8 (8 MSB)
//    ADXL362_XAcceleration = read_rslt1 ;
//
//}
//
//void ADXL362_queueReadMSBY()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_YDATA8; //read Y-reg
//    read[2] = 0x00; //fluff
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_interpretMSBY()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_YDATA8 (Y 8-MSB)
//    ADXL362_YAcceleration = read_rslt1 ;
//
//}
//
//void ADXL362_queueReadMSBZ()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_ZDATA8; //read Z-reg
//    read[2] = 0x00; //fluff
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_InterpretMSBZ()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_ZDATA8 (Z 8-MSB)
//    ADXL362_ZAcceleration = read_rslt1 ;
//
//}

