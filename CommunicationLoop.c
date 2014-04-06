
#include "CommunicationLoop.h"

//******************************************************************************
//Local Variable and Typedef Declarations
//******************************************************************************
UINT16 CommunicationLoop_Index = 0;
UINT8 CommunicationLoop_MsgBuffer[COMMUNICATIONLOOP_BUFFERSIZE];
UINT8 CommunicationLoop_PossibleError = 0;

UINT16 CommunicationLoop_Index2 = 0;
UINT8 CommunicationLoop_MsgBuffer2[COMMUNICATIONLOOP_BUFFERSIZE];
UINT8 CommunicationLoop_PossibleError2 = 0;


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void CommunicationLoop_start()
{

    //Setup Timer4
    INTClearFlag(INT_T4);
    INTSetVectorPriority(INT_TIMER_4_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T4, INT_ENABLED);
    //Turn on clock
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, 6250); // 100hz@40MHz

}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
void __ISR(_TIMER_4_VECTOR, IPL4AUTO) Timer4Handler(void)
{
    UINT8 rxByte = 0;
    UINT8 rslt = 0;

    //Bluetooth
    //***************** Maher **********************
    //This could happen if the incorrect msg length was sent
    if (CommunicationLoop_Index2 > 0)
    {
        CommunicationLoop_PossibleError2++;
        if (CommunicationLoop_PossibleError2 > 20)
        {
            //Reset
            CommunicationLoop_MsgBuffer2[0] = 0;
            CommunicationLoop_Index2 = 0;
        }
    }
    else 
    {
        CommunicationLoop_PossibleError2 = 0;
    }
    //if their are bytes in the receive buffer
    while(FIFOUART4_RxBuffer_Index > 0)
    {
        //Pop the byte
        rslt = FIFOUART4_popRxQueue(&rxByte);

        if (rslt > 0) //successful read
        {
            //Store byte
            CommunicationLoop_MsgBuffer2[CommunicationLoop_Index2] = rxByte;
            CommunicationLoop_Index2++;

            //If we are done receiving msgs (the 0th index holds how long the msg is)
            if (CommunicationLoop_Index2 >= CommunicationLoop_MsgBuffer2[0])
            {
                //Process the message (skims the first and last byte)
                MsgInterpreter_interpret(&(CommunicationLoop_MsgBuffer2[1]), CommunicationLoop_MsgBuffer2[0] - 1);

                //reset message
                CommunicationLoop_MsgBuffer2[0] = 0;
                CommunicationLoop_Index2 = 0;
            }
        }
        else
        {
            //Don't process the message and reset
            CommunicationLoop_MsgBuffer2[0] = 0;
            CommunicationLoop_Index2 = 0;
        }
    }


    //Serial
    //**********************************************
    
    //This could happen if the incorrect msg length was sent
    if (CommunicationLoop_Index > 0)
    {
        CommunicationLoop_PossibleError++;
        if (CommunicationLoop_PossibleError > 20)
        {
            //Reset
            CommunicationLoop_MsgBuffer[0] = 0;
            CommunicationLoop_Index = 0;
        }
    }
    else 
    {
        CommunicationLoop_PossibleError = 0;
    }
    
    //if their are bytes in the receive buffer
    while(FIFOUART1_RxBuffer_Index > 0)
    {
        //Pop the byte
        rslt = FIFOUART1_popRxQueue(&rxByte);

        if (rslt > 0) //successful read
        {
            //Store byte
            CommunicationLoop_MsgBuffer[CommunicationLoop_Index] = rxByte;
            CommunicationLoop_Index++;

            //If we are done receiving msgs (the 0th index holds how long the msg is)
            if (CommunicationLoop_Index >= CommunicationLoop_MsgBuffer[0])
            {
                //Process the message (skims the first and last byte)
                MsgInterpreter_interpret(&(CommunicationLoop_MsgBuffer[1]), CommunicationLoop_MsgBuffer[0] - 1);

                //reset message
                CommunicationLoop_MsgBuffer[0] = 0;
                CommunicationLoop_Index = 0;
            }
        }
        else
        {
            //Don't process the message and reset
            CommunicationLoop_MsgBuffer[0] = 0;
            CommunicationLoop_Index = 0;
        }
    }
  
    INTClearFlag(INT_T4);// Be sure to clear the Timer1 interrupt status
}





