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
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Com1_Handler(void *arg );
static void App_Com_Rx_Handle(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/
static com_tx_state_t com1TxStat = COM_STAT_INIT;

void App_Com_Init(void )
{
    Drv_Com_Init(App_Com_Rx_Handle);

    Drv_Task_Regist_Period(App_Com1_Handler, 0, 1, NULL);
}

static void App_Com_Rx_Handle(uint8_t *buf, uint8_t length )
{
    Drv_Msg_Put(APP_EVENT_COM_RX, buf, length);
}

static void App_Com1_Handler(void *arg )
{
    static com_data_t comData;
    static uint16_t delayCnt;
    static uint8_t  txCnt;
    
    switch(com1TxStat)
    {
        case COM_STAT_INIT:
        {
            if(Drv_Com_Queue_Get(COM1, &comData) == COM_QUEUE_OK)
            {
                Drv_Com_Tx1_Enable();

                txCnt = 0;

                com1TxStat = COM_STAT_TX;
            }
            break;
        }
        case COM_STAT_TX:
        {
            Drv_Com_Tx1_Send(comData.data, comData.length);

            com1TxStat = COM_STAT_TX_WATI_DONE;

            break;
        }
        case COM_STAT_TX_WATI_DONE:
        {
            if(Drv_Com_Tx1_Get_Send_State())
            {
                Drv_Com_Tx1_Clr_Send_State();

                delayCnt = 0;

                com1TxStat = COM_STAT_RX_RESPONSE;

                break;
            }
        }
        case COM_STAT_RX_RESPONSE:
        {
            break;
            
        }
        default: break;
    }
}



