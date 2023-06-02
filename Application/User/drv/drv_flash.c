/********************************************************
* @file       drv_flash.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
uint8_t wBuf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint8_t rBuf[10] = {0};

void Drv_Flash_Init(void )
{
    Drv_Flash_Erase_Page(APP2_START_ADDR);

    Drv_Flash_Write(APP2_START_ADDR, wBuf, sizeof(wBuf));

    for(uint8_t i=0;i<10;i++)
    {
        rBuf[i] = *((uint8_t *)APP2_START_ADDR + i);
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

flash_status_t Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length )
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
