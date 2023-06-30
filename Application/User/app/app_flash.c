/********************************************************
* @file       app_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_flash.h"
/* Private typedef --------------------------------------*/
typedef void (*pFunction)(void );

/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static flash_ctrl_block_t flashCtrl;
static user_data_t userData;

void App_Flash_Erase_App2(void )
{
    uint32_t offsetAddr = 0;
    
    while(offsetAddr < flashCtrl.fwSize)
    {
        Drv_Flash_Erase_Page(APP2_START_ADDR + offsetAddr);

        offsetAddr += FLASH_PAGE_SIZE;
    }
}

void App_Flash_Write_App2(uint32_t offset, uint8_t *buf, uint8_t length )
{
    Drv_Flash_Write(APP2_START_ADDR + offset, buf, length );
}

void App_Flash_Dleay(void )
{
    uint32_t delayCnt = 100000;

    while(delayCnt--)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();
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


uint16_t App_Flash_Cal_Fw_Checksum(void )
{
    uint16_t wCRCin = 0x0000;
    uint16_t wCPoly = 0x8005;
    uint8_t i;

    uint8_t *pBuf = (uint8_t *)APP2_START_ADDR;
    uint32_t length = flashCtrl.fwSize; 
        
    InvertUint16(&wCPoly);

    while(length--)
    {
        wCRCin ^= *(pBuf++);
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

void App_Flash_Upg_Enable(void )
{
    userData.upgEn = 1;

    App_Flash_Save_User_Data();
}

void App_Flash_Save_User_Data(void )
{
    Drv_Flash_Erase_Page(USER_START_ADDR);

    Drv_Flash_Write(USER_START_ADDR, (uint8_t *)&userData, sizeof(userData));
}

void App_Flash_Set_Fw_Size(uint32_t fwSize )
{
    flashCtrl.fwSize = fwSize;

    userData.fwSize = fwSize;
}

#if 0
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
#endif

void App_Jump_to_Bld(void)
{

    #if 0
		static uint32_t JumpAddress;
		static pFunction Jump_To_Bld = NULL;
	
    RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
	
    if(((*(__IO uint32_t*)BLD_START_ADDR) & 0xFFFF0000 ) == 0x20000000)
    {
	    SYSCFG->RMAPCFG &= ~(3 << 0);
        Hal_Deinit();                                               		//close interrupt and clock and so on.
	    JumpAddress = *(__IO uint32_t*) (BLD_START_ADDR + 4);   			// Jump to user application 
	    Jump_To_Bld = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) BLD_START_ADDR);            			// Initialize user application's Stack Pointer 
		Jump_To_Bld();
    }
    #else
    
    NVIC_SystemReset();
    
    #endif 
}


