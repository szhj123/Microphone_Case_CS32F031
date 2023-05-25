#ifndef _APP_LED_H
#define _APP_LED_H

#include "drv_timer.h"
#include "drv_led.h"

typedef void (*led_flash_callback_t)(void );

typedef struct _led_ctrl_block_t
{
    void (*handler)(void );
    
    uint16_t delayCnt;
    uint8_t  ledFlashIndex;
    uint8_t  ledFlashNum;
    
    led_t led1;
    led_t led2;
    led_t led3;
    led_t led4;
}led_ctrl_block_t;

void App_Led_Init(void );
void App_Led_All_Turn_Off(void );
void App_Led_All_Turn_On(void );
void App_Led_Flash_BattLevel(uint8_t battLevel );
void App_Led_Hall_Open(uint8_t battLevel );
void App_Led_Hall_Close(void );


#endif 

