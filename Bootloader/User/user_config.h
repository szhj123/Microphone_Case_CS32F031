/**
  * @file    user_config.h
  * @brief   This file provides functions to manage the following 
  *          functionalities of the Reset and clock unit (RCU) peripheral:
  *           + Internal/external clocks, PLL, CSS and CLKOUT configuration
  *
  * @author  yudz
  * @version V0.1.0
  * @date    2021.03.05         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2021 ChipSea</center></h2>
  */

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdbool.h"
#include "cs32f0xx.h"
#include "cs32f0xx_misc.h"
#include "cs32f0xx_rcu.h"
#include "cs32f0xx_syscfg.h"
#include "cs32f0xx_gpio.h"
#include "cs32f0xx_fwdt.h"
#include "cs32f0xx_flash.h"
#include "cs32f0xx_usart.h"
#include "cs32f0xx_crc.h"
#include "cs32f0xx_exti.h"
	 
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

#define BLD_START_ADDR                 0x08000000
#define BLD_MAX_SIZE                   0x00001000
#define APP1_START_ADDR                (BLD_START_ADDR + BLD_MAX_SIZE)
#define APP1_MAX_SIZE                  0x00007400
#define APP2_START_ADDR                (APP1_START_ADDR + APP1_MAX_SIZE)
#define APP2_MAX_SIZE                  APP1_MAX_SIZE
#define USER_START_ADDR                (APP2_START_ADDR + APP2_MAX_SIZE)
#define USER_MAX_SIZE                  1024

#define APP1_VECTOR_SIZE							 0xC0

#define FLASH_PAGE_SIZE                1024





#ifdef __cplusplus
}
#endif

#endif 
