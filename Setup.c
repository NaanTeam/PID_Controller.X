
#include "Setup.h"


void Setup_initializeBoard(void)
{
    INTDisableInterrupts();
    Setup_setupPorts();

    //Start Protocols
    FIFOSPI2_initialize();
    FIFOI2C2_initialize();
    FIFOUART1_initialize();

    INTEnableSystemMultiVectoredInt();
    //Enable system-wide interrupts
    INTEnableInterrupts();

    SensorLoop_start();
    CommunicationLoop_start();


    Setup_setupTimers();
    setupInputCaptures();
    setupOutputCompares();
    Setup_configInterrupts();
}

void Setup_setupPorts(void)
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

void Setup_setupTimers(void)
{
    //Clear timer controller and timer values
    T2CONCLR = 0xFFFF;
    T3CONCLR = 0xFFFF;
    //T5CONCLR = 0xFFFF;
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


}

void Setup_configInterrupts(void)
{
    //Enable multi-vectored mode for interrupts
    //INTEnableSystemMultiVectoredInt();

    ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture2(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture3(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture4(IC_INT_ON | IC_INT_PRIOR_6);
    ConfigIntCapture5(IC_INT_ON | IC_INT_PRIOR_6);
    //ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7);
}
