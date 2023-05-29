#ifndef _DRV_LED_H
#define _DRV_LED_H

#include "hal_led.h"

typedef struct _led_t
{
    port_t port;
    pin_t  pin;
}led_t;

void Drv_Led_Init(void );
void Drv_Led_On(led_t *led );
void Drv_Led_Off(led_t *led );
uint8_t Drv_Led_Get_Stat(led_t *led );

#endif 

