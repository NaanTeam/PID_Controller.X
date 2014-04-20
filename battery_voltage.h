#ifndef BATTERY_VOLTAGE_H
#define	BATTERY_VOLTAGE_H

#include "hardwareProfile.h"

void setup_ADC(); //function prototype
void read_voltage();

extern float v_batt;

#endif	/* BATTERY_VOLTAGE_H */
