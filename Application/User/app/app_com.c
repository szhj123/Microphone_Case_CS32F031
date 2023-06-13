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
#include "app_com.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Com1_Handler(void *arg );
static void App_Com2_Handler(void *arg );
static void App_Com6_Handler(void *arg );
static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length );
static void App_Com_Send_Upg_Msg(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/

static com_ctrl_block_t com0Ctrl;
static com_ctrl_block_t com1Ctrl;
static com_ctrl_block_t com2Ctrl;
static com_ctrl_block_t com6Ctrl;

void App_Com_Init(void )
{
    Drv_Com_Init(App_Com_Send_Case_Msg, App_Com_Send_Upg_Msg);

    Drv_Task_Regist_Period(App_Com1_Handler, 0, 1, NULL);
    
    Drv_Task_Regist_Period(App_Com2_Handler, 0, 1, NULL);

    Drv_Task_Regist_Period(App_Com6_Handler, 0, 1, NULL);
}

static void App_Com_Send_Case_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_CASE, buf, length);
}

static void App_Com_Send_Upg_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_UPG, buf, length);
}

static void App_Com1_Handler(void *arg )
{    
    switch(com1Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM1, &com1Ctrl.comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx_Enable(COM1);

                com1Ctrl.txCnt = 0;

                com1Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            Drv_Com_Tx_Send(COM1, com1Ctrl.comData.data, com1Ctrl.comData.length);

            com1Ctrl.comState = COM_STAT_TX_WATI_DONE;

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
            else
            {
                if(++com1Ctrl.delayCnt >= 100)
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

static void App_Com2_Handler(void *arg )
{    
    switch(com2Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM2, &com2Ctrl.comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx_Enable(COM2);

                com2Ctrl.txCnt = 0;

                com2Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            Drv_Com_Tx_Send(COM2, com2Ctrl.comData.data, com2Ctrl.comData.length);

            com2Ctrl.comState = COM_STAT_TX_WATI_DONE;

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
            //if(++com6Ctrl.delayCnt >= 5)
            {
                com6Ctrl.delayCnt = 0;
                Drv_Com_Tx_Send(COM6, com6Ctrl.comData.data, com6Ctrl.comData.length);
                
                com6Ctrl.comState = COM_STAT_TX_WATI_DONE;
            }

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

void App_Com_Set_Rx_Response(rx_response_t rxResponse )
{
    switch(rxResponse.devType)
    {
        case DEVICE_RIGHT:
        {
            com2Ctrl.rxDoneFlag = 1;
            break;
        }
        case DEVICE_LEFT:
        {
            com1Ctrl.rxDoneFlag = 1;
            break;
        }
        case DEVICE_MIDDLE:
        {
            break;
        }
        default: break;
    }
}


void App_Com_Case_Tx_Open(uint8_t devType )
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
    txBuf[7] = CMD_OPEN_CASE;
    txBuf[8] = devType;
    txBuf[9] = 50;
    txBuf[10] = VER_BLD;
    txBuf[11] = VER_APP;
    txBuf[12] = VER_HARDWARE;
    txBuf[13] = 0x00;
    
    for(i=0;i<sizeof(txBuf);i++)
    {
        checkSum += txBuf[i];
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

