#ifndef _APP_COM_H
#define _APP_COM_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_com.h"
#include "drv_event.h"
#include "drv_flash.h"

#define VER_BLD                           0x01
#define VER_APP                           0x01
#define VER_HARDWARE                      0x01

#define FW_BUILD_VER                      0x00
#define FW_MINOR_VER                      0x00
#define FW_MAJOR_VER                      0x01

#define CMD_OPEN_CASE                     0x02
#define CMD_CLOSE_CASE                    0x03
#define CMD_CHARGER_OFF                   0x04
#define CMD_CHARGER_KEY                   0x05
#define CMD_EARBUD_BATT_LEVEL             0x06

#define CMD_MCU_TO_PC                     0x80
#define CMD_PC_TO_MCU                     0x00

#define CMD_FW_ERASE                      0x75
#define CMD_FW_DATA                       0x76
#define CMD_FW_CHECKSUM                   0x77
#define CDM_FW_ACK                        0x78
#define CMD_FW_VERSION                    0x79
#define CMD_FW_RESET                      0x7a


#define DEVICE_LEFT                       0x00
#define DEVICE_RIGHT                      0x01
#define DEVICE_MIDDLE                     0x02

typedef enum _com_state_t
{
    COM_STAT_INIT = 0,
    COM_STAT_TX,
    COM_STAT_TX_WATI_DONE,
    COM_STAT_RX_RESPONSE,
    COM_STAT_ERR    
}com_state_t;

typedef struct _rx_response_t
{
    uint8_t cmd;
    uint8_t devType;
    uint8_t devBattLevel;    
}rx_response_t;

typedef struct _com_ctrl_block_t
{
    com_state_t comState;
    com_data_t comData;
    uint16_t delayCnt;
    uint8_t  txCnt;
    uint8_t  rxDoneFlag;
}com_ctrl_block_t;

void App_Com_Init(void );
void App_Com_Set_Rx_Response(rx_response_t rxResponse );
void App_Com_Case_Tx_Open(uint8_t devType );
void App_Com_Upg_Tx_FwVer(void );
void App_Com_Upg_Tx_Ack(void );

#endif 

