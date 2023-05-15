/********************************************************
* @file       hal_interrupt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_task.h"
#include "hal_timer.h"
#include "hal_charger.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

/**
  * @fn void SysTick_Handler(void)
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @return None
  */
void SysTick_Handler(void)
{
    Hal_Task_Isr_Handler();
}

/**
  * @fn void TIM3_IRQHandler(void)
  * @brief  This function handles TIM3  interrupt request.
  * @param  None
  * @return None
  */
void TIM3_IRQHandler(void)
{
	if (__TIM_FLAG_STATUS_GET(TIM3, UPDATE) != RESET)
	{
		__TIM_FLAG_CLEAR(TIM3, TIM_FLAG_UPDATE);

        Hal_Timer_Isr_Handler();

	}
}

/**
  * @fn void EXTI0_1_IRQHandler(void)
  * @brief  This function handles External lines 0 to 1 interrupt request.
  * @param  None
  * @return None
  */
void EXTI0_1_IRQHandler(void)
{
    if(__EXTI_FLAG_STATUS_GET(EXTI_LINE_1) != RESET)
    {    
        /* Clear the EXTI line 1  bit */
        __EXTI_FLAG_CLEAR(EXTI_LINE_1);

        Hal_Charger_Isr_Handler();
    } 
}



