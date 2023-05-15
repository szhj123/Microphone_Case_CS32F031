/********************************************************
* @file       drv_batt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_batt.h"
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
static void test(void *arg )
{
    
}

void Drv_Batt_Init(void )
{
    Hal_Batt_Init();

    Drv_Timer_Regist_Period(test, 0, 250, NULL);
}

