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
#define LED_FLASH_INTERVAL_TIMER                250
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

static uint8_t flag;
static uint8_t dispCnt;
static void ledTest(void *arg )
{

    flag ^= 1;

    if(flag)
    {
        Drv_Led_On(&ledCtrl.led1);
    }
    else
    {
        Drv_Led_Off(&ledCtrl.led1);
    }

    dispCnt++;
}

typedef void (*pFunction)(void );

pFunction bldFunction = NULL;

#define BLD_FLASH_ADDR              0x8000000    

uint32_t bldAddress;


void Jump_To_Bootloader(void )
{
    __disable_irq();
    
	#if 0
    RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;

	if(((*(__IO uint32_t*)BLD_FLASH_ADDR) & 0xFFFF0000 ) == 0x20000000)
    {
        SYSCFG->RMAPCFG = (SYSCFG->RMAPCFG & ~SYSCFG_MEM_REMAP_SRAM) | SYSCFG_MEM_REMAP_SYS_MEMORY;

        bldAddress = *(__IO uint32_t*) (BLD_FLASH_ADDR + 4);   			// Jump to user application 
	
		bldFunction = (pFunction) bldAddress;
	
	    __set_MSP(*(__IO uint32_t*) BLD_FLASH_ADDR);            			// Initialize user application's Stack Pointer 
	
	    __enable_irq();
			
	    bldFunction();
    }
    #else
    NVIC_SystemReset();
		
    #endif 
}

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

    Drv_Timer_Regist_Period(ledTest, 0, 250, NULL);
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

    if(dispCnt >= 10)
    {
        dispCnt = 0;

        Jump_To_Bootloader();
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
                Drv_Led_On(&ledCtrl.led3);
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

