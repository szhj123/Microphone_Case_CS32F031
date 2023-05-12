/********************************************************
* @file       drv_event.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
msg_queue_t msgQueue;

uint8_t Drv_Msg_Put(uint8_t cmd, const uint8_t *buf, uint8_t length )
{
    uint8_t i;

    msgQueue.buf[msgQueue.rear].cmd = cmd;

    if(length > MSG_DATA_MAX_LEN)
    {
        length = MSG_DATA_MAX_LEN;
    }
    
    for(i=0;i<length;i++)
    {
        msgQueue.buf[msgQueue.rear].data[i] = buf[i];
    }

    msgQueue.buf[msgQueue.rear].lenght = length;

    msgQueue.rear = (msgQueue.rear + 1) % MSG_QUEUE_MAX_LEN;

    return MSG_OK;
}

uint8_t Drv_Msg_Get(msg_t *msg )
{
    uint8_t ret;
    
    if(msgQueue.front != msgQueue.rear)
    {
        *msg = msgQueue.buf[msgQueue.front];

        msgQueue.front = (msgQueue.front + 1) % MSG_QUEUE_MAX_LEN;

        ret = MSG_OK;
    }
    else
    {
        ret = MSG_ERR;
    }

    return ret;
}

