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

    Drv_BATT_CHRG_INIT();

    taskBatt = Drv_Task_Regist_Period(App_Batt_Handler, 0, 1, NULL);
}

static void App_Batt_Handler(void *arg )
{
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
            
            battCtrl.dischrgStat = BATT_DISCHRG_PROCESS;

            Drv_Msg_Put(APP_EVENT_USB_PLUG_OUT, NULL, 0);

            Drv_Task_Delay(taskBatt, 5000);
            
            break;
        }
        case BATT_DISCHRG_PROCESS:
        {
            battCtrl.battVol = Drv_Batt_Get_Vol();
                
            battCtrl.battSaveVol = battCtrl.battVol;

            battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

            if(battCtrl.battLevel < battCtrl.battSaveLevel)
            {
                battCtrl.battSaveLevel = battCtrl.battLevel;

            }
            
            Drv_Task_Delay(taskBatt, 1000);
            
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
            battCtrl.battVol = Drv_Batt_Get_Vol();
            
            battCtrl.battSaveVol = battCtrl.battVol;
            
            if(battCtrl.battLevel == BATT_LEVEL_0)
            {
                battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

                battCtrl.battSaveLevel  = battCtrl.battLevel;
            }

            Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);

            battCtrl.chrgStat = BATT_CHRG_GET_VOL_ERR;

            Drv_Task_Delay(taskBatt, 5000);
            
            break;
        }
        case BATT_CHRG_GET_VOL_ERR:
        {
            battCtrl.battVol = Drv_Batt_Get_Vol();

            if(battCtrl.battVol > battCtrl.battSaveLevel)
            {
                battCtrl.battErrVol = battCtrl.battVol - battCtrl.battSaveLevel;
            }
            else
            {
                battCtrl.battErrVol = 0;
            }

            battCtrl.chrgStat = BATT_CHRG_PROCESS;
            
            Drv_Task_Delay(taskBatt, 1000);
            
            break;
        }
        case BATT_CHRG_PROCESS:
        {
            battCtrl.battVol = Drv_Batt_Get_Vol();

            if(battCtrl.battVol > 4180)
            {
                if(battCtrl.battErrVol > 0)
                {
                    battCtrl.battErrVol--;
                }
            }

            battCtrl.battVol -= battCtrl.battErrVol;

            battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

            if(battCtrl.battLevel > battCtrl.battSaveLevel)
            {
                battCtrl.battSaveLevel = battCtrl.battLevel;

                Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);
            }
            
            break;
        }
        default: break;
    }
}

batt_level_t App_Batt_Cal_Level(uint16_t battVol )
{
    const uint16_t battErrVol = 25;//mv
    
    if(battCtrl.battLevel == BATT_LEVEL_0)
    {
        if(battVol > BATT_VOL_80)
        {
            battCtrl.battLevel = BATT_LEVEL_81_100;
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
        if(battCtrl.battLevel == BATT_LEVEL_81_100)
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
                battCtrl.battLevel = BATT_LEVEL_81_100;
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

void App_Batt_Send_Level(void )
{
    Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);
}

