#ifndef _DRV_EVENT_H
#define _DRV_EVENT_H

#include "hal_cs32f031.h"

#define MSG_DATA_MAX_LEN                  72
#define MSG_QUEUE_MAX_LEN                  8     

#define MSG_QUEUE_OK                      0
#define MSG_QUEUE_ERR                     (!MSG_QUEUE_OK)

typedef struct _msg_t
{
    uint8_t cmd;
    uint8_t data[MSG_DATA_MAX_LEN];
    uint8_t lenght;
}msg_t;

typedef struct _msg_queue_t
{
    msg_t buf[MSG_QUEUE_MAX_LEN];
    uint8_t front;
    uint8_t rear;
}msg_queue_t;

uint8_t Drv_Msg_Put(uint8_t cmd, const uint8_t *buf, uint8_t length );
uint8_t Drv_Msg_Get(msg_t *msg );


#endif 

