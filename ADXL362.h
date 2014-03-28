/**
 * @file: ADXL362.h
 * @brief: A set of function for interfacing with the ADXL362 Accelerometer.
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

#ifndef ADXL362_H
#define	ADXL362_H

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
#define ADXL362_SLAVE_SELECT_LINE 1

//******************************************************************************
//Device #defines
//******************************************************************************
//From ADXL362's sample code
/* --- command --- */
#define ADXL362_REG_WRITE   0x0a
#define ADXL362_REG_READ    0x0b
#define ADXL362_FIFO_READ   0x0d

/* ------- Register names ------- */
/* -- part ID constants -- */
#define ADXL362_DEVID_AD           0x00
#define ADXL362_DEVID_MST          0x01
#define ADXL362_PARTID             0x02
#define ADXL362_REVID              0x03
#define ADXL362_XID                0x04

/* -- RO data -- */
#define ADXL362_XDATA8             0x08
#define ADXL362_YDATA8             0x09
#define ADXL362_ZDATA8             0x0A
#define ADXL362_STATUS             0x0B
#define ADXL362_FIFO_ENTRIES_L     0x0C
#define ADXL362_FIFO_ENTRIES_H     0x0D
#define ADXL362_XDATAL             0x0e
#define ADXL362_XDATAH             0x0f
#define ADXL362_YDATAL             0x10
#define ADXL362_YDATAH             0x11
#define ADXL362_ZDATAL             0x12
#define ADXL362_ZDATAH             0x13
#define ADXL362_TEMPL              0x14
#define ADXL362_TEMPH              0x15
#define ADXL362_X_ADCL             0x16
#define ADXL362_X_ADCH             0x17

/* -- Control and Config -- */
#define ADXL362_SOFT_RESET         0x1f
#define ADXL362_THRESH_ACTL        0x20
#define ADXL362_THRESH_ACTH        0x21
#define ADXL362_TIME_ACT           0x22
#define ADXL362_THRESH_INACTL      0x23
#define ADXL362_THRESH_INACTH      0x24
#define ADXL362_TIME_INACTL        0x25
#define ADXL362_TIME_INACTH        0x26
#define ADXL362_ACT_INACT_CTL      0x27
#define ADXL362_FIFO_CONTROL       0x28
#define ADXL362_FIFO_SAMPLES       0x29
#define ADXL362_INTMAP1            0x2a
#define ADXL362_INTMAP2            0x2b
#define ADXL362_FILTER_CTL         0x2c
#define ADXL362_POWER_CTL          0x2d
#define ADXL362_SELF_TEST          0x2e


// Bit field definitions and register values

/* register values for DEVID                                            */
/* The device ID should always read this value, The customer does not
  need to use this value but it can be samity checked to check that the
  device can communicate                                                */
#define ADXL362_ID              0xe2

/* Key Value in soft reset                                              */
#define ADXL362_SOFT_RESET_KEY      0x52

/* Registers THRESH_ACTL through TIME_INACTH just take values
   in lsbs or samples.
   There are no specific bit fields in these registers                  */

/* Bit values in ACT_INACT_CTL  (6 bits allocated)                      */
#define ADXL362_ACT_ENABLE      0x01
#define ADXL362_ACT_DISABLE     0x00
#define ADXL362_ACT_AC          0x02
#define ADXL362_ACT_DC          0x00
#define ADXL362_INACT_ENABLE    0x04
#define ADXL362_INACT_DISABLE   0x00
#define ADXL362_INACT_AC        0x08
#define ADXL362_INACT_DC        0x00
#define ADXL362_ACT_INACT_LINK  0x10
#define ADXL362_ACT_INACT_LOOP  0x20
#define ADXL362_ACT_INACT_DEFAULT  0x00

/* Bit values in FIFO_CTL  (4 bits allocated)                            */
#define ADXL362_FIFO_MODE_OFF     0x00
#define ADXL362_FIFO_MODE_FIFO    0x01
#define ADXL362_FIFO_MODE_STREAM  0x02
#define ADXL362_FIFO_MODE_TRIGGER 0x03
#define ADXL362_FIFO_TEMP         0x04
#define ADXL362_FIFO_SAMPLES_AH   0x08



/* bit values in INTMAP1 and INTMAP2 (and status)                        */
#define ADXL362_INT_DATA_READY     0x01
#define ADXL362_INT_FIFO_READY     0x02
#define ADXL362_INT_FIFO_WATERMARK 0x04
#define ADXL362_INT_FIFO_OVERRUN   0x08
#define ADXL362_INT_ACT            0x10
#define ADXL362_INT_INACT          0x20
#define ADXL362_INT_AWAKE          0x40
#define ADXL362_INT_LOW            0x80

/* Bit values in FILTER_CTL  (8 bits allocated)                            */
#define ADXL362_RATE_400        0x05
#define ADXL362_RATE_200        0x04
#define ADXL362_RATE_100        0x03  /* default */
#define ADXL362_RATE_50         0x02
#define ADXL362_RATE_25         0x01
#define ADXL362_RATE_12_5       0x00

#define ADXL362_EXT_TRIGGER     0x08

#define ADXL362_AXIS_X          0x00
#define ADXL362_AXIS_Y          0x10
#define ADXL362_AXIS_Z          0x20

#define ADXL362_RANGE_2G        0x00
#define ADXL362_RANGE_4G        0x40
#define ADXL362_RANGE_8G        0x80


/* Bit values in POWER_CTL  (8 bits allocated)                            */
#define ADXL362_STANDBY           0x00
#define ADXL362_MEASURE_1D        0x01
#define ADXL362_MEASURE_3D        0x02
#define ADXL362_AUTO_SLEEP        0x04
#define ADXL362_SLEEP             0x08

#define ADXL362_LOW_POWER         0x00
#define ADXL362_LOW_NOISE1        0x10
#define ADXL362_LOW_NOISE2        0x20
#define ADXL362_LOW_NOISE3        0x30

#define ADXL362_EXT_CLOCK         0x40
#define ADXL362_EXT_ADC           0x80

/* Bit values in SELF_TEST  (1 bit allocated)                            */
#define ADXL362_SELFTEST_ON       0x01
#define ADXL362_SELFTEST_OFF      0x00

//******************************************************************************
//Global Variables
//******************************************************************************
/** Contains the ADXL362's X acceleration reading in m/s^2*/
extern float ADXL362_XAcceleration;
/** Contains the ADXL362's Y acceleration reading in m/s^2*/
extern float ADXL362_YAcceleration;
/** Contains the ADXL362's Z acceleration reading in m/s^2*/
extern float ADXL362_ZAcceleration;
/** Contains the ADXL362's temperature reading in Celsius*/
extern float ADXL362_Temperature;

/** Contains the ADXL362's raw X acceleration reading*/
extern INT16 ADXL362_XAcceleration_Raw;
/** Contains the ADXL362's raw Y acceleration reading*/
extern INT16 ADXL362_YAcceleration_Raw;
/** Contains the ADXL362's raw Z acceleration reading*/
extern INT16 ADXL362_ZAcceleration_Raw;
/** Contains the ADXL362's raw temperature reading*/
extern INT16 ADXL362_Temperature_Raw;

//******************************************************************************
//Function Prototypes
//******************************************************************************
/**
 * Turns on the ADXL362 device in: Ultra low noise, half bandwidth anti aliaising,
 * 400hz data rate, no activity or inactivity thresholds. (200hz sampling rate)
 */
void ADXL362_startMeasurements();

/**
 * Pushes a request to read X, Y, Z and Temperature data from an ADXL362 onto
 * the SPI2's TX buffer.
 */
void ADXL362_pushReadXYZT();

/**
 * Pops X, Y, Z, and temperature values off the SPI2 stack and stores them
 * in the global raw ADXL362 variables.
 */
void ADXL362_popXYZT();

/**
 * Converts the global raw ADXL362 variables into the global float ADXL362 variables.
 */
void ADXL362_convertXYZT();





#ifdef	__cplusplus
}
#endif

#endif	/* ADXL362_H */

