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
unsigned char vectors[APP_VECTOR_SIZE] __attribute__ ((section(".ARM.__at_0x20000000"))) = {0};

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
		    if(++dispCnt < 5 )
				{
				    __GPIO_PIN_SET(GPIOA, GPIO_PIN_3);
					
						Delay_Ms(250);
					
					  __GPIO_PIN_RESET(GPIOA, GPIO_PIN_3);
					
					  Delay_Ms(250);
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
		__GPIO_PIN_SET(GPIOA, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));
}

static void Jump_To_App(void )
{
		__disable_irq();
	
	  memcpy((void*)vectors, (void*)APP_VECTOR_ADDR, APP_VECTOR_SIZE);	//copy interrupt vector table to sram. 
	
		if(((*(__IO uint32_t*)APP_FLASH_ADDR) & 0xFFFF0000 ) == 0x20000000)
    {
        SYSCFG->RMAPCFG |= SYSCFG_MEM_REMAP_SRAM;

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
