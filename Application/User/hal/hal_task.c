/********************************************************
* @file       hal_task.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_task_isr_callback = NULL;


void Hal_Task_Init(void )
{
    RCU->APB2EN |= RCU_APB2_PERI_SYSCFG;
    
    SystemCoreClockUpdate();
    
    SysTick_Config(SystemCoreClock / 1000);
}

void Hal_Task_Regist_Isr_Callback(hal_isr_callback_t callback )
{
    hal_task_isr_callback = callback;
}

void Hal_Task_Isr_Handler(void )
{
    if(hal_task_isr_callback != NULL)
    {
        hal_task_isr_callback();
    }
}

