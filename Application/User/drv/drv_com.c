/********************************************************
* @file       drv_com.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_com.h"
#include "drv_event.h"
#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx1_Done_Callback(void );
static void Drv_Com_Tx2_Done_Callback(void );
static void Drv_Com_Tx6_Done_Callback(void );
static void Drv_Com_Rx0_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx1_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx2_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx6_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx_Isr_Handler(rx_ctrl_block_t *rxCtrl, uint8_t recvVal );
static uint8_t Drv_Com_Cal_Checksum(uint8_t *buf, uint8_t length );

/* Private variables ------------------------------------*/
static tx_queue_t tx0Queue;
static tx_queue_t tx1Queue;
static tx_queue_t tx2Queue;
static tx_queue_t tx6Queue;

static rx_ctrl_block_t rx0Ctrl;
static rx_ctrl_block_t rx1Ctrl;
static rx_ctrl_block_t rx2Ctrl;
static rx_ctrl_block_t rx6Ctrl;

static com_event_callback_t comCaseEventCallback = NULL;
static com_event_callback_t comUpgEventCallback = NULL;
static uint8_t tx1DoneFlag;
static uint8_t tx2DoneFlag;
static uint8_t tx6DoneFlag;

void Drv_Com_Init(com_event_callback_t comCaseCallback, com_event_callback_t comUpgCallback )
{
    Hal_Com_Init();

    Hal_Com_Regist_Rx_Isr_Callback(Drv_Com_Rx0_Isr_Handler, Drv_Com_Rx1_Isr_Handler, Drv_Com_Rx2_Isr_Handler, Drv_Com_Rx6_Isr_Handler);

    comCaseEventCallback = comCaseCallback;

    comUpgEventCallback = comUpgCallback;
}

static void Drv_Com_Rx0_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Isr_Handler(&rx0Ctrl, recvVal);
}

static void Drv_Com_Rx1_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Isr_Handler(&rx1Ctrl, recvVal);
}

static void Drv_Com_Rx2_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Isr_Handler(&rx2Ctrl, recvVal);
}

static void Drv_Com_Rx_Isr_Handler(rx_ctrl_block_t *rxCtrl, uint8_t recvVal )
{
    switch(rxCtrl->stat)
    {
        case RX_STAT_HEADER:
        {
            if(recvVal == 0x05)
            {
                rxCtrl->dataBuf[0] = recvVal;

                rxCtrl->stat = RX_STAT_TYPE;

                break;
            }
        }
        case RX_STAT_TYPE:
        {
            if(recvVal == 0x5b)
            {
                rxCtrl->dataBuf[1] = recvVal;

                rxCtrl->lengthIndex = 0;

                rxCtrl->stat = RX_STAT_LENGTH;

                break;
            }
            else
            {
                rxCtrl->stat = RX_STAT_HEADER;
            }
            break;
        }
        case RX_STAT_LENGTH:
        {
            if(rxCtrl->lengthIndex == 0)
            {
                rxCtrl->dataBuf[2] = recvVal;
                
                rxCtrl->dataLength = recvVal;

                rxCtrl->lengthIndex = 1;
            }
            else
            {
                rxCtrl->dataBuf[3] = recvVal;
                
                rxCtrl->dataLength |= (uint16_t)recvVal << 8;
                
                rxCtrl->lengthIndex = 0;
                
                rxCtrl->dataCnt = 0;

                rxCtrl->stat = RX_STAT_DATA;
            }
            break;
        }
        case RX_STAT_DATA:
        {
            rxCtrl->dataBuf[4 + rxCtrl->dataCnt++] = recvVal;
            
            if(rxCtrl->dataCnt == (rxCtrl->dataLength-1))
            {
                rxCtrl->stat = RX_STAT_CHECKSUM;
            }
            break;
        }
        case RX_STAT_CHECKSUM:
        {
            if(Drv_Com_Cal_Checksum(rxCtrl->dataBuf, 3+rxCtrl->dataLength) == recvVal)
            {
                rxCtrl->dataBuf[3+rxCtrl->dataLength] = recvVal;

                if(comCaseEventCallback != NULL)
                {
                    comCaseEventCallback(rxCtrl->dataBuf, 4+rxCtrl->dataLength);
                }
            }
            
            rxCtrl->dataCnt = 0;
            rxCtrl->dataLength = 0;
            rxCtrl->lengthIndex = 0;
            rxCtrl->stat = RX_STAT_HEADER;
            
            break;
        }
        default: break;
    }
}

static void Drv_Com_Rx6_Isr_Handler(uint8_t recvVal )
{
    switch(rx6Ctrl.stat)
    {
        case RX_STAT_HEADER:
        {
            if(recvVal == 0x5a)
            {
                rx6Ctrl.dataCnt++;
            }
            else 
            {
                rx6Ctrl.dataCnt = 0;
            }

            if(rx6Ctrl.dataCnt == 2)
            {
                rx6Ctrl.dataCnt = 0;

                rx6Ctrl.checkSum = 0;
                
                rx6Ctrl.stat = RX_STAT_LENGTH;
            }
            
            break;
        }
        case RX_STAT_LENGTH:
        {
            rx6Ctrl.dataLength = recvVal;

            rx6Ctrl.dataBuf[rx6Ctrl.dataCnt++] = rx6Ctrl.dataLength;

            rx6Ctrl.checkSum += rx6Ctrl.dataLength;
            
            rx6Ctrl.stat = RX_STAT_DATA;

            break;
        }
        case RX_STAT_DATA:
        {
            if(rx6Ctrl.dataCnt < (rx6Ctrl.dataLength - 1))
            {
                rx6Ctrl.dataBuf[rx6Ctrl.dataCnt++] = recvVal;

                rx6Ctrl.checkSum += recvVal;
            }

            if(rx6Ctrl.dataCnt == (rx6Ctrl.dataLength -1))
            {
                rx6Ctrl.stat = RX_STAT_CHECKSUM;
            }

            break;
        }
        case RX_STAT_CHECKSUM:
        {
            static uint16_t cnt;
            if(rx6Ctrl.checkSum == recvVal)
            {
                while ((__USART_FLAG_STATUS_GET(USART6, TC) == RESET));
                
                #if 1
                if(comUpgEventCallback != NULL)
                {
                    comUpgEventCallback(&rx6Ctrl.dataBuf[1], rx6Ctrl.dataLength - 2);
                }
                #else
                extern void App_Com_Upg_Tx_Ack();
                App_Com_Upg_Tx_Ack();
                cnt++;
                #endif
            }

            rx6Ctrl.dataCnt = 0;
            
            rx6Ctrl.stat = RX_STAT_HEADER;
            
            break;
        }
        default: break;
    }
}

static uint8_t Drv_Com_Cal_Checksum(uint8_t *buf, uint8_t length )
{
    uint8_t i = 0;
    uint8_t checkSum = 0;

    for(i=0;i<length;i++)
    {
        checkSum += buf[i];
    }

    return checkSum;
}

void Drv_Tx_Queue_Put(com_port_t com, uint8_t *buf, uint8_t length )
{
    uint8_t i;

    if(length >= TX_DATA_MAX_SIZE)
    {
        length = TX_DATA_MAX_SIZE;
    }

    tx_queue_t *pTxQueue = NULL;

    switch(com)
    {
        case COM0: pTxQueue = &tx0Queue; break;
        case COM1: pTxQueue = &tx1Queue; break;
        case COM2: pTxQueue = &tx2Queue; break;
        case COM6: pTxQueue = &tx6Queue; break;
        default: break;
    }

    pTxQueue->buf[pTxQueue->rear].com = com;
    
    pTxQueue->buf[pTxQueue->rear].length = length;

    for(i=0;i<length;i++)
    {
        pTxQueue->buf[pTxQueue->rear].data[i] = buf[i];
    }

    pTxQueue->rear = (pTxQueue->rear + 1) % TX_QUEUE_MAX_SIZE;
}

uint8_t Drv_Tx_Queue_Get(com_port_t com, com_data_t *txData )
{
    uint8_t retVal; 
    
    tx_queue_t *pTxQueue = NULL;

    switch(com)
    {
        case COM0: pTxQueue = &tx0Queue; break;
        case COM1: pTxQueue = &tx1Queue; break;
        case COM2: pTxQueue = &tx2Queue; break;
        case COM6: pTxQueue = &tx6Queue; break;
        default: break;
    }
    
    if(pTxQueue->front != pTxQueue->rear)
    {
        *txData = pTxQueue->buf[pTxQueue->front];

        pTxQueue->front = (pTxQueue->front + 1) % TX_QUEUE_MAX_SIZE;

        retVal = COM_QUEUE_OK;
    }
    else
    {
        retVal = COM_QUEUE_ERR;
    }

    return retVal;
}

/*********************** UART1 ******************************/

void Drv_Com_Tx_Enable(com_port_t com )
{
    if(com == COM1)
    {
        Hal_Com_Tx1_Enable();
    }
    else if(com == COM2)
    {
        Hal_Com_Tx2_Enable();
    }
}

void Drv_Com_Tx_Disable(com_port_t com )
{
    if(com == COM1)
    {
        Hal_Com_Tx1_Disable();
    }
    else if(com == COM2)
    {
        Hal_Com_Tx2_Disable();
    }
}

void Drv_Com_Tx_Send(com_port_t com, uint8_t *buf, uint16_t length )
{
    if(com == COM1)
    {
        Hal_Com_Tx1_Send(buf, length, Drv_Com_Tx1_Done_Callback); 
    }
    else if(com == COM2)
    {
        Hal_Com_Tx2_Send(buf, length, Drv_Com_Tx2_Done_Callback);
    }
    else if(com == COM6)
    {
        Hal_Com_Tx6_Send(buf, length, Drv_Com_Tx6_Done_Callback);
    }
}

uint8_t Drv_Com_Tx_Get_State(com_port_t com )
{
    uint8_t retVal;
    
    if(com == COM1)
    {
        retVal = tx1DoneFlag;
    }
    else if(com == COM2)
    {
        retVal = tx2DoneFlag;
    }
    else if(com == COM6)
    {
        retVal = tx6DoneFlag;
    }
    
    return retVal;
}

void Drv_Com_Tx_Clr_State(com_port_t com )
{
    if(com == COM1)
    {
        tx1DoneFlag = 0;
    }
    else if(com == COM2)
    {
        tx2DoneFlag = 0;
    }
    else if(com == COM6)
    {
        tx6DoneFlag = 0;
    }
}

static void Drv_Com_Tx1_Done_Callback(void )
{
    tx1DoneFlag = 1;
}

static void Drv_Com_Tx2_Done_Callback(void )
{
    tx2DoneFlag = 1;
}

static void Drv_Com_Tx6_Done_Callback(void )
{
    tx6DoneFlag = 1;
}


/************************************************************/

