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
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Led_Handler(void *arg );
void App_Led_Flash_Callback(void );
/* Private variables ------------------------------------*/
led_ctrl_block_t ledCtrl;

void App_Led_Init(void )
{
    Drv_Led_Init();

    ledCtrl.led1.port = PORTA;
    ledCtrl.led1.pin = PIN11;

    ledCtrl.led2.port = PORTA;
    ledCtrl.led2.pin = PIN8;

    ledCtrl.led3.port = PORTA;
    ledCtrl.led3.pin = PIN3;

    ledCtrl.handler = App_Led_Flash_Callback;
    ledCtrl.delayCnt = 0;

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

void App_Led_Flash_Callback(void )
{
   if(ledCtrl.delayCnt > 1000)
   {
       ledCtrl.delayCnt = 0;
       
       ledCtrl.flashFlag ^= 1;
       
       if(ledCtrl.flashFlag)
       {
           Drv_Led_On(&ledCtrl.led1);
       }
       else
       {
           Drv_Led_Off(&ledCtrl.led1);
       }
   }
}

