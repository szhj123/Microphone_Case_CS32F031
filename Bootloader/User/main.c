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

typedef union _word_t
{
    uint32_t val;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte_t;
}word_t;

typedef struct _user_data_t
{
    uint8_t upgEn;
    uint32_t fwSize; 
}user_data_t;
/* Private define ---------------------------------------*/
#define APP_FLASH_ADDR                 0x8001000   
#define APP_VECTOR_ADDR					 				APP_FLASH_ADDR
#define APP_VECTOR_SIZE                0xc0

#define BLD_START_ADDR                 0x08000000
#define BLD_MAX_SIZE                   0x00001000
#define APP1_START_ADDR                (BLD_START_ADDR + BLD_MAX_SIZE)
#define APP1_MAX_SIZE                  0x00007800
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE

#define FLASH_PAGE_SIZE                1024

#define USER_START_ADDR                0x1FFFF840
#define USER_MAX_SIZE                  192
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Get_User_Data(void );
static void App_Flash_Erase(void );
static void Jump_To_App(void );
/* Private variables ------------------------------------*/
unsigned char vectors[APP_VECTOR_SIZE] __attribute__ ((section(".ARM. __at_0x20000000"))) = {0};

static user_data_t userData;
static uint32_t appAddress;
static pFunction appFunction = NULL;
static uint32_t delayCnt;

int main(void )
{
	  __disable_irq();
	
		Get_User_Data();
	
		if(userData.upgEn)
		{
		    App_Flash_Erase();     	
			
				App_Flash_Write();
		}
	
    while(1)
		{
		   
		}
}

static void Get_User_Data(void )
{
		uint8_t i;
	
		uint8_t *pData = (uint8_t *)USER_START_ADDR;
	
		for(i=0;i<sizeof(user_data_t);i++)
	  {
		    *((uint8_t *)&userData + i) = *((uint8_t *)USER_START_ADDR + i);	
	  }
}

flash_status_t Drv_Flash_Erase_Page(uint32_t pageAddr )
{
    flash_status_t retStatus;
    
    flash_unlock();
    
    retStatus = flash_page_erase(pageAddr);

    flash_lock();

    return retStatus;
}

static void App_Flash_Erase(void )
{
   	uint32_t offsetAddr = 0;
    
    while(offsetAddr < userData.fwSize)
    {
        Drv_Flash_Erase_Page(APP_FLASH_ADDR + offsetAddr);

        offsetAddr += FLASH_PAGE_SIZE;
    }
}

flash_status_t App_Flash_Write(void )
{
		uint16_t i;
    uint16_t u32DataLen;
    uint32_t *pData;

    flash_status_t retStatus;
        
    u32DataLen = userData.fwSize / 4;
	
		pData = (uint32_t *)

    flash_unlock();    
    
    for(i=0;i<u32DataLen;i++)
    {
        u32Data.byte_t.byte0 = buf[i*4 + 0];
        u32Data.byte_t.byte1 = buf[i*4 + 1];
        u32Data.byte_t.byte2 = buf[i*4 + 2];
        u32Data.byte_t.byte3 = buf[i*4 + 3];
        
        retStatus = flash_word_program(APP_FLASH_ADDR, u32Data.val);
        
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
	  __GPIO_DEF_INIT(GPIOA);	
	  RCU->AHBEN  = 0x00000014; 	// Set to reset value
	  RCU->APB2EN = 0x00000000;	// Set to reset value
}

static void Jump_To_App(void )
{
		__disable_irq();
	
		RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
	
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


