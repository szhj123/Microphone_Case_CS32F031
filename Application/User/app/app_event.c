/********************************************************
* @file       app_event.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_event.h"
#include "app_batt.h"
#include "app_led.h"
#include "app_hall.h"
#include "app_com.h"
#include "app_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define SLEEP_OUT              0
#define SLEEP_IN               (!SLEEP_OUT)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Event_Handler(void *arg );
static void App_Event_Case_Handler(uint8_t *buf, uint8_t length );
static void App_Event_Send_Sleep(void *arg );
/* Private variables ------------------------------------*/
static uint8_t timerSleep = TIMER_NULL;
static uint8_t sleepStat = SLEEP_OUT;

void App_Event_Init(void )
{
    Drv_Task_Regist_Period(App_Event_Handler, 0, 1, NULL);
}

static void App_Event_Handler(void *arg )
{
    static msg_t msg;

    if(Drv_Msg_Get(&msg) == MSG_QUEUE_ERR)
    {
        return ;
    }

    switch((app_event_t )msg.cmd)
    {
        case APP_EVENT_USB_PLUG_OUT:
        {
            batt_level_t battLevel = App_Batt_Cal_Level(App_Batt_Get_Vol());

            App_Led_Hall_Open(battLevel);
            
            break;
        }
        case APP_EVENT_USB_PLUG_IN:
        {
            break;
        }
        case APP_EVENT_BATT_LEVEL:
        {
            batt_level_t battLevel = (batt_level_t )msg.data[0];
            
            if(battLevel == BATT_LEVEL_100)
            {
                App_Led_All_Turn_On();
            }
            else
            {
                App_Led_Flash_BattLevel(battLevel);
            }

            if(Drv_Chrg_Get_Usb_State() == USB_PLUG_OUT)
            {
                if(battLevel == BATT_LEVEL_0)
                {
                    uint8_t ebudChrgOffReason = REASON_BATT_LOW;
                    
                    App_Com_Tx_Cmd_Chrg_Off(DEVICE_TX1, ebudChrgOffReason);
                    
                    App_Com_Tx_Cmd_Chrg_Off(DEVICE_TX2, ebudChrgOffReason);

                    App_Com_Tx_Cmd_Chrg_Off(DEVICE_RX, ebudChrgOffReason);

                    if(sleepStat == SLEEP_OUT)
                    {
                        sleepStat = SLEEP_IN;
                        
                        Drv_Timer_Delete(timerSleep);
                        
                        timerSleep = Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 500, NULL);
                    }
                }
            }
            
            break;
        }
        case APP_EVENT_EBUD_TX1_CHRG_OFF:
        {
            uint8_t ebudChrgOffReason = msg.data[0];

            App_Com_Tx_Cmd_Chrg_Off(DEVICE_TX1, ebudChrgOffReason);
            
            break;
        }
        case APP_EVENT_EBUD_TX2_CHRG_OFF:
        {
            uint8_t ebudChrgOffReason = msg.data[0];
            
            App_Com_Tx_Cmd_Chrg_Off(DEVICE_TX2, ebudChrgOffReason);
            
            break;
        }
        case APP_EVENT_EBUD_RX_CHRG_OFF:
        {
            uint8_t ebudChrgOffReason = msg.data[0];
            
            App_Com_Tx_Cmd_Chrg_Off(DEVICE_RX, ebudChrgOffReason);
            
            break;
        }
        case APP_EVENT_HALL_STATE:
        {
            uint8_t hallState = msg.data[0];
            
            batt_level_t battLevel = App_Batt_Cal_Level(App_Batt_Get_Vol());

            if(Drv_Chrg_Get_Usb_State() == USB_PLUG_OUT)
            {
                if(hallState == HALL_OPEN)
                {
                    if(battLevel > BATT_LEVEL_0)
                    {
                        App_Led_Hall_Open(battLevel);
                        
                        App_Com_Tx_Cmd_Case_Open(DEVICE_TX1);
                        
                        App_Com_Tx_Cmd_Case_Open(DEVICE_TX2);

                        App_Com_Tx_Cmd_Case_Open(DEVICE_RX);
                    }
                }
                else
                {
                    if(battLevel > BATT_LEVEL_0)
                    {
                        App_Com_Tx_Cmd_Case_Close(DEVICE_TX1);
                        
                        App_Com_Tx_Cmd_Case_Close(DEVICE_TX2);
                        
                        App_Com_Tx_Cmd_Case_Close(DEVICE_RX);
                        
                        App_Led_Hall_Close();
                    }
                }
                
                App_Sleep_Enable();
            }
            break;
        }
        case APP_EVENT_COM_CASE:
        {
            App_Event_Case_Handler(msg.data, msg.lenght);
            
            break;
        }
        case APP_EVENT_SYS_SLEEP:
        {
            //todo: system sleep
            App_Event_Case_Sleep();
            break;
        }
        default: break;
    }
}

void App_Event_Case_Handler(uint8_t *buf, uint8_t length )
{
    uint8_t cmd = buf[0];
    uint8_t devType = buf[1];
    
    App_Com_Set_Rx_Stat(devType);

    switch(cmd )
    {
        case CMD_CASE_OPEN: 
        case CMD_CASE_CLOSE:
        case CMD_CHRG_OFF:
        case CMD_CASE_BATT:
        {
            App_Com_Cmd_Case_Response(buf, length );
            break;
        }
        case CMD_GET_SIRK:
        {
            App_Com_Cmd_Get_Sirk_Response(buf, length );
            break;
        }
        case CMD_SET_SIRK:
        {
            App_Com_Cmd_Set_Sirk_Response(buf, length );
            break;
        }
        case CMD_GET_RANDOM_SIRK:
        {
            App_Com_Cmd_Get_Random_Sirk_Response(buf, length );
            break;
        }
        case CMD_GET_FW_VER:
        {
            App_Com_Cmd_Fw_Ver_Response(buf, length );
            break;
        }
        case CMD_GET_FW_SIZE:
        {
            App_Com_Cmd_Fw_Size_Response(buf, length );
            break;
        }
        case CMD_GET_FW_DATA:
        {
            App_Com_Cmd_Fw_Data_Response(buf, length );
            break;
        }
        case CMD_GET_FW_CRC:
        {
            App_Com_Cmd_Fw_CRC_Response(buf, length );
            break;
        }
        default: break;
    }
}

void App_Sleep_Enable(void )
{
    if(Drv_Chrg_Get_Usb_State() == USB_PLUG_OUT)
    {
        if(App_Batt_Get_Level() == BATT_LEVEL_0)
        {
            if(sleepStat == SLEEP_OUT)
            {
                sleepStat = SLEEP_IN;
                
                Drv_Timer_Delete(timerSleep);

                timerSleep = Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 500, NULL);
            }
        }
        else
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                if(App_Ebud_Get_All_Chrg_Stat() == EBUD_CHRG_NONE || App_Ebud_Get_All_Chrg_Stat() == EBUD_CHRG_DONE)
                {   
                    if(sleepStat == SLEEP_OUT)
                    {
                        sleepStat = SLEEP_IN;
                        
                        Drv_Timer_Delete(timerSleep);

                        timerSleep = Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 500, NULL);
                    }
                }
            }
        }
    }
}

static void App_Event_Send_Sleep(void *arg )
{
    if(App_Ebud_Get_All_Chrg_Stat() == EBUD_CHRG_NONE || App_Ebud_Get_All_Chrg_Stat() == EBUD_CHRG_DONE || \
       App_Batt_Get_Level() == BATT_LEVEL_0)
    {
        Drv_Msg_Put(APP_EVENT_SYS_SLEEP, NULL, 0);
    }
}

void App_Event_Case_Sleep(void )
{
    /* Configure all GPIO as analog to reduce current consumption on non used IOs */
    /* Warning : Reconfiguring all GPIO will close the connetion with the debugger */

	__RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA|RCU_AHB_PERI_GPIOB|RCU_AHB_PERI_GPIOF);

    Drv_Chrg_Chg_Boost_Disable();
    
    //Drv_Chrg_Enter_Ship_Mode();

    App_Led_All_Turn_Off();

    #if 1
    gpio_mode_set(GPIOA,GPIO_PIN_ALL,GPIO_MODE_ANALOG);
    gpio_mode_set(GPIOB,GPIO_PIN_ALL,GPIO_MODE_ANALOG);
    gpio_mode_set(GPIOF,GPIO_PIN_ALL,GPIO_MODE_ANALOG); 
	
	__RCU_AHB_CLK_DISABLE(RCU_AHB_PERI_GPIOA|RCU_AHB_PERI_GPIOB|RCU_AHB_PERI_GPIOF);	

    __RCU_APB2_CLK_DISABLE(RCU_APB2_PERI_ADC);   
    
	Hal_Hall_Init();

    __RCU_APB1_CLK_ENABLE(RCU_APB1_PERI_PMU); // Enable the PMU clock
    
    pmu_deep_sleep_mode_enter(PMU_LDO_LOW_POWER, PMU_DSM_ENTRY_WFI); // enter STOP mode 
    #endif 

    SystemInit();
    
    Hal_Task_Init();

    Hal_Timer_Init();

    Hal_Com_Init();

    Hal_Hall_Init();

    Hal_Batt_Init();

    Hal_Batt_Chrg_Init();

    Drv_Chrg_Exit_Ship_Mode();

    Hal_Led_Init();


    sleepStat = SLEEP_OUT;
        
}




