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

    //led4, pa03, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    //led3, pa12, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_12);
    gpio_mode_set(GPIOA, GPIO_PIN_12, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    //led2, pa11, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_11);
    gpio_mode_set(GPIOA, GPIO_PIN_11, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    //led1, pa08, output
    __GPIO_PIN_SET(GPIOA, GPIO_PIN_8);
    gpio_mode_set(GPIOA, GPIO_PIN_8, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));    
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
    else if(port == PORTF)
    {
        __GPIO_PIN_SET(GPIOF, 1 << pin);
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
    else if(port == PORTF)
    {
        __GPIO_PIN_RESET(GPIOF, 1 << pin);
    }
}

uint8_t Hal_Led_Get_Stat(port_t port, pin_t pin )
{
    uint8_t retVal;
    
    if(port == PORTA)
    {
        retVal = __GPIO_INPUT_PIN_GET(GPIOA, 1 << pin);
    }
    else if(port == PORTB)
    {
        retVal = __GPIO_INPUT_PIN_GET(GPIOB, 1 << pin);
    }
    else if(port == PORTF)
    {
        retVal = __GPIO_INPUT_PIN_GET(GPIOF, 1 << pin);
    } 

    return retVal;
}

