#ifndef _DRV_COM_H
#define _DRV_COM_H

#include "hal_com.h"

typedef void (*com_event_callback_t)(uint8_t *, uint8_t );

#define COM_DATA_MAX_SIZE                32
#define COM_QUEUE_MAX_SIZE               4

#define COM_QUEUE_OK                     0
#define COM_QUEUE_ERR                    (!COM_QUEUE_OK)

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

typedef struct _com_cmd_t
{
    uint8_t header;
    uint8_t type;
    uint8_t length;
    uint8_t id_l;
    uint8_t id_h;
    uint8_t flags;
    uint8_t payload[16];
    uint8_t checkSum;
}com_cmd_t;

typedef enum _com_rx_state
{
    RX_STAT_HEADER = 0,
    RX_STAT_TYPE,
    RX_STAT_LENGTH,
    RX_STAT_DATA,
    RX_STAT_CHECKSUM
}com_rx_state;

typedef struct _com_rx_ctrl_block_t
{
    com_cmd_t cmd;
    com_rx_state stat;
    uint8_t length;
    uint8_t cnt;
    uint8_t *pData;
    uint8_t checkSum;
}com_rx_ctrl_block_t;

void Drv_Com_Init(com_event_callback_t callback );
void Drv_Com_Tx1_Enable(void );
void Drv_Com_Tx1_Send(uint8_t *buf, uint16_t length );
uint8_t Drv_Com_Tx1_Get_Send_State(void );
void Drv_Com_Tx1_Clr_Send_State(void );

void Drv_Com_Queue_Put(com_port_t com, uint8_t *buf, uint8_t length );
uint8_t Drv_Com_Queue_Get(com_port_t com, com_data_t *comData );

#endif 

