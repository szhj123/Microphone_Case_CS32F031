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
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static task_ctrl_block_t taskCom = NULL;
static com_tx_state_t comTxStat = COM_STAT_ENTRY;

void App_Com_Init(void )
{
    Drv_Com_Init();

    taskCom = Drv_Task_Regist_Period(App_Com_Handler, 0, 1, NULL);
}

static void App_Com_Handler(void *arg )
{
    static com_data_t comData;
    
    switch(comTxStat)
    {
        case COM_STAT_ENTRY:
        {
            if(Drv_Com_Queue_Get(&comData) == COM_QUEUE_OK)
            {
                if((com_port_t )comData.com == COM0)
                {
                    
                }
                else if((com_port_t )comData.com == COM1)
                {
                    Drv_Com_Tx1_Enable();
                }
                else if((com_port_t )comData.com == COM2)
                {
                }
                else if((com_port_t )comData.com == COM3)
                {
                }
            }
            break
        }
        case COM_STAT_PROCESS:
        {
            
        }
        default: break;
    }
}

