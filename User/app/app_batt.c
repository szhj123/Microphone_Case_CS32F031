/********************************************************
* @file       app_batt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_batt.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Batt_Handler(void *arg );
/* Private variables ------------------------------------*/
batt_ctrl_block_t battCtrl;


void App_Batt_Init(void )
{
    Drv_Batt_Init();

    Drv_Task_Regist_Period(App_Batt_Handler, 0, 1, NULL);
}

static void App_Batt_Handler(void *arg )
{
    if(battCtrl.delayCnt < 0xffff)
    {
        battCtrl.delayCnt++;
    }

    if(battCtrl.handler != NULL)
    {
        battCtrl.handler();
    }
}


