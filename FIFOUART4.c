#include "FIFOUART4.h"

//******************************************************************************
// Local Variables and Typedefs
//******************************************************************************
UINT16 FIFOUART4_RxBuffer_Index = 0;
UINT16 FIFOUART4_RxBuffer_ReadIndex = 0;
UINT8 FIFOUART4_RxBuffer[FIFOUART4_BUFFERSIZE]; //Holds all the receive chars from UART

UINT16 FIFOUART4_TxBuffer_Index = 0;
UINT16 FIFOUART4_TxBuffer_TxIndex = 0;
UINT8 FIFOUART4_TxBuffer[FIFOUART4_BUFFERSIZE]; //Holds all the chars to TX on UART4


//******************************************************************************
//Local Function Declarations
//******************************************************************************
inline int FIFOUART4_pushRxQueue(UINT8 rxBytes[], int length);
inline int FIFOUART4_popTxQueue(UINT8 * txByte);

//******************************************************************************
//Local Function Definitions
//******************************************************************************
inline int FIFOUART4_pushRxQueue(UINT8 rxBytes[], int length)
{
    int i = 0;

    if ((FIFOUART4_RxBuffer_Index + length) >= FIFOUART4_BUFFERSIZE)
    {
        //Overflow condition
        return -1;
    }
    else
    {
        //Add each byte to the tx buffer
        while (i < length)
        {
            FIFOUART4_RxBuffer[FIFOUART4_RxBuffer_Index] = rxBytes[i];
            FIFOUART4_RxBuffer_Index++;
            i++;

        }

        return 1;
    }
}
inline int FIFOUART4_popTxQueue(UINT8 * txByte)
{
    if (FIFOUART4_TxBuffer_Index <= 0)
    {//The receive buffer is empty.

        *txByte = 0;
        return -2;
    }
    else if (FIFOUART4_TxBuffer_Index >= FIFOUART4_BUFFERSIZE)
    {//The receive buffer is overflowing.

        *txByte = 0;
        return -1;
    }

    else
    {//The receiver buffer has entries.

        //pop byte
        *txByte = FIFOUART4_TxBuffer[FIFOUART4_TxBuffer_TxIndex++];

        //If all bytes from the buffer have been read.
        if (FIFOUART4_TxBuffer_TxIndex >= FIFOUART4_TxBuffer_Index)
        {
            //Reset the indexs
            FIFOUART4_TxBuffer_TxIndex = 0;
            FIFOUART4_TxBuffer_Index = 0;

            return 1; //success, empty buffer
        }
        else
        {
            return 2; //success, non-empty buffer
        }

    }

    //unknown error (possibly a wild program counter got us here)
    return -3;
}

//******************************************************************************
//Public Function Definitions
//******************************************************************************
void FIFOUART4_initialize()
{
    UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | //Sets the data transfer size to 8-bits per frame. 
            UART_PARITY_NONE | //Disables parity bit generation. 
            UART_STOP_BITS_1); //1 stop bit per frame (default). 

    UARTSetDataRate(UART4, GetPeripheralClock(), FIFOUART4_BAUD_RATE);



    //Interrupt Stuff
    INTSetVectorPriority(INT_UART_4_VECTOR, INT_PRIORITY_LEVEL_5);
    INTSetVectorSubPriority(INT_UART_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    INTClearFlag(INT_U4RX);
    INTClearFlag(INT_U4TX);

    //configure what triggers UART4 itnerrupts
    UARTSetFifoMode(UART4,
        UART_INTERRUPT_ON_TX_BUFFER_EMPTY | //TX interrupt will occur when the TX buffer is empty. 
        UART_INTERRUPT_ON_RX_NOT_EMPTY); //RX interrupt will occur whenever the RX buffer has any data. 

    //Enable UART4 Rx Interrupt
    INTEnable(INT_U4RX, INT_ENABLED);
    //Enable UART4 Tx Interrupt
    //INTEnable(INT_U4TX, INT_ENABLED);

    UARTEnable(UART4, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

}

int FIFOUART4_pushTxQueue(UINT8 txBytes[], int length)
{
    int i = 0;

    if ((FIFOUART4_TxBuffer_Index + length) >= FIFOUART4_BUFFERSIZE)
    {
        //Overflow condition
        return -1;
    }
    else
    {
        //Add each byte to the tx buffer
        while (i < length)
        {
            FIFOUART4_TxBuffer[FIFOUART4_TxBuffer_Index] = txBytes[i];
            FIFOUART4_TxBuffer_Index++;
            i++;
        }

        //enable TX interrupt
        INTEnable(INT_U4TX, INT_ENABLED);


        return 1;
    }


}

int FIFOUART4_popRxQueue(UINT8 * rxByte)
{
    if (FIFOUART4_RxBuffer_Index <= 0)
    {//The receive buffer is empty.

        *rxByte = 0;
        return -2;
    }
    else if (FIFOUART4_RxBuffer_Index >= FIFOUART4_BUFFERSIZE)
    {//The receive buffer is overflowing.

        *rxByte = 0;
        return -1;
    }

    else
    {//The receiver buffer has entries.

        //pop byte
        *rxByte = FIFOUART4_RxBuffer[FIFOUART4_RxBuffer_ReadIndex++];

        //If all bytes from the buffer have been read.
        if (FIFOUART4_RxBuffer_ReadIndex >= FIFOUART4_RxBuffer_Index)
        {
            //Reset the indexs
            FIFOUART4_RxBuffer_ReadIndex = 0;
            FIFOUART4_RxBuffer_Index = 0;

            return 1; //success, empty buffer
        }
        else
        {
            return 2; //success, non-empty buffer
        }
    }

    //unknown error (possibly a wild program counter got us here)
    return -3;
}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
void __ISR(_UART_4_VECTOR, IPL5AUTO) __UART4Interrupt(void)
{
    UINT8 rxByte = 0;
    UINT8 txByte = 0;
    int rslt = 0;

    if (INTGetFlag(INT_U4RX))
    {
        //Add received byte
        rxByte = UARTGetDataByte(UART4);
        FIFOUART4_pushRxQueue(&rxByte, 1);

        INTClearFlag(INT_U4RX);
    }
    if (INTGetFlag(INT_U4TX))
    {
        rslt = FIFOUART4_popTxQueue(&txByte);

        switch (rslt)
        {
            case 2: //Success, non-empty buffer
                UARTSendDataByte(UART4, txByte);
                break;

            case 1: //Success, empty buffer
                UARTSendDataByte(UART4, txByte);
                INTEnable(INT_U4TX, INT_DISABLED);
                break;

            case -1: //Queue is Overflowing
                //Reset the indexs
                FIFOUART4_TxBuffer_TxIndex = 0;
                FIFOUART4_TxBuffer_Index = 0;
            case -2: //Queue is Empty
            default: //unknown result
                INTEnable(INT_U4TX, INT_DISABLED);
                break;
        }

        INTClearFlag(INT_U4TX);
    }
}
