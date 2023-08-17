#ifndef _APP_COM_H
#define _APP_COM_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_com.h"
#include "drv_event.h"
#include "drv_flash.h"

#define VER_BLD                           0x01
#define VER_APP                           0x01
#define VER_HW                            0x01

#define FW_BUILD_VER                      0x02
#define FW_MINOR_VER                      0x00
#define FW_MAJOR_VER                      0x01

#define CMD_CASE_OPEN                     0x02
#define CMD_CASE_CLOSE                    0x03
#define CMD_CHRG_OFF                      0x04
#define CMD_CHRG_KEY                      0x05
#define CMD_CASE_BATT                     0x06
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

typedef struct _cmd_case_t
{
    uint8_t cmd;
    uint8_t devType;
    uint8_t ebudBattLevel;    
}cmd_case_t;

typedef struct _cmd_fw_ver_t
{
    uint8_t cmd;
    uint8_t devType;
    uint8_t ebudBattLevel;    
    uint8_t verBld;
    uint8_t verApp;
    uint8_t verHw;
}cmd_fw_ver_t;

typedef struct _cmd_fw_info_t
{
    uint8_t cmd;
    uint8_t devType;
    uint8_t ebudBattLevel;
    uint8_t fwSizeByte_L;
    uint8_t fwSizeByte_H;
}cmd_fw_info_t;

typedef struct _com_ctrl_block_t
{
    com_port_t  comPort;
    com_state_t comState;
    com_data_t  comData;
    uint16_t    delayCnt;
    uint8_t     txCnt;
    uint8_t     rxDoneFlag;   
}com_ctrl_block_t;

void App_Com_Init(void );
void App_Com_Tx_Cmd_Case_Open(uint8_t devType );
void App_Com_Tx_Cmd_Case_Close(uint8_t devType );
void App_Com_Tx_Cmd_Chrg_Off(uint8_t devType, uint8_t ebudChrgOffReason);
void App_Com_Tx_Cmd_Get_Sirk(uint8_t devType );
void App_Com_Tx_Cmd_Random_Sirk(void );
void App_Com_Tx_Cmd_Get_Fw_Ver(void );
void App_Com_Tx_Cmd_Get_Fw_Size(void );
void App_Com_Tx_Cmd_Get_Fw_Data(uint32_t fwOffset, uint32_t fwLen);
void App_Com_Tx_Cmd_Get_Fw_CRC(void );


void App_Com_Set_Rx_Stat(uint8_t devType );

void App_Com_Cmd_Case_Response(uint8_t *buf, uint8_t length );
void App_Com_Cmd_Fw_Ver_Response(uint8_t *buf, uint8_t length );
void App_Com_Cmd_Fw_Size_Response(uint8_t *buf, uint8_t length );
void App_Com_Cmd_Fw_Data_Response(uint8_t *buf, uint8_t length );
void App_Com_Cmd_Fw_CRC_Response(uint8_t *buf, uint8_t length );

#endif 

