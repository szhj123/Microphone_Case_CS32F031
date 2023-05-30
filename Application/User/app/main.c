/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_task.h"
#include "drv_timer.h"

#include "app_led.h"
#include "app_event.h"
#include "app_batt.h"
#include "app_hall.h"
#include "app_com.h"
/* Private typedef --------------------------------------*/
typedef void (*pFunction)(void );
/* Private define ------------------ --------------------*/
#define BLD_ADDR                0x8000000
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void jump_to_bld(void);

/* Private variables ------------------------------------*/
static uint32_t JumpAddress;
static pFunction Jump_To_Bld = NULL;
static uint32_t delayMs;

void timing_delay_decrement(void)
{
    if (delayMs != 0x00)
    { 
        delayMs--;
    }
}

void delay(volatile uint32_t value)
{ 
    delayMs = value;

    while(delayMs != 0);
}

static void ledTest(void *arg )
{
    static uint8_t cnt ;
    static uint8_t flag;

    flag ^= 1;

    if(++cnt < 20)
    {
        if(flag)
        {
            __GPIO_PIN_SET(GPIOA, GPIO_PIN_3);
        }
        else
        {
            __GPIO_PIN_RESET(GPIOA, GPIO_PIN_3);
        }
    }
    else
    {
        cnt = 0;

        jump_to_bld();
    }
    
}

int main(void )
{   
    Drv_Task_Init();

    Drv_Timer_Init();

    App_Com_Init();

    App_Hall_Init();

    App_Batt_Init();

    App_Led_Init();

    App_Event_Init();

    Drv_Task_Regist_Period(ledTest, 0, 200, NULL);
    
    while(1)
	{
        Drv_Task_Run();
	}
}

static void Hal_Deinit(void)
{
	/* Lock the access to flash & option bytes */
    FLASH->CTR |= FMC_CTR_LOCK;
    FLASH->CTR &= ~FMC_CTR_OBWEN;
	
	
	__GPIO_DEF_INIT(GPIOA);	
    __GPIO_DEF_INIT(GPIOB);	

	RCU->AHBEN  = 0x00000014; 	// Set to reset value
	RCU->APB1EN = 0x00000000;	// Set to reset value	
	RCU->APB2EN = 0x00000000;	// Set to reset value
}

static void jump_to_bld(void)
{
    RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
	
    if(((*(__IO uint32_t*)BLD_ADDR) & 0xFFFF0000 ) == 0x20000000)
    {
	    SYSCFG->RMAPCFG &= ~(3 << 0);
        Hal_Deinit();                                               		//close interrupt and clock and so on.
	    JumpAddress = *(__IO uint32_t*) (BLD_ADDR + 4);   			// Jump to user application 
	    Jump_To_Bld = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) BLD_ADDR);            			// Initialize user application's Stack Pointer 
		Jump_To_Bld();
    }
}


