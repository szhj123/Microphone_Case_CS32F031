/********************************************************
* @file       app_upgrade.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_upgrade.h"
#include "app_hall.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Upg_Handler(void *arg );
/* Private variables ------------------------------------*/
static upg_para_t upgPara;

void App_Upg_Init(void )
{
    Drv_Task_Regist_Period(App_Upg_Handler, 0, 1, NULL);
}

static void App_Upg_Handler(void *arg )
{
    switch(upgPara.stat)
    {
        case UPG_STAT_GET_VER:
        {
            if(App_Hall_Get_State() == HALL_OPEN)
            {
                App_Com_Tx_Cmd_Get_Ver();
            }
            break;
        }
        default: break;
    }
}

void App_Upg_Set_Ver(uint8_t bldVer, uint8_t appVer, uint8_t hwVer )
{
    upgPara.bldVer = bldVer;
    upgPara.appVer = appVer;
    upgPara.hwVer  = hwVer;
}

