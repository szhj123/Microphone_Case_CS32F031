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
static void App_ComTx_Handler(void *arg );

static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/

static com_ctrl_block_t com1Ctrl = 
{
    .comPort = COM1,
    .comState = COM_STAT_INIT,
    .rxDoneFlag = 0,
};
static com_ctrl_block_t com2Ctrl =
{
    .comPort = COM2,
    .comState = COM_STAT_INIT,
    .rxDoneFlag = 0,
};
static com_ctrl_block_t com3Ctrl = 
{
    .comPort = COM3,
    .comState = COM_STAT_INIT,
    .rxDoneFlag = 0,
};

void App_Com_Init(void )
{
    Drv_Com_Init(App_Com_Send_Case_Msg);

    Drv_Task_Regist_Period(App_ComTx_Handler, 0, 1, (void *)&com1Ctrl);
    
    Drv_Task_Regist_Period(App_ComTx_Handler, 0, 1, (void *)&com2Ctrl);
    
    Drv_Task_Regist_Period(App_ComTx_Handler, 0, 1, (void *)&com3Ctrl);

    App_Com_Tx_Cmd_Get_Fw_Ver();
}

static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_CASE, buf, length);
}

static void App_ComTx_Handler(void *arg )
{    
    com_ctrl_block_t *comCtrl = (com_ctrl_block_t *)arg;
    
    switch(comCtrl->comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(comCtrl->comPort, &comCtrl->comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx_Enable(comCtrl->comPort);

                comCtrl->txCnt = 0;
                
                comCtrl->delayCnt = 0;

                comCtrl->comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            if(++comCtrl->delayCnt >= 50)
            {
                comCtrl->delayCnt = 0;
                
                Drv_Com_Tx_Send(comCtrl->comPort, comCtrl->comData.data, comCtrl->comData.length);

                comCtrl->comState = COM_STAT_TX_WATI_DONE;
            }

            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx_Get_State(comCtrl->comPort))
            {
                Drv_Com_Tx_Clr_State(comCtrl->comPort);

                comCtrl->delayCnt = 0;

                comCtrl->comState = COM_STAT_RX_RESPONSE;

                break;
            }
        }
        case COM_STAT_RX_RESPONSE:
        {
            if(comCtrl->rxDoneFlag)
            {
                comCtrl->rxDoneFlag = 0;
                
                comCtrl->delayCnt = 0;

                comCtrl->txCnt = 0;

                comCtrl->comState = COM_STAT_INIT;
            }
            #if 1
            else
            {
                if(++comCtrl->delayCnt >= 500)
                {
                    comCtrl->delayCnt = 0;

                    if(++comCtrl->txCnt < 3)
                    {
                        comCtrl->comState = COM_STAT_TX;
                    }
                    else
                    {
                        comCtrl->comState = COM_STAT_ERR;
                    }
                }
            }
            #endif 
            break;
        }
        case COM_STAT_ERR:
        {
            Drv_Com_Tx_Disable(comCtrl->comPort);
            
            comCtrl->rxDoneFlag = 0;

            comCtrl->delayCnt = 0;

            comCtrl->txCnt = 0;

            comCtrl->comState = COM_STAT_INIT;
            break;
        }
        default: break;
    }
}

void App_Com_Set_Rx_Stat(uint8_t devType )
{
    if(devType == DEVICE_TX1)
    {
        com1Ctrl.rxDoneFlag = 1;
    }
    else if(devType == DEVICE_TX2)
    {
        com2Ctrl.rxDoneFlag = 1;
    }
    else if(devType == DEVICE_RX)
    {
        com3Ctrl.rxDoneFlag = 1;
    }
}

void App_Com_Cmd_Case_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    cmd_case_t cmdCase;

    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdCase +i)  = buf[i];
    }

    App_Ebud_Set_Level(cmdCase.devType, cmdCase.ebudBattLevel);

    DEBUG("response_cmd_case:%d, device type:%d, earbud battery level:%d\n", cmdCase.cmd, cmdCase.devType, cmdCase.ebudBattLevel);
    
}

void App_Com_Cmd_Get_Sirk_Response(uint8_t *buf, uint8_t length )
{
    uint8_t devType = buf[1];

    App_Sirk_Save_Data(devType, &buf[3], length - 3);
    
    DEBUG("response_cmd_Get_Sirk[%d]\n", devType);
}

void App_Com_Cmd_Set_Sirk_Response(uint8_t *buf, uint8_t length )
{
    uint8_t devType = buf[1];

    App_Sirk_Set_Response(devType);

    DEBUG("response_cmd_Set_Sirk[%d]\n", devType);
}

void App_Com_Cmd_Get_Random_Sirk_Response(uint8_t *buf, uint8_t length )
{
    App_Sirk_Save_Random_Data(&buf[3], length - 3);
    
    DEBUG("response_cmd_Get_Random_Sir\n");
}

void App_Com_Cmd_Fw_Ver_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    cmd_fw_ver_t cmdFwVer;

    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdFwVer +i)  = buf[i];
    }
    
    App_Upg_Set_Fw_Ver(cmdFwVer.verBld, cmdFwVer.verApp, cmdFwVer.verHw);

    DEBUG("response_cmd_fw_ver:%d, verBld=%d, verApp=%d, verHw=%d\n",cmdFwVer.cmd, cmdFwVer.verBld, cmdFwVer.verApp, cmdFwVer.verHw);
}

void App_Com_Cmd_Fw_Size_Response(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    uint16_t fwSize;
    cmd_fw_info_t cmdFwInfo;
    
    for(i=0;i<length;i++)
    {
        *((uint8_t *)&cmdFwInfo +i)  = buf[i];
    }

    fwSize = (uint16_t )cmdFwInfo.fwSizeByte_H <<8 | cmdFwInfo.fwSizeByte_L;

    App_Upg_Set_Fw_Size(fwSize);

    DEBUG("response_cmd_fw_size:%d, fw size:%d\n", cmdFwInfo.cmd, fwSize);
}

void App_Com_Cmd_Fw_Data_Response(uint8_t *buf, uint8_t length )
{
    App_Upg_Set_Fw_Data(&buf[3], length - 3);

    DEBUG("response_cmd_fw_data:%d, fw data: offset:%d, length:%d\n", buf[0], App_Upg_Get_Fw_Data_Offset(), length);
}

void App_Com_Cmd_Fw_CRC_Response(uint8_t *buf, uint8_t length )
{
    uint16_t crc = 0;

    crc = (uint16_t )buf[4] << 8 | buf[3];
    
    App_Upg_Set_Fw_CRC(crc);

    DEBUG("response_cmd_fw_crc:%d, fw crc:%d\n", buf[0], crc);
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
    txBuf[4] = 0x20;
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

    if(devType == DEVICE_TX1)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_TX2)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RX)
    {
        Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
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
    txBuf[4] = 0x20;
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

    if(devType == DEVICE_TX1)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_TX2)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RX)
    {
        Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
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
    txBuf[4] = 0x20;
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

    if(devType == DEVICE_TX1)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_TX2)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RX)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
}

void App_Com_Tx_Cmd_Get_Sirk(uint8_t devType )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_SIRK;
    txBuf[8] = devType;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    if(devType == DEVICE_TX1)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_TX2)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RX)
    {
        Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
    }
}

void App_Com_Tx_Cmd_Get_Random_Sirk(void )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_RANDOM_SIRK;
    txBuf[8] = DEVICE_RX;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
}

void App_Com_Tx_Cmd_Set_Sirk(uint8_t devType, uint8_t *buf, uint8_t length )
{
    uint8_t txBuf[19] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    if(length > SIRK_DATA_PACK_MAX_SIZE)
    {
        return ;
    }

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x0f;
    txBuf[3] = 0x00;
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_SET_SIRK;
    txBuf[8] = devType;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();

    for(i=0;i<length;i++)
    {
        txBuf[10+i] = buf[i];
    }
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10+length] = checkSum;
    
    if(devType == DEVICE_TX1)
    {
        Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_TX2)
    {
        Drv_Tx_Queue_Put(COM2, txBuf, sizeof(txBuf));
    }
    else if(devType == DEVICE_RX)
    {
        Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
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
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_VER;
    txBuf[8] = DEVICE_RX;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    txBuf[10] = VER_BLD;
    txBuf[11] = VER_APP;
    txBuf[12] = VER_HW;
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[13] = checkSum;

    Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
}

void App_Com_Tx_Cmd_Get_Fw_Size(void )
{
    uint8_t txBuf[11] = {0};
    uint8_t checkSum = 0;
    uint8_t i;

    txBuf[0] = 0x05;
    txBuf[1] = 0x5a;
    txBuf[2] = 0x07;
    txBuf[3] = 0x00;
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_SIZE;
    txBuf[8] = DEVICE_RX;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
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
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_DATA;
    txBuf[8] = DEVICE_RX;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    txBuf[10]= (uint8_t )fwOffset;
    txBuf[11]= (uint8_t )(fwOffset >> 8);
    txBuf[12]= (uint8_t )(fwLen);
    txBuf[13]= (uint8_t )(fwLen >>8 );
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[14] = checkSum;

    Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
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
    txBuf[4] = 0x20;
    txBuf[5] = 0x20;
    txBuf[6] = 0x01;
    txBuf[7] = CMD_GET_FW_CRC;
    txBuf[8] = DEVICE_RX;
    txBuf[9] = (uint8_t )App_Batt_Get_Level();
    
    for(i=0;i<sizeof(txBuf)-5;i++)
    {
        checkSum += txBuf[i+4];
    }

    txBuf[10] = checkSum;

    Drv_Tx_Queue_Put(COM3, txBuf, sizeof(txBuf));
}

