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
#include "app_com.h"
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
    if(upgPara.delayCnt < 0xffff)
    {
        upgPara.delayCnt++;
    }
    
    switch(upgPara.stat)
    {
        case UPG_STAT_GET_VER:
        {
            if(App_Hall_Get_State() == HALL_OPEN)
            {
                upgPara.appVer = 0;
                
                App_Com_Tx_Cmd_Get_Ver();

                upgPara.delayCnt = 0;
                
                upgPara.stat = UPG_STAT_COMPARE_VER;
            }
            break;
        }
        case UPG_STAT_COMPARE_VER:
        {
            if(upgPara.delayCnt > 500)
            {
                upgPara.delayCnt = 0;
                
                if(upgPara.appVer != 0)
                {
                    if(upgPara.appVer != App_Flash_Get_App_Ver())
                    {
                        upgPara.stat = UPG_STAT_START;
                    }
                    else
                    {
                        upgPara.stat = UPG_STAT_EXIT;
                    }
                }
                else
                {
                    upgPara.stat = UPG_STAT_GET_VER;
                }
            }
            break;
        }
        case UPG_STAT_START:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                if(upgPara.delayCnt > 30000)
                {
                    upgPara.delayCnt = 0;

                    //Todo: get firmware information from earbud
                    
                    upgPara.stat = UPG_STAT_GET_FW_INFO;
                }
            }
            else
            {
                upgPara.delayCnt = 0;
            }
            break;
        }
        case UPG_STAT_GET_FW_INFO:
        {
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

