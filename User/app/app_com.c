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
static void App_Com_Send_Msg(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/

static com_ctrl_block_t com0Ctrl;
static com_ctrl_block_t com1Ctrl;
static com_ctrl_block_t com2Ctrl;

static void com_test(void *arg )
{
    App_Com_Tx_Open_Case();
}

void App_Com_Init(void )
{
    Drv_Com_Init(App_Com_Send_Msg);

    Drv_Task_Regist_Period(App_Com1_Handler, 0, 1, NULL);

    Drv_Timer_Regist_Oneshot(com_test, 1000, NULL);
}

static void App_Com_Send_Msg(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_RX, buf, length);
}

static void App_Com1_Handler(void *arg )
{    
    switch(com1Ctrl.comState)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Tx_Queue_Get(COM1, &com1Ctrl.comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx1_Enable();

                com1Ctrl.txCnt = 0;

                com1Ctrl.comState = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            Drv_Com_Tx1_Send(com1Ctrl.comData.data, com1Ctrl.comData.length);

            com1Ctrl.comState = COM_STAT_TX_WATI_DONE;

            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx1_Get_State())
            {
                Drv_Com_Tx1_Clr_State();

                com1Ctrl.delayCnt = 0;

                com1Ctrl.comState = COM_STAT_RX_RESPONSE;

                break;
            }
        }
        case COM_STAT_RX_RESPONSE:
        {
            if(com1Ctrl.rxDoneFlag)
            {
                
            }
            break;
            
        }
        default: break;
    }
}

void App_Com_Rx_Handler(uint8_t *buf, uint8_t length )
{
    rx_response_t rxResponse;
    uint8_t i;

    for(i=0;i<length-8;i++)
    {
        *((uint8_t *)&rxResponse + i) = buf[7+i];
    }

    switch(rxResponse.devType)
    {
        case DEVICE_RIGHT:
        {
            
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

void App_Com_Tx_Open_Case(void )
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
    txBuf[8] = DEVICE_LEFT;
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

    Drv_Tx_Queue_Put(COM1, txBuf, sizeof(txBuf));
    
}


