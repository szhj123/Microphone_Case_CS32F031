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
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx1_Done_Callback(void );
static void Drv_Com_Rx0_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx1_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx2_Isr_Handler(uint8_t recvVal );
static void Drv_Com_Rx_Handler(com_rx_ctrl_block_t *comRxCtrl, uint8_t recvVal );
static uint8_t Drv_Com_Cal_Checksum(uint8_t *buf, uint8_t length );

/* Private variables ------------------------------------*/
static com_queue_t com0Queue;
static com_queue_t com1Queue;
static com_queue_t com2Queue;
static com_queue_t com3Queue;

static com_rx_ctrl_block_t com0RxCtrl;
static com_rx_ctrl_block_t com1RxCtrl;
static com_rx_ctrl_block_t com2RxCtrl;

static com_event_callback_t comEventCallback = NULL;


static uint8_t tx1DoneFlag;

void Drv_Com_Init(com_event_callback_t callback )
{
    Hal_Com_Init();

    Hal_Com_Rx0_Regist_Isr_Callback(Drv_Com_Rx0_Isr_Handler);
    Hal_Com_Rx1_Regist_Isr_Callback(Drv_Com_Rx1_Isr_Handler);
    Hal_Com_Rx2_Regist_Isr_Callback(Drv_Com_Rx2_Isr_Handler);

    comEventCallback = callback;
}

static void Drv_Com_Rx0_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Handler(&com0RxCtrl, recvVal);
}

static void Drv_Com_Rx1_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Handler(&com1RxCtrl, recvVal);
}

static void Drv_Com_Rx2_Isr_Handler(uint8_t recvVal )
{
    Drv_Com_Rx_Handler(&com2RxCtrl, recvVal);
}

static void Drv_Com_Rx_Handler(com_rx_ctrl_block_t *comRxCtrl, uint8_t recvVal )
{
    switch(comRxCtrl->stat)
    {
        case RX_STAT_HEADER:
        {
            if(recvVal == 0x05)
            {
                comRxCtrl->cmd.header = recvVal;

                comRxCtrl->stat = RX_STAT_TYPE;

                break;
            }
        }
        case RX_STAT_TYPE:
        {
            if(recvVal == 0x5b)
            {
                comRxCtrl->cmd.type = recvVal;

                comRxCtrl->stat = RX_STAT_LENGTH;

                break;
            }
            break;
        }
        case RX_STAT_LENGTH:
        {
            comRxCtrl->length = recvVal;
            
            comRxCtrl->cnt = 0;

            comRxCtrl->pData = (uint8_t *)&comRxCtrl->cmd.id_l;
            
            comRxCtrl->stat = RX_STAT_DATA;
            break;
        }
        case RX_STAT_DATA:
        {
            comRxCtrl->pData[comRxCtrl->cnt++] = recvVal;
            
            if(comRxCtrl->cnt == (comRxCtrl->length-1))
            {
                comRxCtrl->stat = RX_STAT_CHECKSUM;
            }
            break;
        }
        case RX_STAT_CHECKSUM:
        {
            if(Drv_Com_Cal_Checksum((uint8_t *)&comRxCtrl->cmd, sizeof(com_cmd_t)-1) == recvVal)
            {
                comRxCtrl->cmd.checkSum = recvVal;

                comEventCallback((uint8_t *)&comRxCtrl->cmd, sizeof(com_cmd_t));
            }
            
            comRxCtrl->cnt = 0;
            comRxCtrl->length = 0;
            comRxCtrl->pData = NULL;
            comRxCtrl->stat = RX_STAT_HEADER;
            
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

void Drv_Com_Queue_Put(com_port_t com, uint8_t *buf, uint8_t length )
{
    uint8_t i;

    if(length >= COM_DATA_MAX_SIZE)
    {
        length = COM_DATA_MAX_SIZE;
    }

    com_queue_t *pComQueue = NULL;

    switch(com)
    {
        case COM0: pComQueue = &com0Queue; break;
        case COM1: pComQueue = &com1Queue; break;
        case COM2: pComQueue = &com2Queue; break;
        case COM3: pComQueue = &com3Queue; break;
        default: break;
    }

    pComQueue->buf[pComQueue->rear].com = com;

    for(i=0;i<length;i++)
    {
        pComQueue->buf[pComQueue->rear].data[i] = buf[i];
    }

    pComQueue->rear = (pComQueue->rear + 1) % COM_QUEUE_MAX_SIZE;
}

uint8_t Drv_Com_Queue_Get(com_port_t com, com_data_t *comData )
{
    uint8_t retVal; 
    
    com_queue_t *pComQueue = NULL;

    switch(com)
    {
        case COM0: pComQueue = &com0Queue; break;
        case COM1: pComQueue = &com1Queue; break;
        case COM2: pComQueue = &com2Queue; break;
        case COM3: pComQueue = &com3Queue; break;
        default: break;
    }
    
    if(pComQueue->front != pComQueue->rear)
    {
        *comData = pComQueue->buf[pComQueue->front];

        pComQueue->front = (pComQueue->front + 1) % COM_QUEUE_MAX_SIZE;

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

uint8_t Drv_Com_Tx1_Get_Send_State(void )
{
    return tx1DoneFlag;
}

void Drv_Com_Tx1_Clr_Send_State(void )
{
    tx1DoneFlag = 0;
}

static void Drv_Com_Tx1_Done_Callback(void )
{
    tx1DoneFlag = 1;
}

