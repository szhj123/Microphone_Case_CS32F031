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
#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Batt_Handler(void *arg );
static void App_Batt_Dischrg_Handler(void );
static void App_Batt_Chrg_Handler(void );
/* Private variables ------------------------------------*/
batt_ctrl_block_t battCtrl;
task_ctrl_block_t *taskBatt = NULL;

void App_Batt_Init(void )
{
    Drv_Batt_Init();

    Drv_Chrg_Init();

    taskBatt = Drv_Task_Regist_Period(App_Batt_Handler, 500, 1, NULL);
}

static void App_Batt_Handler(void *arg )
{
    if(battCtrl.delayCnt < 0xffff)
    {
        battCtrl.delayCnt++;
    }

    if(Drv_Chrg_Get_Usb_State() == USB_PLUG_OUT)
    {
        App_Batt_Dischrg_Handler();
    }
    else
    {
        App_Batt_Chrg_Handler();
    }
}

static void App_Batt_Dischrg_Handler(void )
{
    battCtrl.chrgStat = BATT_CHRG_INIT;

    switch(battCtrl.dischrgStat)
    {
        case BATT_DISCHRG_INIT:
        {
            if(battCtrl.battLevel == BATT_LEVEL_0)
            {
                battCtrl.battVol = Drv_Batt_Get_Vol();
                
                battCtrl.battSaveVol = battCtrl.battVol;
                
                battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

                battCtrl.battSaveLevel  = battCtrl.battLevel;
            }
            
            Drv_Msg_Put(APP_EVENT_USB_PLUG_OUT, NULL, 0);

            battCtrl.delayCnt = 0;

            battCtrl.dischrgStat = BATT_DISCHRG_PROCESS;
            
            break;
        }
        case BATT_DISCHRG_PROCESS:
        {
            if(battCtrl.delayCnt > 1000)
            {
                battCtrl.delayCnt = 0;
                
                battCtrl.battVol = Drv_Batt_Get_Vol();
                    
                battCtrl.battSaveVol = battCtrl.battVol;

                battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

                if(battCtrl.battLevel < battCtrl.battSaveLevel)
                {
                    battCtrl.battSaveLevel = battCtrl.battLevel;

                    if(battCtrl.battLevel == BATT_LEVEL_0);
                    {
                        Drv_Msg_Put(APP_EVENT_SYS_SLEEP, NULL, 0);
                    }
                }
            }
            break;
        }
        default: break;
    }
}

static void App_Batt_Chrg_Handler(void )
{
    battCtrl.dischrgStat = BATT_DISCHRG_INIT;
    
    switch(battCtrl.chrgStat)
    {
        case BATT_CHRG_INIT:
        {            
            if(battCtrl.battLevel == BATT_LEVEL_0)
            {
                battCtrl.battVol = Drv_Batt_Get_Vol();
            
                battCtrl.battSaveVol = battCtrl.battVol;
                
                battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

                battCtrl.battSaveLevel  = battCtrl.battLevel;
            }

            Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);

            battCtrl.delayCnt = 0;
            
            battCtrl.chrgStat = BATT_CHRG_GET_VOL_ERR;
            
            break;
        }
        case BATT_CHRG_GET_VOL_ERR:
        {
            if(battCtrl.delayCnt > 5000)
            {                
                battCtrl.delayCnt = 0;
                
                battCtrl.battVol = Drv_Batt_Get_Vol();

                if(battCtrl.battVol > battCtrl.battSaveVol)
                {
                    battCtrl.battErrVol = battCtrl.battVol - battCtrl.battSaveVol;
                }
                else
                {
                    battCtrl.battErrVol = 0;
                }
                
                battCtrl.chrgStat = BATT_CHRG_PROCESS;
            }            
            break;
        }
        case BATT_CHRG_PROCESS:
        {
            if(battCtrl.delayCnt > 5000)
            {
                battCtrl.delayCnt = 0;
                
                battCtrl.battVol = Drv_Batt_Get_Vol();
    
                if(battCtrl.battVol > 4195)
                {
                    if(battCtrl.battErrVol > 0)
                    {
                        battCtrl.battErrVol--;
                    }
                }
    
                battCtrl.battVol -= battCtrl.battErrVol;
    
                battCtrl.battSaveVol = battCtrl.battVol;
    
                battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);
    
                if(battCtrl.battLevel > battCtrl.battSaveLevel)
                {
                    battCtrl.battSaveLevel = battCtrl.battLevel;
    
                    Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);
                }
            }

            if(Drv_Chrg_Get_Charging_State() == CHRG_TERMINATION)
            {
                battCtrl.battLevel = BATT_LEVEL_100;
                
                battCtrl.battSaveLevel = battCtrl.battLevel;
                
                Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);

                battCtrl.delayCnt = 0;
                
                battCtrl.chrgStat = BATT_CHRG_DONE;
            }
            
            break;
        }
        case BATT_CHRG_DONE:
        {
            if(battCtrl.delayCnt > 5000)
            {
                battCtrl.delayCnt  = 0;
                
                battCtrl.battVol = Drv_Batt_Get_Vol();
                
                battCtrl.battSaveVol = battCtrl.battVol;
            }            
            break;
        }
        default: break;
    }
}

uint16_t App_Batt_Get_Vol(void )
{
    return Drv_Batt_Get_Vol();
}

batt_level_t App_Batt_Cal_Level(uint16_t battVol )
{
    const uint16_t battErrVol = 25;//mv
    
    if(battCtrl.battLevel == BATT_LEVEL_0)
    {
        if(battVol > BATT_VOL_80)
        {
            battCtrl.battLevel = BATT_LEVEL_81_99;
        }
        else if(battVol > BATT_VOL_50 && battVol <= BATT_VOL_80)
        {
            battCtrl.battLevel = BATT_LEVEL_51_80;
        }
        else if(battVol > BATT_VOL_20 && battVol <= BATT_VOL_50)
        {
            battCtrl.battLevel = BATT_LEVEL_21_50;
        }
        else
        {
            battCtrl.battLevel = BATT_LEVEL_1_20;
        }
    }
    else
    {
        if(battCtrl.battLevel == BATT_LEVEL_81_99)
        {
            if(battVol < (BATT_VOL_80- battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_51_80;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_51_80)
        {
            if(battVol > (BATT_VOL_80 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_81_99;
            }
            else if(battVol < (BATT_VOL_50 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_21_50;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_21_50)
        {
            if(battVol > (BATT_VOL_50 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_51_80;
            }
            else if(battVol < (BATT_VOL_20 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_1_20;
            }
        }
        else
        {
            if(battVol > (BATT_VOL_20 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_21_50;
            }
            else if(battVol <= BATT_VOL_3)
            {
                battCtrl.battLevel = BATT_LEVEL_0;
            }
        }
    }

    return battCtrl.battLevel;
}


