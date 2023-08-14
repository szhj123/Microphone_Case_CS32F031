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
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Event_Handler(void *arg );
static void App_Event_Case_Handler(uint8_t *buf, uint8_t length );
static void App_Event_Send_Sleep(void *arg );

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

            if(Drv_Chrg_Get_Usb_State() == USB_PLUG_OUT)
            {
                if(battLevel == BATT_LEVEL_0)
                {
                    uint8_t ebudChrgOffReason = REASON_BATT_LOW;
                    
                    App_Com_Tx_Cmd_Chrg_Off(DEVICE_LEFT, ebudChrgOffReason);
                    
                    App_Com_Tx_Cmd_Chrg_Off(DEVICE_RIGHT, ebudChrgOffReason);

                    Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 250, NULL);
                }
            }
            
            break;
        }
        case APP_EVENT_EBUD_TX1_CHRG_OFF:
        {
            uint8_t ebudChrgOffReason = msg.data[0];
            
            App_Com_Tx_Cmd_Chrg_Off(DEVICE_LEFT, ebudChrgOffReason);

            if(App_Ebud_Get_Chrg_State() == EBUD_CHRG_DONE)
            {
                Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 250, NULL);
            }
            
            break;
        }
        case APP_EVENT_EBUD_TX2_CHRG_OFF:
        {
            uint8_t ebudChrgOffReason = msg.data[0];
            
            App_Com_Tx_Cmd_Chrg_Off(DEVICE_RIGHT, ebudChrgOffReason);

            if(App_Ebud_Get_Chrg_State() == EBUD_CHRG_DONE)
            {
                Drv_Timer_Regist_Oneshot(App_Event_Send_Sleep, 250, NULL);
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
                    
                    App_Com_Tx_Cmd_Case_Open(DEVICE_LEFT);
                    
                    App_Com_Tx_Cmd_Case_Open(DEVICE_RIGHT);
                }
                else
                {
                    App_Com_Tx_Cmd_Case_Close(DEVICE_LEFT);
                    
                    App_Com_Tx_Cmd_Case_Close(DEVICE_RIGHT);
                    
                    App_Led_Hall_Close();
                }
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
            App_Com_Cmd_Case_Response(buf, length);
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

static void App_Event_Send_Sleep(void *arg )
{
    Drv_Msg_Put(APP_EVENT_SYS_SLEEP, NULL, 0);
}

