#ifndef _APP_FLASH_H
#define _APP_FLASH_H

#include "drv_flash.h"

#define BLD_START_ADDR                 0x08000000
#define BLD_MAX_SIZE                   0x00001000
#define APP1_START_ADDR                (BLD_START_ADDR + BLD_MAX_SIZE)
#define APP1_MAX_SIZE                  0x00007800
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE

#define USER_START_ADDR                0x1FFFF840
#define USER_MAX_SIZE                  192

#define FLASH_PAGE_SIZE                1024

typedef struct _flash_ctrl_block_t
{
    uint32_t fwSize;
    uint32_t fwOffset;
}flash_ctrl_block_t;

typedef struct _user_data_t
{
    uint8_t upgEn;
}user_data_t;

void App_Flash_Erase_App2(void );
void App_Flash_Write_App2(uint32_t offset, uint8_t *buf, uint8_t length );
void App_Flash_Set_Fw_Size(uint32_t fwSize );
uint16_t App_Flash_Cal_Fw_Checksum(void );
void App_Flash_Upg_Enable(void );
void App_Flash_Save_User_Data(void );
void App_Jump_to_Bld(void);

#endif 

