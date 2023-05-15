/********************************************************
* @file       hal_batt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_batt.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Batt_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);

    gpio_mode_set(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_3);

    
}

void Hal_Batt_Detect_Enable(void )
{
    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_3);
}

void Hal_Batt_Detect_Disable(void )
{
    __GPIO_PIN_SET(GPIOB, GPIO_PIN_3);
}

