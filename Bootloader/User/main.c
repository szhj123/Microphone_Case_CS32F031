/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs32f0xx.h"
#include "cs32f0xx_conf.h"
/* Private typedef --------------------------------------*/
typedef void (*pFunction)(void );
/* Private define ---------------------------------------*/
#define APP_FLASH_ADDR                 0x8001000   
#define APP_VECTOR_ADDR					 				APP_FLASH_ADDR
#define APP_VECTOR_SIZE                0xc0
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Hardware_Init(void );
static void Jump_To_App(void );
/* Private variables ------------------------------------*/
unsigned char vectors[APP_VECTOR_SIZE] __attribute__ ((section(".ARM. __at_0x20000000"))) = {0};

static uint32_t appAddress;
static pFunction appFunction = NULL;
static uint32_t delayCnt;

void Delay_Ms(uint32_t ms )
{
    delayCnt = ms;
	  
		while(delayCnt);
}


int main(void )
{
		static uint8_t dispCnt;
	
	  Hardware_Init();
	
    while(1)
		{
		    if(++dispCnt < 10 )
				{
				    __GPIO_PIN_SET(GPIOA, GPIO_PIN_11);
					
						Delay_Ms(200);
					
					  __GPIO_PIN_RESET(GPIOA, GPIO_PIN_11);
					
					  Delay_Ms(200);
				}
				else
				{
				    dispCnt = 0;
					
					  Jump_To_App();
				}
		}
}

static void Hardware_Init(void )
{
  	RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
	
		SystemCoreClockUpdate();
    
    SysTick_Config(SystemCoreClock / 1000);
	  
		__RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);

    gpio_mode_set(GPIOA, GPIO_PIN_11, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));
}

static void Hal_DeInit(void)
{
	/* Lock the access to flash & option bytes */
    FLASH->CTR |= FMC_CTR_LOCK;
    FLASH->CTR &= ~FMC_CTR_OBWEN;
	  __GPIO_DEF_INIT(GPIOA);	
	  RCU->AHBEN  = 0x00000014; 	// Set to reset value
	  RCU->APB2EN = 0x00000000;	// Set to reset value
}

static void Jump_To_App(void )
{
		__disable_irq();
	
	  memcpy((void*)vectors, (void*)APP_VECTOR_ADDR, APP_VECTOR_SIZE);	//copy interrupt vector table to sram. 
	
		if(((*(__IO uint32_t*)APP_FLASH_ADDR) & 0xFFFF0000 ) == 0x20000000)
    {
        SYSCFG->RMAPCFG |= SYSCFG_MEM_REMAP_SRAM;
			
				Hal_DeInit();

				appAddress = *(__IO uint32_t*) (APP_FLASH_ADDR + 4);   			// Jump to user application 
			
				appFunction = (pFunction) appAddress;
			
			  __set_MSP(*(__IO uint32_t*) APP_FLASH_ADDR);            			// Initialize user application's Stack Pointer 
				
			 __enable_irq();
			
	     appFunction();
    }
}

void SysTick_Handler(void)
{
    if(delayCnt > 0)
		{
		    delayCnt--;
		}
}

