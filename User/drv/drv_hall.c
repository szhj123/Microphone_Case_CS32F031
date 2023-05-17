/********************************************************
* @file       drv_hall.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_hall.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Hall_Isr_Handler(void );
/* Private variables ------------------------------------*/
hall_callback_t hall_state_callback;


void Drv_Hall_Init(hall_callback_t callback )
{
    Hal_Hall_Init();

    Hal_Hall_Regist_Isr_Callback(Drv_Hall_Isr_Handler);

    hall_state_callback = callback;
}

static void Drv_Hall_Isr_Handler(void )
{
    static uint8_t timerHall = TIMER_NULL;

    Drv_Timer_Delete(timerHall);

    timerHall = Drv_Timer_Regist_Oneshot(hall_state_callback, 200, NULL);
}

uint8_t Drv_Hall_Get_State(void )
{
    uint8_t retVal;
    
    if(Hal_Hall_Get_State())
    {
        retVal = HALL_OPEN;
    }
    else
    {
        retVal = HALL_CLOSE;
    }

    return retVal;
}


