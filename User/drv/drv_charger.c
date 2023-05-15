/********************************************************
* @file       drv_charger.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_charger.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define CHARGER_DEV_ADDR                 (0x6a << 1)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Charger_Write(uint8_t regAddr, uint8_t val );
static uint8_t Drv_Charger_Read(uint8_t regAddr );

/* Private variables ------------------------------------*/

void Drv_Charger_Init(void )
{
    Hal_Charger_Init();

    Drv_Charger_Config();
}

void Drv_Charger_Config(void )
{
    static uint8_t val;
    
    Drv_Charger_Write(0x00, 0x17);

    
    
    val = Drv_Charger_Read(0x00);
}

static void Drv_Charger_Write(uint8_t regAddr, uint8_t val )
{
    uint8_t buf[2] = {0};
    uint8_t i;

    buf[0] = regAddr;
    buf[1] = val;
    
    for(i=0;i<3;i++)
    {
        if(Hal_Charger_Write(CHARGER_DEV_ADDR, buf, 2))
        {
            break;
        }
    }
}

static uint8_t Drv_Charger_Read(uint8_t regAddr )
{
    uint8_t i;
    uint8_t val = 0;

    for(i=0;i<3;i++)
    {
        if(Hal_Charger_Read(CHARGER_DEV_ADDR, 0, &val, 1))
        {
            break;
        }
    }

    return val;
}

void Drv_Charger_Get_State(void )
{
    static uint8_t state;
    
    Hal_Charger_Read(CHARGER_DEV_ADDR, 0x08, &state, 1);
}

