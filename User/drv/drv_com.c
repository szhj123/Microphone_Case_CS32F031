/********************************************************
* @file       drv_com.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_com.h"

#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx1_Done_Callback(void );
/* Private variables ------------------------------------*/
static uint8_t tx1DoneFlag;

void Drv_Com_Init(void )
{
    Hal_Com_Init();
}

void Drv_Com_Tx1_Data(uint8_t *buf, uint16_t length )
{
    Hal_Com_Tx1_Enable();
    
    Hal_Com_Tx1_Data(buf, length, Drv_Com_Tx1_Done_Callback);    
}

static void Drv_Com_Tx1_Done_Callback(void )
{
    tx1DoneFlag = 1;
}

