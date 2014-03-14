#include "hardwareProfile.h"

void initializeBoard(void)
{
    INTDisableInterrupts();
    setupPorts();


    FIFOSPI2_initialize();
    FIFOI2C2_initialize();
	FIFOUART1_initialize();

    INTEnableSystemMultiVectoredInt();
    //Enable system-wide interrupts
    INTEnableInterrupts();

    ADXL362_startMeasurements();        //Setup Accelerometer
    L3G4200D_startMeasurements();       //Setup Gyroscope
    HMC5883L_startMeasurements();       //Setup 3-axis compass

    INTEnableSystemMultiVectoredInt();
    //Enable system-wide interrupts
    INTEnableInterrupts();

    setupTimers();
    setupInputCaptures();
    setupOutputCompares();
    configInterrupts();
}

void setupPorts(void)
{
    //Setup ESC pins for PWM output
    PORTSetPinsDigitalOut(IOPORT_D, BIT_0); //PWM1-OC1 digital pin 3
    PORTSetPinsDigitalOut(IOPORT_D, BIT_1); //PWM2-OC2 digital pin 5
    PORTSetPinsDigitalOut(IOPORT_D, BIT_2); //PWM3-OC3 digital pin 6
    PORTSetPinsDigitalOut(IOPORT_D, BIT_3); //PWM4-OC4 digital pin 9

    //Setup Input Capture pins
    PORTSetPinsDigitalIn(IOPORT_D, BIT_8);  //IC1 digital pin 2  (THRO)
    PORTSetPinsDigitalIn(IOPORT_D, BIT_9);  //IC2 digital pin 7  (AILE)
    PORTSetPinsDigitalIn(IOPORT_D, BIT_10); //IC3 digital pin 8  (ELEV)
    PORTSetPinsDigitalIn(IOPORT_D, BIT_11); //IC4 digital pin 35 (RUDD)
    PORTSetPinsDigitalIn(IOPORT_D, BIT_12); //IC5 digital pin 10 (GEAR)
}

void setupTimers(void)
{
    //Clear timer controller and timer values
    T2CONCLR = 0xFFFF;
    T3CONCLR = 0xFFFF;
    T5CONCLR = 0xFFFF;
    TMR2CLR = 0xFFFF;
    TMR3CLR = 0xFFFF;
    //TMR5CLR = 0xFFFF;

    //Set PR values to desired period register value and enable timers
    PR2 = PER_REG;
    PR3 = PER_REG;
    //PR5 = MAX_PER;
    T2CONSET = 0x8010;      //PS = 8
    T3CONSET = 0x8010;      //PS = 8
    //T5CONSET = 0x8000;

    //Turn on clock
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 6250); //800hz

    //Turn on clock
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, 10240);

    //Turn on clock
    OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_32, 16666); //75hz
}

void configInterrupts(void)
{
    //Enable multi-vectored mode for interrupts
    //INTEnableSystemMultiVectoredInt();

    ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture2(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture3(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture4(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture5(IC_INT_ON | IC_INT_PRIOR_6);
    //ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7);

    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    //Setup Timer4
    INTClearFlag(INT_T4);
    INTSetVectorPriority(INT_TIMER_4_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T4, INT_ENABLED);

    //Setup Timer5
    INTClearFlag(INT_T5);
    INTSetVectorPriority(INT_TIMER_5_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_5_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
    INTEnable(INT_T5, INT_ENABLED);

//    //Enable system-wide interrupts
//    INTEnableInterrupts();
}
