#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "hal_cs32f031.h"

#define BLD_START_ADDR                 0x08000000
#define BLD_MAX_SIZE                   0x00001000
#define APP1_START_ADDR                (BLD_START_ADDR + BLD_MAX_SIZE)
#define APP1_MAX_SIZE                  0x00007800
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE

#define USER_START_ADDR                0x1FFFF840
#define USER_MAX_SIZE                  192

void Drv_Flash_Init(void );
flash_status_t Drv_Flash_Erase_Page(uint32_t pageAddr );
flash_status_t Drv_Flash_Write(uint32_t flashAddr, uint8_t *buf, uint16_t length );

#endif 

