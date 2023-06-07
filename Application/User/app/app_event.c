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
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Event_Handler(void *arg );
/* Private variables ------------------------------------*/

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
                    App_Led_Hall_Open(battLevel);
                    
                    App_Com_Tx_Open_Case(DEVICE_LEFT);
                    
                    App_Com_Tx_Open_Case(DEVICE_RIGHT);
                }
                else
                {
                    App_Led_Hall_Close();
                }
            }
            break;
        }
        case APP_EVENT_COM_CASE:
        {
            App_Com_Case_Handler(msg.data, msg.lenght);
            
            break;
        }
        case APP_EVENT_COM_UPG:
        {
            App_Com_Upg_Handler(msg.data, msg.lenght);
            break;
        }
        case APP_EVENT_SYS_SLEEP:
        {
            //todo: system sleep
            break;
        }
        default: break;
    }
}

