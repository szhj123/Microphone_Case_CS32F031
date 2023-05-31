#ifndef _DRV_COM_H
#define _DRV_COM_H

#include "hal_com.h"

typedef void (*com_event_callback_t)(uint8_t *, uint8_t );

#define TX_DATA_MAX_SIZE                32
#define TX_QUEUE_MAX_SIZE               4

#define COM_QUEUE_OK                     0
#define COM_QUEUE_ERR                    (!COM_QUEUE_OK)

typedef enum _com_port_t
{
    COM0 = 0,
    COM1,
    COM2,
    COM3,
    COM4,
    COM5,
    COM6,
    COM7,
    COM8
}com_port_t;

typedef struct _com_data_t
{
    uint8_t com;
    uint8_t data[TX_DATA_MAX_SIZE];
    uint8_t length;
}com_data_t;

typedef struct tx_queue_t
{
    com_data_t buf[TX_QUEUE_MAX_SIZE];
    uint8_t front;
    uint8_t rear;
}tx_queue_t;

typedef struct _com_cmd_t
{
    uint8_t header;
    uint8_t type;
    uint8_t length_l;
    uint8_t length_h;
    uint8_t id_l;
    uint8_t id_h;
    uint8_t flags;
    uint8_t payload[16];
    uint8_t checkSum;
}com_cmd_t;

typedef enum _rx_state
{
    RX_STAT_HEADER = 0,
    RX_STAT_TYPE,
    RX_STAT_LENGTH,
    RX_STAT_DATA,
    RX_STAT_CHECKSUM
}rx_state;

typedef struct _rx_ctrl_block_t
{
    rx_state stat;
    uint8_t  lengthIndex;
    uint16_t dataLength;
    uint16_t dataCnt;
    uint8_t  dataBuf[256];
    uint8_t checkSum;
}rx_ctrl_block_t;

void Drv_Com_Init(com_event_callback_t callback );
void Drv_Com_Tx_Enable(com_port_t com );
void Drv_Com_Tx_Disable(com_port_t com );
void Drv_Com_Tx_Send(com_port_t com, uint8_t *buf, uint16_t length );
uint8_t Drv_Com_Tx_Get_State(com_port_t com );
void Drv_Com_Tx_Clr_State(com_port_t com );

void Drv_Tx_Queue_Put(com_port_t com, uint8_t *buf, uint8_t length );
uint8_t Drv_Tx_Queue_Get(com_port_t com, com_data_t *txData );

#endif 

