#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hal_cs32f031.h"

flash_status_t Drv_Flash_Erase_Page(uint32_t pageAddr );
flash_status_t Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length );

#endif 

