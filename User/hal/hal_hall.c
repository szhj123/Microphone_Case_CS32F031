/********************************************************
* @file       hal_hall.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_hall.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_hall_isr_callback = NULL;

void Hal_Hall_Init(void )
{
    nvic_config_t nvic_config_struct;

    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);    // Enable GPIOC clock
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_SYSCFG); // Enable SYSCFG clock

    // Configure PB5 pins as input
    gpio_mode_set(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_PU);

    syscfg_exti_line_config(SYSCFG_EXTI_PORT_PB, SYSCFG_EXTI_PIN_5); // Connect EXTI1 Line to PB5 pin

    // Configure EXTI5 line
    __EXTI_INTR_ENABLE(EXTI_LINE_5);
    __EXTI_EDGE_ENABLE(EXTI_EDGE_RISING, EXTI_LINE_5);
    __EXTI_EDGE_ENABLE(EXTI_EDGE_FALLING, EXTI_LINE_5);

    // Enable and set EXTI4_15 Interrupt
    nvic_config_struct.IRQn = IRQn_EXTI4_15;
    nvic_config_struct.priority = 0x00;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);
}

uint8_t Hal_Hall_Get_State(void )
{
    return __GPIO_INPUT_PIN_GET(GPIOB, GPIO_PIN_5);
}

void Hal_Hall_Regist_Isr_Callback(hal_isr_callback_t callback )
{
    hal_hall_isr_callback = callback;
}

void Hal_Hall_Isr_Handler(void )
{
    if(hal_hall_isr_callback != NULL)
    {
        hal_hall_isr_callback();
    }
}

