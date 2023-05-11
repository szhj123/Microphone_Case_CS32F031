/********************************************************
* @file       hal_timer.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_timer_isr_callback = NULL;

void Hal_Timer_Init(void )
{
    tim_base_t timer_config_struct;
    nvic_config_t nvic_config_struct;
    uint16_t prescaler_value = 0;

    __RCU_APB1_CLK_ENABLE(RCU_APB1_PERI_TIM3); // TIM3 clock enable

    prescaler_value = (uint16_t)(SystemCoreClock / 1000000) - 1; // Compute the prescaler value

    // Time base configuration
    timer_config_struct.period = 1000;
    timer_config_struct.predivider = 0;
    timer_config_struct.clk_division = 0;
    timer_config_struct.count_mode = TIM_COUNT_PATTERN_UP;
    tim_base_init(TIM3, &timer_config_struct);
    tim_prescaler_set(TIM3, prescaler_value, TIM_PDIV_MODE_IMMEDIATE); // Prescaler configuration

    // Enable the TIM3  Interrupt.
    nvic_config_struct.IRQn = IRQn_TIM3;
    nvic_config_struct.priority = 0;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);

    __TIM_INTR_ENABLE(TIM3, TIM_INTR_UPDATE); /* TIM Interrupts enable */
    __TIM_ENABLE(TIM3); 
}

void Hal_Timer_Regist_Isr_Callback(hal_isr_callback_t callback )
{
    hal_timer_isr_callback = callback;
}

void Hal_Timer_Isr_Handler(void )
{
    if(hal_timer_isr_callback != NULL)
    {
        hal_timer_isr_callback();
    }
}
