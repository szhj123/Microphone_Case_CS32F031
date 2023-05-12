/********************************************************
* @file       hal_led.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_led.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Led_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);

    //led green, pa11, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_11);

    gpio_mode_set(GPIOA, GPIO_PIN_11, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    //led orange, pa08, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_8);

    gpio_mode_set(GPIOA, GPIO_PIN_8, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    //led white, pa03, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_3);

    gpio_mode_set(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));
    
}

void Hal_Led_On(port_t port, pin_t pin )
{
    if(port == PORTA)
    {
        __GPIO_PIN_SET(GPIOA, 1 << pin);
    }
    else if(port == PORTB)
    {
        __GPIO_PIN_SET(GPIOB, 1 << pin);
    }
}

void Hal_Led_Off(port_t port, pin_t pin )
{
    if(port == PORTA)
    {
        __GPIO_PIN_RESET(GPIOA, 1 << pin);
    }
    else if(port == PORTB)
    {
        __GPIO_PIN_RESET(GPIOB, 1 << pin);
    }
}

