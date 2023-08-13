/********************************************************
* @file       app_com.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_com.h"
#include "app_event.h"
#include "app_batt.h"
#include "app_upgrade.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
const uint8_t fwVer[3] __attribute__((at(0x080010b8))) = {VER_BLD, VER_APP, VER_HW};
/* Private function -------------------------------------*/
static void App_ComTx1_Handler(void *arg );
static void App_ComTx2_Handler(void *arg );
static void App_Com6_Handler(void *arg );
static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length );
static void App_Com_Send_Upg_Msg(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/

static com_ctrl_block_t com0Ctrl;
static com_ctrl_block_t com1Ctrl;
static com_ctrl_block_t com2Ctrl;
static com_ctrl_block_t com6Ctrl;

static com_para_t comPara;

static uint8_t txFlag;

void App_Com_Init(void )
{
    Drv_Com_Init(App_Com_Send_Case_Msg, App_Com_Send_Upg_Msg);

    Drv_Task_Regist_Period(App_ComTx1_Handler, 0, 1, NULL);
    
    Drv_Task_Regist_Period(App_ComTx2_Handler, 0, 1, NULL);

    Drv_Task_Regist_Period(App_Com6_Handler, 0, 1, NULL);

    App_Com_Tx_Cmd_Get_Fw_Ver();
}

static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_CASE, buf, length);
}

static void App_Com_Send_Upg_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_UPG, buf, length);
}

static void App_ComTx1_Handler(void *arg )
{    
    switch(com1Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM1, &com1Ctrl.comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx_Enable(COM1);

                com1Ctrl.txCnt = 0;
                
                com1Ctrl.delayCnt = 0;

                com1Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            if(++com1Ctrl.delayCnt >= 10)
            {
                com1Ctrl.delayCnt = 0;
                
                Drv_Com_Tx_Send(COM1, com1Ctrl.comData.data, com1Ctrl.comData.length);

                com1Ctrl.comState = COM_STAT_TX_WATI_DONE;
            }

            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx_Get_State(COM1))
            {
                Drv_Com_Tx_Clr_State(COM1);

                com1Ctrl.delayCnt = 0;

                com1Ctrl.comState = COM_STAT_RX_RESPONSE;

                break;
            }
        }
        case COM_STAT_RX_RESPONSE:
        {
            if(com1Ctrl.rxDoneFlag)
            {
                com1Ctrl.rxDoneFlag = 0;
                
                com1Ctrl.delayCnt = 0;

                com1Ctrl.txCnt = 0;

                com1Ctrl.comState = COM_STAT_INIT;
            }
            #if 1
            else
            {
                if(++com1Ctrl.delayCnt >= 500)
                {
                    com1Ctrl.delayCnt = 0;

                    if(++com1Ctrl.txCnt < 3)
                    {
                        com1Ctrl.comState = COM_STAT_TX;
                    }
                    else
                    {
                        com1Ctrl.comState = COM_STAT_ERR;
                    }
                }
            }
            #endif 
            break;
        }
        case COM_STAT_ERR:
        {
            Drv_Com_Tx_Disable(COM1);
            
            com1Ctrl.rxDoneFlag = 0;

            com1Ctrl.delayCnt = 0;

            com1Ctrl.txCnt = 0;

            com1Ctrl.comState = COM_STAT_INIT;
            break;
        }
        default: break;
    }
}

static void App_ComTx2_Handler(void *arg )
{    
    switch(com2Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM2, &com2Ctrl.comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx_Enable(COM2);

                com2Ctrl.txCnt = 0;
                
                com2Ctrl.delayCnt = 0;

                com2Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            if(++com2Ctrl.delayCnt >= 5)
            {
                Drv_Com_Tx_Send(COM2, com2Ctrl.comData.data, com2Ctrl.comData.length);

                com2Ctrl.comState = COM_STAT_TX_WATI_DONE;
            }
            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx_Get_State(COM2))
            {
                Drv_Com_Tx_Clr_State(COM2);

                com2Ctrl.delayCnt = 0;

                com2Ctrl.comState = COM_STAT_RX_RESPONSE;

                break;
            }
        }
        case COM_STAT_RX_RESPONSE:
        {
            if(com2Ctrl.rxDoneFlag)
            {
                com2Ctrl.rxDoneFlag = 0;
                
                com2Ctrl.delayCnt = 0;

                com2Ctrl.txCnt = 0;

                com2Ctrl.comState = COM_STAT_INIT;
            }
            else
            {
                if(++com2Ctrl.delayCnt >= 100)
                {
                    com2Ctrl.delayCnt = 0;

                    if(++com2Ctrl.txCnt < 3)
                    {
                        com2Ctrl.comState = COM_STAT_TX;
                    }
                    else
                    {
                        com2Ctrl.comState = COM_STAT_ERR;
                    }
                }
            }
            break;
        }
        case COM_STAT_ERR:
        {
            Drv_Com_Tx_Disable(COM2);
            
            com2Ctrl.rxDoneFlag = 0;

            com2Ctrl.delayCnt = 0;

            com2Ctrl.txCnt = 0;

            com2Ctrl.comState = COM_STAT_INIT;
            break;
        }
        default: break;
    }
}

static void App_Com6_Handler(void *arg )
{    
    switch(com6Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM6, &com6Ctrl.comData) == COM_QUEUE_OK)
            {                
                com6Ctrl.delayCnt = 0;
                com6Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            Drv_Com_Tx_Send(COM6, com6Ctrl.comData.data, com6Ctrl.comData.length);
            
            com6Ctrl.comState = COM_STAT_TX_WATI_DONE;

            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx_Get_State(COM6))
            {
                Drv_Com_Tx_Clr_State(COM6);

                com6Ctrl.comState = COM_STAT_INIT;

                break;
            }
        }
        default: break;
    }
}

void App_Com_Set_Rx_Stat(uint8_t devType )
{
    if(devType == DEVICE_LEFT)
    {
        com1Ctrl.rxDoneFlag = 1;
    }
    else if(devType == DEVICE_RIGHT)
    {
        com2Ctrl.rxDoneFlag = 1;
    }
    else if(devType == DEVICE_MIDDLE)
    {
    }
}

void App_Com_Cmd_Case_Open_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    cmd_case_open_t cmdCaseOpen;

    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdCaseOpen +i)  = buf[i];
    }

    App_Ebud_Set_Level(cmdCaseOpen.devType, cmdCaseOpen.ebudBattLevel);
    
}

void App_Com_Cmd_Fw_Ver_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    cmd_fw_ver_t cmdFwVer;

    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdFwVer +i)  = buf[i];
    }
    
    App_Upg_Set_Fw_Ver(cmdFwVer.bldVer, cmdFwVer.appVer, cmdFwVer.hwVer);
}

void App_Com_Cmd_Fw_Info_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    word_t fwSize;
    cmd_fw_info_t cmdFwInfo;
    
    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdFwInfo +i)  = buf[i];
    }

    fwSize.byte_t.byte0 = cmdFwInfo.fwSizeByte0;
    fwSize.byte_t.byte1 = cmdFwInfo.fwSizeByte1;
    fwSize.byte_t.byte2 = cmdFwInfo.fwSizeByte2;
    fwSize.byte_t.byte3 = cmdFwInfo.fwSizeByte3;
    
    App_Upg_Set_Fw_Size(fwSize.val);
}

void App_Com_Cmd_Fw_Data_Response(uint8_t *buf, uint8_t length )
{
    App_Upg_Set_Fw_Data(&buf[3], length - 3);
}

void App_Com_Cmd_Fw_CRC_Response(uint8_t *buf, uint8_t length )
{
    uint16_t crc = 0;

    crc = (uint16_t )buf[4] << 8 | buf[3];
    
    App_Upg_Set_Fw_CRC(crc);
}

void App_Com_Tx_Cmd_Case_Open(uint8_t devType )
{
    uint8_t txBuf[15] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x0b;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CASE_OPEN;
    txBuf[8] = devType;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    txBuf[10] = VER_BLD;
    txBuf[11] = VER_APP;
    txBuf[12] = VER_HW;
    txBuf[13] = 0x00;
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[14] = checkSum;

    if(devType == DEVICE_LEFT)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RIGHT)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
}

void App_Com_Tx_Cmd_Case_Close(uint8_t devType )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CASE_CLOSE;
    txBuf[8] = devType;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    if(devType == DEVICE_LEFT)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RIGHT)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
}

void App_Com_Tx_Cmd_Chrg_Off(uint8_t devType, uint8_t ebudChrgOffReason)
{
    uint8_t txBuf[12] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x08;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_CHRG_OFF;
    txBuf[8] = devType;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    txBuf[10] = ebudChrgOffReason;
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[11] = checkSum;

    if(devType == DEVICE_LEFT)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RIGHT)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
}

void App_Com_Tx_Cmd_Get_Fw_Ver(void )
{
    uint8_t txBuf[14] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x0a;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_VER;
    txBuf[8] = 0x00;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    txBuf[10] = VER_BLD;
    txBuf[11] = VER_APP;
    txBuf[12] = VER_HW;
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[13] = checkSum;

    Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
}

void App_Com_Tx_Cmd_Get_Fw_Info(void )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_INFO;
    txBuf[8] = 0x00;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
}

void App_Com_Tx_Cmd_Get_Fw_Data(uint32_t fwOffset, uint32_t fwLen)
{
    uint8_t txBuf[15] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x0b;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_DATA;
    txBuf[8] = 0x00;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    #if 0
    txBuf[10]= (uint8_t )fwOffset;
    txBuf[11]= (uint8_t )(fwOffset >> 8);
    txBuf[12]= (uint8_t )(fwOffset >> 16);
    txBuf[13]= (uint8_t )(fwOffset >> 24);
    txBuf[14]= (uint8_t )(fwLen);
    txBuf[15]= (uint8_t )(fwLen >>8 );
    txBuf[16]= (uint8_t )(fwLen >> 16);
    txBuf[17]= (uint8_t )(fwLen >> 24);
    #else
    txBuf[10]= (uint8_t )fwOffset;
    txBuf[11]= (uint8_t )(fwOffset >> 8);
    txBuf[12]= (uint8_t )(fwLen);
    txBuf[13]= (uint8_t )(fwLen >>8 );
    #endif 
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[14] = checkSum;

    Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
}

void App_Com_Tx_Cmd_Get_Fw_CRC(void )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_CRC;
    txBuf[8] = 0x00;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
}

void App_Com_Upg_Tx_FwVer(void )
{
    uint8_t txBuf[8] = {0};
    uint8_t checksum = 0;

    txBuf[0] = 0x5a;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x06;
    txBuf[3] = CMD_FW_VERSION | CMD_MCU_TO_PC;
    txBuf[4] = FW_MAJOR_VER;
    txBuf[5] = FW_MINOR_VER;
    txBuf[6] = FW_BUILD_VER;

    for(int i = 0;i<txBuf[2]-1;i++)
    {
        checksum += txBuf[i+2];
    }

    txBuf[7] = (char)checksum;

    Drv_Tx_Queue_Put(COM6, txBuf, sizeof(txBuf));
}


void App_Com_Upg_Tx_Ack(void )
{
    uint8_t txBuf[5] = {0};
    uint8_t checksum = 0;

    txBuf[0] = 0x5a;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x03;
    txBuf[3] = CDM_FW_ACK | CMD_MCU_TO_PC;

    for(int i = 0;i<txBuf[2]-1;i++)
    {
        checksum += txBuf[i+2];
    }

    txBuf[4] = (char)checksum;

    Drv_Tx_Queue_Put(COM6, txBuf, sizeof(txBuf));
}

