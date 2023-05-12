#ifndef _HAL_LED_H
#define _HAL_LED_H

#include "hal_cs32f031.h"

typedef enum _port_t
{
    PORTA = 0,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF
}port_t;

typedef enum _pin_t
{
    PIN0 = 0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7,
    PIN8,
    PIN9,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15
}pin_t;


void Hal_Led_Init(void );
void Hal_Led_On(port_t port, pin_t pin );
void Hal_Led_Off(port_t port, pin_t pin );

#endif 

