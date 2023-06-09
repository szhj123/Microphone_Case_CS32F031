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
static void App_Event_Upg_Handler(uint8_t *buf, uint8_t length );

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
                    
                    App_Com_Case_Tx_Open(DEVICE_LEFT);
                    
                    App_Com_Case_Tx_Open(DEVICE_RIGHT);
                }
                else
                {
                    App_Com_Case_Tx_Close(DEVICE_LEFT);
                    
                    App_Com_Case_Tx_Close(DEVICE_RIGHT);
                    
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
        case APP_EVENT_COM_UPG:
        {
            App_Event_Upg_Handler(msg.data, msg.lenght);
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
    rx_response_t rxResponse;
    uint8_t i;

    for(i=0;i<length-8;i++)
    {
        *((uint8_t *)&rxResponse + i) = buf[7+i];
    }
    
    App_Com_Set_Rx_Response(rxResponse);
}

void App_Event_Upg_Handler(uint8_t *buf, uint8_t length )
{
    static word_t saveFwOffset;
    uint8_t cmd = buf[0];

    switch(cmd)
    {
        case CMD_FW_ERASE:
        {
            word_t fwSize;

            fwSize.byte_t.byte0 = buf[1];
            fwSize.byte_t.byte1 = buf[2];
            fwSize.byte_t.byte2 = buf[3];
            fwSize.byte_t.byte3 = buf[4];

            App_Flash_Set_Fw_Size(fwSize.val);
                
            App_Flash_Erase_App2();
            
            App_Com_Upg_Tx_Ack();

            saveFwOffset.val = 0xffff;
            break;
        }
        case CMD_FW_DATA:
        {
            word_t fwOffset;

            fwOffset.byte_t.byte0 = buf[1];
            fwOffset.byte_t.byte1 = buf[2];
            fwOffset.byte_t.byte2 = buf[3];
            fwOffset.byte_t.byte3 = buf[4];

            if(saveFwOffset.val != fwOffset.val)
            {
                saveFwOffset.val = fwOffset.val;
                
                App_Flash_Write_App2(fwOffset.val, &buf[5], length-5);
            }

            App_Com_Upg_Tx_Ack();
            break;
        }
        case CMD_FW_CHECKSUM:
        {
            uint16_t fwChecksum = (uint16_t )buf[2] << 8 | buf[1];

            uint16_t calFwChecksum = App_Flash_Cal_Fw_Checksum();

            if(fwChecksum == calFwChecksum)
            {
                App_Com_Upg_Tx_Ack();
            }
            break;
        }
        case CMD_FW_VERSION:
        {
            App_Com_Upg_Tx_FwVer();
            
            break;
        }
        case CMD_FW_RESET:
        {
            App_Flash_Upg_Enable();

            App_Jump_to_Bld();
            
            break;
        }
        default: break;
    }
}


