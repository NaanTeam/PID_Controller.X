#include "battery_voltage.h"

#define PARAM1 ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
#define PARAM2 ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | \
               ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON
#define PARAM3 ADC_CONV_CLK_PB | ADC_SAMPLE_TIME_12
#define PARAM4 SKIP_SCAN_ALL
#define PARAM5 ENABLE_AN2_ANA

unsigned int ADC_value;
float v_batt;
float constant = 1.480;//1.488;    //Battery constant

void setup_ADC()
{
    CloseADC10();
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2);
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );
    EnableADC10();
}

void read_voltage()
{
    ADC_value = ReadADC10(0);
    v_batt = ((float)ADC_value/310.3)*4*constant;
}