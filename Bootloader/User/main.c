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
#include "user_config.h"
/* Private typedef --------------------------------------*/

/* Private define ---------------------------------------*/

/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void User_Data_Get(void );
static void User_Data_Set(void );
static void App_Flash_Erase(void );
static flash_status_t App_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint32_t length );
static uint16_t App_Flash_Get_Checksum(uint8_t *buf, uint32_t length );

static void Jump_To_App(void );
/* Private variables ------------------------------------*/

static user_data_t userData;

static volatile uint32_t timing_delay;

extern unsigned char Vectors[];
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
unsigned int JumpAddress;

void timing_delay_decrement(void)
{
    if (timing_delay != 0x00)
    { 
        timing_delay--;
    }
}

void delay(volatile uint32_t value)
{ 
    timing_delay = value;

    while(timing_delay != 0);
}

void SysTick_Handler(void)
{
		timing_delay_decrement();
}

int main(void )
{
	  static uint8_t i;
	
    RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
	
	  __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
 
    // Configure PA5 pins as output  
	  //gpio_mode_set(GPIOA, GPIO_PIN_11, GPIO_MODE_OUT_PP(GPIO_SPEED_LOW));
	
    SysTick_Config(SystemCoreClock / 1000);
	
//    for(i=0;i<5;i++)
//		{
//		    __GPIO_PIN_SET(GPIOA, GPIO_PIN_11);
//			  delay(100);
//				__GPIO_PIN_RESET(GPIOA, GPIO_PIN_11);
//				delay(100);
//		}
			
		User_Data_Get();
				
		if(userData.upgEn == 0x01)
	  {
		    uint16_t app1Checksum = 0;
				uint16_t app2Checksum = 0;
						
			  App_Flash_Erase();     	
			
			  App_Flash_Write(APP1_START_ADDR, (uint8_t *)APP2_START_ADDR, userData.fwSize);
			
				app1Checksum = App_Flash_Get_Checksum((uint8_t *)APP1_START_ADDR, userData.fwSize);
			
			  app2Checksum = App_Flash_Get_Checksum((uint8_t *)APP2_START_ADDR, userData.fwSize);
						
				if(app1Checksum == app2Checksum)
				{
				    userData.upgEn = 0;
									
						User_Data_Set();
								
						__enable_irq();
								
						Jump_To_App();
				}
				else
				{
				    NVIC_SystemReset();
				}
    }		
		else
		{
		    Jump_To_App();   	
		}
		
		while(1);
}

flash_status_t Drv_Flash_Erase_Page(uint32_t pageAddr )
{
    flash_status_t retStatus;
    
    flash_unlock();
    
    retStatus = flash_page_erase(pageAddr);

    flash_lock();

    return retStatus;
}

static void User_Data_Get(void )
{
		uint8_t i;
	
		for(i=0;i<sizeof(user_data_t);i++)
	  {
		    *((uint8_t *)&userData + i) = *((uint8_t *)USER_START_ADDR + i);	
	  }
}

static void User_Data_Set(void )
{
		Drv_Flash_Erase_Page(USER_START_ADDR);
	
		App_Flash_Write(USER_START_ADDR, (uint8_t *)&userData, sizeof(user_data_t));
}

static void App_Flash_Erase(void )
{
   	uint32_t offsetAddr = 0;
    
    while(offsetAddr < userData.fwSize)
    {
        Drv_Flash_Erase_Page(APP1_START_ADDR + offsetAddr);

        offsetAddr += FLASH_PAGE_SIZE;
    }
}

void InvertUint16(uint16_t *poly )
{
    uint8_t i;
    uint16_t tmp = 0;
    uint16_t polyVal = *poly;

    for(i=0;i<16;i++)
    {
        if(polyVal & (1 << i))
            tmp |= 1 << (15-i);
    }
    *poly = tmp;
}

static uint16_t App_Flash_Get_Checksum(uint8_t *buf, uint32_t length )
{
		uint16_t wCRCin = 0x0000;
    uint16_t wCPoly = 0x8005;
    uint8_t i;

    InvertUint16(&wCPoly);

    while(length--)
    {
        wCRCin ^= *(buf++);
        for(i=0;i<8;i++)
        {

            if(wCRCin & 0x0001)
                wCRCin = (wCRCin >> 1) ^ wCPoly;
            else
                wCRCin >>= 1;
        }
    }
    return wCRCin;
}

static flash_status_t App_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint32_t length )
{
		uint16_t i;
    uint16_t u32DataLen;
    word_t u32Data;

    flash_status_t retStatus;
        
    u32DataLen = length / 4;

    flash_unlock();    
    
    for(i=0;i<u32DataLen;i++)
    {
        u32Data.byte_t.byte0 = buf[i*4 + 0];
        u32Data.byte_t.byte1 = buf[i*4 + 1];
        u32Data.byte_t.byte2 = buf[i*4 + 2];
        u32Data.byte_t.byte3 = buf[i*4 + 3];
        
        retStatus = flash_word_program(flashAddr, u32Data.val);
        
        if(retStatus == FLASH_STATUS_COMPLETE)
        {
            flashAddr += 4;
        }
        else 
        {
            return retStatus;
        }
    }

    u32Data.val = 0xffffffff;

    for(i=0;i<(length % 4);i++)
    {
        *((uint8_t *)&u32Data.val + i) = *((uint8_t *)&buf[u32DataLen*4] + i);
    }

    retStatus = flash_word_program(flashAddr, u32Data.val);

    flash_lock();

    return retStatus;
}

static void Hal_DeInit(void)
{
		/* Lock the access to flash & option bytes */
    FLASH->CTR |= FMC_CTR_LOCK;
    FLASH->CTR &= ~FMC_CTR_OBWEN;
	
	/* Wait for USART TX completed */
	//while(!(USART1->STS & USART_FLAG_TC));
	
	__GPIO_DEF_INIT(GPIOA);	
	//__USART_DEF_INIT(USART1);
	
	//NVIC_DisableIRQ(IRQn_USART1);

	RCU->AHBEN  = 0x00000014; 	// Set to reset value
	RCU->APB2EN = 0x00000000;	// Set to reset value
	
#if CSBOOT_CONFIG_DEBUG_LOG_ENABLE	
	__USART_DEF_INIT(USART2);	
	NVIC_DisableIRQ(IRQn_USART2);
#endif	
}

static void Jump_To_App(void )
{
	  memcpy((void*)Vectors, (void*)APP1_START_ADDR, APP1_VECTOR_SIZE);	//copy interrupt vector table to sram.
    
		if(((*(__IO uint32_t*)APP1_START_ADDR) & 0xFFFF0000 ) == 0x20000000)
		{
				SYSCFG->RMAPCFG |= SYSCFG_MEM_REMAP_SRAM;
				Hal_DeInit();                                               		//close interrupt and clock and so on.
				JumpAddress = *(__IO uint32_t*) (APP1_START_ADDR + 4);   			// Jump to user application 
				Jump_To_Application = (pFunction) JumpAddress;
				__set_MSP(*(__IO uint32_t*) APP1_START_ADDR);            			// Initialize user application's Stack Pointer 
				Jump_To_Application();
		}
}


