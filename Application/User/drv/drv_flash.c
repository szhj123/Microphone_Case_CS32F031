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

flash_status_t Drv_Flash_Erase_Page(uint32_t pageAddr )
{
    flash_status_t retStatus;
    
    flash_unlock();

    do{

        __FLASH_FLAG_CLEAR(FLASH_FLAG_ENDF | FLASH_FLAG_PGERR | FLASH_FLAG_WPERR);
    
        retStatus = flash_page_erase(pageAddr);
    
    }while(retStatus != FLASH_STATUS_COMPLETE);
    
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

        __FLASH_FLAG_CLEAR(FLASH_FLAG_ENDF | FLASH_FLAG_PGERR | FLASH_FLAG_WPERR);
        
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

    __FLASH_FLAG_CLEAR(FLASH_FLAG_ENDF | FLASH_FLAG_PGERR | FLASH_FLAG_WPERR);

    retStatus = flash_word_program(flashAddr, u32Data.val);

    flash_lock();

    return retStatus;
}

