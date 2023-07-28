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
static void App_Ebud_Chrg_Handler(void );
/* Private variables ------------------------------------*/
batt_ctrl_block_t battCtrl;
task_ctrl_block_t *taskBatt = NULL;

void App_Batt_Init(void )
{
    Drv_Batt_Init();

    Drv_Chrg_Init();

    Drv_Chrg_Boost_Enable();

    taskBatt = Drv_Task_Regist_Period(App_Batt_Handler, 1000, 1, NULL);
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

    App_Ebud_Chrg_Handler();
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
            
            Drv_Chrg_Chg_Disable();
            
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

                    if(battCtrl.battLevel == BATT_LEVEL_0)
                    {
                        Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);
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
            battCtrl.battVol = Drv_Batt_Get_Vol();
            
            battCtrl.battSaveVol = battCtrl.battVol;
            
            battCtrl.battLevel = App_Batt_Cal_Level(battCtrl.battVol);

            battCtrl.battSaveLevel  = battCtrl.battLevel;

            Drv_Msg_Put(APP_EVENT_BATT_LEVEL, (const uint8_t *)&battCtrl.battLevel, 1);

            Drv_Chrg_Chg_Enable();
            
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
    
                if(battCtrl.battVol > 4190)
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

static void App_Ebud_Chrg_Handler(void )
{
    static uint8_t ebudDetCnt;
    static uint32_t ebudTx1CurSum;
    static uint32_t ebudTx2CurSum;
    static uint32_t ebudRxCurSum;

    if(ebudDetCnt < 100)
    {
        ebudTx1CurSum += Drv_Ebud_Get_Tx1_Cur();

        ebudTx2CurSum += Drv_Ebud_Get_Tx2_Cur();

        ebudRxCurSum += Drv_Ebud_Get_Rx_Cur();

        ebudDetCnt++;
    }
    else
    {
        battCtrl.ebudTx1Cur = ebudTx1CurSum / 100;
        battCtrl.ebudTx2Cur = ebudTx2CurSum / 100;
        battCtrl.ebudRxCur = ebudRxCurSum / 100;

        ebudTx1CurSum = 0;
        ebudTx2CurSum = 0;
        ebudRxCurSum = 0;
        ebudDetCnt = 0;

        if(battCtrl.ebudTx1Stat == EBUD_CHRG_PROCESS)
        {
            if(battCtrl.ebudTx1Cur <= 7)
            {
                battCtrl.ebudTx1Stat = EBUD_CHRG_DONE;

                battCtrl.ebudTx1ChrgOffReason = REASON_CHRG_FULL;

                Drv_Msg_Put(APP_EVENT_EBUD_TX1_CHRG_OFF, (const uint8_t *)&battCtrl.ebudTx1ChrgOffReason, 1);
            }
        }
        else
        {
            if(battCtrl.ebudTx1Cur >= 10)
            {
                battCtrl.ebudTx1Stat = EBUD_CHRG_PROCESS;
            }
        }

        if(battCtrl.ebudTx2Stat == EBUD_CHRG_PROCESS)
        {
            if(battCtrl.ebudTx2Cur <= 7)
            {
                battCtrl.ebudTx2Stat = EBUD_CHRG_DONE;
                
                battCtrl.ebudTx2ChrgOffReason = REASON_CHRG_FULL;
                
                Drv_Msg_Put(APP_EVENT_EBUD_TX2_CHRG_OFF, (const uint8_t *)&battCtrl.ebudTx2ChrgOffReason, 1);
            }
        }
        else
        {
            if(battCtrl.ebudTx2Cur >= 10)
            {
                battCtrl.ebudTx2Stat = EBUD_CHRG_PROCESS;
            }
        }

        if(battCtrl.ebudRxStat == EBUD_CHRG_PROCESS)
        {
            if(battCtrl.ebudRxCur <= 7)
            {
                battCtrl.ebudRxStat = EBUD_CHRG_DONE;

                battCtrl.ebudRxChrgOffReason = REASON_CHRG_FULL;

                Drv_Msg_Put(APP_EVENT_EBUD_RX_CHRG_OFF, (const uint8_t *)&battCtrl.ebudRxChrgOffReason, 1);
            }
        }
        else
        {
            if(battCtrl.ebudRxCur >= 10)
            {
                battCtrl.ebudRxStat = EBUD_CHRG_PROCESS;
            }
        }
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
        if(battVol > BATT_VOL_90)
        {
            battCtrl.battLevel = BATT_LEVEL_91_99;
        }
        else if(battVol > BATT_VOL_80 && battVol <= BATT_VOL_90)
        {
            battCtrl.battLevel = BATT_LEVEL_81_90;
        }
        else if(battVol > BATT_VOL_70 && battVol <= BATT_VOL_80)
        {
            battCtrl.battLevel = BATT_LEVEL_71_80;
        }
        else if(battVol > BATT_VOL_60 && battVol <= BATT_VOL_70)
        {
            battCtrl.battLevel = BATT_LEVEL_61_70;
        }
        else if(battVol > BATT_VOL_50 && battVol <= BATT_VOL_60)
        {
            battCtrl.battLevel = BATT_LEVEL_51_60;
        }
        else if(battVol > BATT_VOL_40 && battVol <= BATT_VOL_50)
        {
            battCtrl.battLevel = BATT_LEVEL_41_50;
        }
        else if(battVol > BATT_VOL_30 && battVol <= BATT_VOL_40)
        {
            battCtrl.battLevel = BATT_LEVEL_31_40;
        }
        else if(battVol > BATT_VOL_20 && battVol <= BATT_VOL_30)
        {
            battCtrl.battLevel = BATT_LEVEL_21_30;
        }
        else if(battVol > BATT_VOL_10 && battVol <= BATT_VOL_20)
        {
            battCtrl.battLevel = BATT_LEVEL_11_20;
        }
        else if(battVol > BATT_VOL_3 && battVol <= BATT_VOL_10)
        {
            battCtrl.battLevel = BATT_LEVEL_1_10;
        }
        else
        {
            battCtrl.battLevel = BATT_LEVEL_0;
        }
    }
    else
    {
        if(battCtrl.battLevel == BATT_LEVEL_91_99)
        {
            if(battVol < (BATT_VOL_90- battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_81_90;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_81_90)
        {
            if(battVol > (BATT_VOL_90 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_91_99;
            }
            else if(battVol < (BATT_VOL_80 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_71_80;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_71_80)
        {
            if(battVol > (BATT_VOL_80 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_81_90;
            }
            else if(battVol < (BATT_VOL_70 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_61_70;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_61_70)
        {
            if(battVol > (BATT_VOL_70 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_71_80;
            }
            else if(battVol < (BATT_VOL_60 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_51_60;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_51_60)
        {
            if(battVol > (BATT_VOL_60 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_61_70;
            }
            else if(battVol < (BATT_VOL_50 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_41_50;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_41_50)
        {
            if(battVol > (BATT_VOL_50 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_51_60;
            }
            else if(battVol < (BATT_VOL_40 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_31_40;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_31_40)
        {
            if(battVol > (BATT_VOL_40 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_41_50;
            }
            else if(battVol < (BATT_VOL_30 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_21_30;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_21_30)
        {
            if(battVol > (BATT_VOL_30 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_31_40;
            }
            else if(battVol < (BATT_VOL_20 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_11_20;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_11_20)
        {
            if(battVol > (BATT_VOL_20 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_21_30;
            }
            else if(battVol < (BATT_VOL_10 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_1_10;
            }
        }
        else if(battCtrl.battLevel == BATT_LEVEL_1_10)
        {
            if(battVol > (BATT_VOL_10 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_11_20;
            }
            else if(battVol < (BATT_VOL_3 - battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_0;
            }
        }
        else
        {
            if(battVol > (BATT_VOL_5 + battErrVol))
            {
                battCtrl.battLevel = BATT_LEVEL_1_10;
            }
        }
    }

    return battCtrl.battLevel;
}

batt_level_t App_Batt_Get_Level(void )
{
	return battCtrl.battLevel;
}

ebud_charging_stat_t App_Ebud_Get_Chrg_State(void )
{
    ebud_charging_stat_t retVal;
    
    if(battCtrl.ebudTx1Stat == EBUD_CHRG_DONE && battCtrl.ebudTx2Stat == EBUD_CHRG_DONE && battCtrl.ebudRxStat == EBUD_CHRG_DONE)
    {
        retVal = EBUD_CHRG_DONE;
    }
    else
    {
        retVal = EBUD_CHRG_PROCESS;
    }

    return retVal;
}


