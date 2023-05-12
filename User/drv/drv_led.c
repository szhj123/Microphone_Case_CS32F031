/********************************************************
* @file       drv_led.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_led.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Led_Init(void )
{
    Hal_Led_Init();
}

void Drv_Led_On(led_t *led )
{
    Hal_Led_Off(led->port, led->pin);
}

void Drv_Led_Off(led_t *led )
{
    Hal_Led_On(led->port, led->pin);
}

