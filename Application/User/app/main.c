/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_task.h"
#include "drv_timer.h"

#include "app_led.h"
#include "app_event.h"
#include "app_batt.h"
#include "app_hall.h"
#include "app_com.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

int main(void )
{
    __enable_irq();
    
    Drv_Task_Init();

    Drv_Timer_Init();

    App_Com_Init();

    App_Hall_Init();

    App_Batt_Init();

    App_Led_Init();

    App_Event_Init();
    
    while(1)
	{
        Drv_Task_Run();
	}
}


