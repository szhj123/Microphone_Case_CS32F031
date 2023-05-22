#ifndef _DRV_COM_H
#define _DRV_COM_H

#include "hal_com.h"

#define COM_DATA_MAX_SIZE                64
#define COM_QUEUE_MAX_SIZE               8

#define COM_QUEUE_OK                     0
#define COM_QUEUE_ERR                    (!COM_QUEUE_ERR)

typedef enum _com_port_t
{
    COM0 = 0,
    COM1,
    COM2,
    COM3
}com_port_t;

typedef struct _com_data_t
{
    uint8_t com;
    uint8_t data[COM_DATA_MAX_SIZE];
    uint8_t length;
}com_data_t;

typedef struct _com_queue_t
{
    com_data_t buf[COM_QUEUE_MAX_SIZE];
    uint8_t front;
    uint8_t rear;
}com_queue_t;

void Drv_Com_Init(void );
void Drv_Com_Tx1_Enable(void );
void Drv_Com_Tx1_Data(uint8_t *buf, uint16_t length );

void Drv_Com_Queue_Put(uint8_t *buf, uint8_t length );
uint8_t Drv_Com_Queue_Get(com_data_t *comData );

#endif 

