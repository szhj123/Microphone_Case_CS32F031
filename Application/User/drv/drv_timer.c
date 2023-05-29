/********************************************************
* @file       drv_timer.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Timer_Isr_Handler(void );
/* Private variables ------------------------------------*/
timer_ctrl_block_t timerList[TIMER_MAX_NUM];

void Drv_Timer_Init(void )
{
    Hal_Timer_Init();

    Hal_Timer_Regist_Isr_Callback(Drv_Timer_Isr_Handler);
}

static uint8_t Drv_Timer_Regist(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    uint8_t timerId;

    for(timerId=0;timerId<TIMER_MAX_NUM;timerId++)
    {
        if(timerList[timerId].isRegFlag == 0)
        {
            timerList[timerId].handler = handler;
            timerList[timerId].arg = arg;
            timerList[timerId].ticks = ticks;
            timerList[timerId].period = period;
            timerList[timerId].isRegFlag = 1;

            break;
        }
    }

    return timerId;
}

uint8_t Drv_Timer_Regist_Oneshot(void (*handler)(void *), uint16_t ticks, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, 0, arg);
}

uint8_t Drv_Timer_Regist_Period(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg )
{
    return Drv_Timer_Regist(handler, ticks, period, arg );
}

void Drv_Timer_Delete(uint8_t timerId )
{
    if(timerId >= TIMER_MAX_NUM)
    {
        return ;
    }

    timerList[timerId].handler = NULL;
    timerList[timerId].arg = NULL;
    timerList[timerId].ticks = 0;
    timerList[timerId].period = 0;
    timerList[timerId].isRegFlag = 0;
}

static void Drv_Timer_Isr_Handler(void )
{
    uint8_t i;

    for(i=0;i<TIMER_MAX_NUM;i++)
    {
        if(timerList[i].isRegFlag)
        {
            if(timerList[i].ticks)
            {
                timerList[i].ticks--;
            }

            if(timerList[i].ticks == 0)
            {
                if(timerList[i].handler != NULL)
                {
                    timerList[i].handler(timerList[i].arg);
                }

                if(timerList[i].period)
                {
                    timerList[i].ticks = timerList[i].period;
                }
                else
                {
                    Drv_Timer_Delete(i);
                }
            }
        }
    }
}

