/********************************************************
* @file       app_hall.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "app_hall.h"
#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Hall_Send_State(void *arg );
/* Private variables ------------------------------------*/

void App_Hall_Init(void )
{
    Drv_Hall_Init(App_Hall_Send_State);    
}

static void App_Hall_Send_State(void *arg )
{
    uint8_t hallState;

    hallState = Drv_Hall_Get_State();

    Drv_Msg_Put(APP_EVENT_HALL_STATE, (const uint8_t *)&hallState, 1);
}

uint8_t App_Hall_Get_State(void )
{
    return Drv_Hall_Get_State();
}

