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
static void Drv_Com_Rx0_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx1_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx2_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx_Isr_Handler(rx_ctrl_block_t *rxCtrl, uint8_t recvVal );
static uint8_t Drv_Com_Cal_Checksum(uint8_t *buf, uint8_t length );

/* Private variables ------------------------------------*/
static tx_queue_t tx0Queue;
static tx_queue_t tx1Queue;
static tx_queue_t tx2Queue;
static tx_queue_t tx3Queue;

static rx_ctrl_block_t rx0Ctrl;
static rx_ctrl_block_t rx1Ctrl;
static rx_ctrl_block_t rx2Ctrl;

static com_event_callback_t comEventCallback = NULL;
static uint8_t tx1DoneFlag;

void Drv_Com_Init(com_event_callback_t callback )
{
    Hal_Com_Init();

    Hal_Com_Regist_Rx_Isr_Callback(Drv_Com_Rx0_Isr_Handler, Drv_Com_Rx1_Isr_Handler, Drv_Com_Rx2_Isr_Handler);

    comEventCallback = callback;
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

                comEventCallback(rxCtrl->dataBuf, 4+rxCtrl->dataLength);
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
        case COM3: pTxQueue = &tx3Queue; break;
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
        case COM3: pTxQueue = &tx3Queue; break;
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

void Drv_Com_Tx1_Enable(void )
{
    Hal_Com_Tx1_Enable();
}

void Drv_Com_Tx1_Send(uint8_t *buf, uint16_t length )
{
    Hal_Com_Tx1_Send(buf, length, Drv_Com_Tx1_Done_Callback);    
}

uint8_t Drv_Com_Tx1_Get_State(void )
{
    return tx1DoneFlag;
}

void Drv_Com_Tx1_Clr_State(void )
{
    tx1DoneFlag = 0;
}

static void Drv_Com_Tx1_Done_Callback(void )
{
    tx1DoneFlag = 1;
}

