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

#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx1_Done_Callback(void );
/* Private variables ------------------------------------*/
static com_queue_t comQueue;

static uint8_t tx1DoneFlag;

void Drv_Com_Init(void )
{
    Hal_Com_Init();
}
void Drv_Com_Queue_Put(com_port_t com, uint8_t *buf, uint8_t length )
{
    uint8_t i;

    if(length >= COM_DATA_MAX_SIZE)
    {
        length = COM_DATA_MAX_SIZE;
    }

    comQueue.buf[comQueue.rear].com = com;

    for(i=0;i<length;i++)
    {
        comQueue.buf[comQueue.rear].data[i] = buf[i];
    }

    comQueue.rear = (comQueue.rear + 1) % COM_QUEUE_MAX_SIZE;
}

uint8_t Drv_Com_Queue_Get(com_data_t *comData )
{
    uint8_t retVal; 
    
    if(comQueue.front != comQueue.rear)
    {
        *comData = comQueue.buf[comQueue.front];

        comQueue.front = (comQueue.front + 1) % COM_QUEUE_MAX_SIZE;

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

void Drv_Com_Tx1_Data(uint8_t *buf, uint16_t length )
{
    
    Hal_Com_Tx1_Data(buf, length, Drv_Com_Tx1_Done_Callback);    
}

static void Drv_Com_Tx1_Done_Callback(void )
{
    tx1DoneFlag = 1;
}

