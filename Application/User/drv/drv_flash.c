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

void Drv_Flash_Init(void )
{
    
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
    uint32_t *pData = NULL;
    uint32_t lastWord;

    flash_status_t retStatus;
        
    u32DataLen = length / 4;

    if(((uint32_t )buf % 4) != 0)
    {
        return FLASH_STATUS_ERROR;
    }

    flash_unlock();

    pData = (uint32_t *)buf;
    
    for(i=0;i<u32DataLen;i++)
    {
        retStatus = flash_word_program(flashAddr, *pData);
        
        if(retStatus == FLASH_STATUS_COMPLETE)
        {
            flashAddr += 4;

            pData++;
        }
        else 
        {
            return retStatus;
        }
    }

    lastWord = 0xffffffff;

    for(i=0;i<(length % 4);i++)
    {
        *((uint8_t *)lastWord + i) = *((uint8_t *)pData + i);
    }

    retStatus = flash_word_program(flashAddr, lastWord);

    flash_lock();

    return retStatus;
}

