/********************************************************
* @file       app_led.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_led.h"
#include "app_batt.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define LED_FLASH_INTERVAL_TIMER                350
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Led_Handler(void *arg );
static void App_Led_Flash_BattLevel_1_20(void );
static void App_Led_Flash_BattLevel_21_50(void );
static void App_Led_Flash_BattLevel_51_80(void );
static void App_Led_Flash_BattLevel_81_99(void );

static void App_Led_Hall_Open_Callback(void *arg );

/* Private variables ------------------------------------*/
led_ctrl_block_t ledCtrl;
uint8_t timer_led_5s = TIMER_NULL;

void App_Led_Init(void )
{
    Drv_Led_Init();

    ledCtrl.led1.port = PORTA;
    ledCtrl.led1.pin = PIN8;

    ledCtrl.led2.port = PORTA;
    ledCtrl.led2.pin = PIN11;

    ledCtrl.led3.port = PORTA;
    ledCtrl.led3.pin = PIN12;

    ledCtrl.led4.port = PORTA;
    ledCtrl.led4.pin = PIN3;

    Drv_Timer_Regist_Period(App_Led_Handler, 0, 1, NULL);
}

static void App_Led_Handler(void *arg )
{
    if(ledCtrl.delayCnt < 0xffff)
    {
        ledCtrl.delayCnt++;
    }

    if(ledCtrl.handler != NULL)
    {
        ledCtrl.handler();
    }    
}

void App_Led_All_Turn_Off(void )
{
    ledCtrl.handler = NULL;
    
    Drv_Led_Off(&ledCtrl.led1);
    Drv_Led_Off(&ledCtrl.led2);
    Drv_Led_Off(&ledCtrl.led3);
    Drv_Led_Off(&ledCtrl.led4);
}

void App_Led_All_Turn_On(void )
{
    ledCtrl.handler = NULL;
    
    Drv_Led_On(&ledCtrl.led1);
    Drv_Led_On(&ledCtrl.led2);
    Drv_Led_On(&ledCtrl.led3);
    Drv_Led_On(&ledCtrl.led4);
}

void App_Led_Flash_BattLevel(uint8_t battLevel )
{
    static led_flash_callback_t  led_flash_callback = NULL;
    
    if((batt_level_t )battLevel >= BATT_LEVEL_81_90)
    {
        ledCtrl.handler = App_Led_Flash_BattLevel_81_99; 
    }
    else if((batt_level_t )battLevel >= BATT_LEVEL_51_60 && (batt_level_t )battLevel < BATT_LEVEL_81_90)
    {
        ledCtrl.handler = App_Led_Flash_BattLevel_51_80; 
    }
    else if((batt_level_t )battLevel >= BATT_LEVEL_21_30 && (batt_level_t )battLevel < BATT_LEVEL_51_60)
    {
        ledCtrl.handler = App_Led_Flash_BattLevel_21_50;
    }
    else
    {
        ledCtrl.handler = App_Led_Flash_BattLevel_1_20; 
    }

    if(ledCtrl.handler != led_flash_callback)
    {
        ledCtrl.delayCnt = 0;
        ledCtrl.ledFlashIndex = 0;

        Drv_Led_Off(&ledCtrl.led1);
        Drv_Led_Off(&ledCtrl.led2);
        Drv_Led_Off(&ledCtrl.led3);
        Drv_Led_Off(&ledCtrl.led4);

        led_flash_callback = ledCtrl.handler;
    }
}

static void App_Led_Flash_BattLevel_1_20(void )
{
   if(ledCtrl.delayCnt >= LED_FLASH_INTERVAL_TIMER)
   {
       ledCtrl.delayCnt = 0;
              
       if(Drv_Led_Get_Stat(&ledCtrl.led1))
       {
           Drv_Led_On(&ledCtrl.led1);
       }
       else
       {
           Drv_Led_Off(&ledCtrl.led1);
       }
   }
}

static void App_Led_Flash_BattLevel_21_50(void )
{
    if(ledCtrl.delayCnt >= LED_FLASH_INTERVAL_TIMER)
    {
        ledCtrl.delayCnt = 0;
        
        switch(ledCtrl.ledFlashIndex)
        {
            case 0: 
            {
                Drv_Led_On(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 1:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_On(&ledCtrl.led2);
                ledCtrl.ledFlashIndex = 0;
                break;
            }
            default: break;
        }
    }
}

static void App_Led_Flash_BattLevel_51_80(void )
{
    if(ledCtrl.delayCnt >= LED_FLASH_INTERVAL_TIMER)
    {
        ledCtrl.delayCnt = 0;
        
        switch(ledCtrl.ledFlashIndex)
        {
            case 0: 
            {
                Drv_Led_On(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                Drv_Led_Off(&ledCtrl.led3);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 1:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_On(&ledCtrl.led2);
                Drv_Led_Off(&ledCtrl.led3);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 2:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                Drv_Led_On(&ledCtrl.led3);
                ledCtrl.ledFlashIndex = 0;
                break;
            }
            default: break;
        }
    }
}

static void App_Led_Flash_BattLevel_81_99(void )
{
    if(ledCtrl.delayCnt >= LED_FLASH_INTERVAL_TIMER)
    {
        ledCtrl.delayCnt = 0;
        
        switch(ledCtrl.ledFlashIndex)
        {
            case 0: 
            {
                Drv_Led_On(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                Drv_Led_Off(&ledCtrl.led3);
                Drv_Led_Off(&ledCtrl.led4);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 1:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_On(&ledCtrl.led2);
                Drv_Led_Off(&ledCtrl.led3);
                Drv_Led_Off(&ledCtrl.led4);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 2:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                Drv_Led_On(&ledCtrl.led3);
                Drv_Led_Off(&ledCtrl.led4);
                ledCtrl.ledFlashIndex++;
                break;
            }
            case 3:
            {
                Drv_Led_Off(&ledCtrl.led1);
                Drv_Led_Off(&ledCtrl.led2);
                Drv_Led_Off(&ledCtrl.led3);
                Drv_Led_On(&ledCtrl.led4);
                ledCtrl.ledFlashIndex = 0;
                break;
            }
            default: break;
        }
    }
}

void App_Led_Hall_Open(uint8_t battLevel )
{
    ledCtrl.handler = NULL;

    if((batt_level_t )battLevel >= BATT_LEVEL_81_90)
    {
        Drv_Led_On(&ledCtrl.led1);
        Drv_Led_On(&ledCtrl.led2);
        Drv_Led_On(&ledCtrl.led3);
        Drv_Led_On(&ledCtrl.led4);
    }
    else if((batt_level_t )battLevel >= BATT_LEVEL_51_60 && (batt_level_t )battLevel < BATT_LEVEL_81_90)
    {
        Drv_Led_On(&ledCtrl.led1);
        Drv_Led_On(&ledCtrl.led2);
        Drv_Led_On(&ledCtrl.led3);
        Drv_Led_Off(&ledCtrl.led4);
    }
    else if((batt_level_t )battLevel >= BATT_LEVEL_21_30 && (batt_level_t )battLevel < BATT_LEVEL_51_60)
    {
        Drv_Led_On(&ledCtrl.led1);
        Drv_Led_On(&ledCtrl.led2);
        Drv_Led_Off(&ledCtrl.led3);
        Drv_Led_Off(&ledCtrl.led4);
    }
    else
    {
        Drv_Led_On(&ledCtrl.led1);
        Drv_Led_Off(&ledCtrl.led2);
        Drv_Led_Off(&ledCtrl.led3);
        Drv_Led_Off(&ledCtrl.led4);
    }

    Drv_Timer_Delete(timer_led_5s);

    timer_led_5s = Drv_Timer_Regist_Oneshot(App_Led_Hall_Open_Callback, 5000, NULL);
}

static void App_Led_Hall_Open_Callback(void *arg )
{
    timer_led_5s = TIMER_NULL;
    
    App_Led_All_Turn_Off();
}

void App_Led_Hall_Close(void )
{
    if(timer_led_5s != TIMER_NULL)
    {
        Drv_Timer_Delete(timer_led_5s);

        timer_led_5s = TIMER_NULL;
    }
    
    App_Led_All_Turn_Off();
}

