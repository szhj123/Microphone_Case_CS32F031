#ifndef _APP_COM_H
#define _APP_COM_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_com.h"
#include "drv_event.h"
#include "drv_flash.h"

#define VER_BLD                           0x00
#define VER_APP                           0x00
#define VER_HARDWARE                      0x00

#define FW_BUILD_VER                      0x02
#define FW_MINOR_VER                      0x00
#define FW_MAJOR_VER                      0x01

#define CMD_CASE_OPEN                     0x02
#define CMD_CASE_CLOSE                    0x03
#define CMD_CHRG_OFF                      0x04
#define CMD_CHRG_KEY                      0x05
#define CMD_EARBUD_BATT_LEVEL             0x06
#define CMD_GET_SIRK                      0x10
#define CMD_SET_SIRK                      0x11
#define CMD_GET_RANDOM_SIRK               0x12
#define CMD_GET_FW_SIZE                   0x13
#define CMD_GET_FW_DATA                   0x14
#define CMD_GET_FW_CRC                    0x15
#define CMD_GET_FW_VER                    0x16

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

typedef struct _cmd_case_open_t
{
    uint8_t devType;
    uint8_t ebudBattLevel;    
    uint8_t bldVer;
    uint8_t appVer;
    uint8_t hwVer;
}cmd_case_open_t;

typedef struct _com_ctrl_block_t
{
    com_state_t comState;
    com_data_t  comData;
    uint16_t    delayCnt;
    uint8_t     txCnt;
    uint8_t     rxDoneFlag;   
}com_ctrl_block_t;

typedef struct _com_para_t
{
    uint8_t ebudLBattLevel;
    uint8_t ebudRBattLevel;
    uint8_t ebudMBattLevel;
    uint8_t bldVer;
    uint8_t appVer;
    uint8_t hwVer;
    uint8_t ebudLSirkBuf[8];
    uint8_t ebudRSirkBuf[8];
    uint8_t ebudMSirkBuf[8];
    uint8_t randomSirkBuf[8];
    
}com_para_t;

void App_Com_Init(void );
void App_Com_Tx_Cmd_Case_Open(uint8_t devType );
void App_Com_Tx_Cmd_Case_Close(uint8_t devType );
void App_Com_Tx_Cmd_Chrg_Off(uint8_t devType, uint8_t ebudChrgOffReason);
void App_Com_Tx_Cmd_Get_Ver(void );

void App_Com_Upg_Tx_FwVer(void );
void App_Com_Upg_Tx_Ack(void );

void App_Com_Case_Open_Response(uint8_t *buf, uint8_t length );


#endif 

