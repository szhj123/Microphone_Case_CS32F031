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
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define CHARGER_DEV_ADDR                 (0x6a << 1)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Chrg_Write(uint8_t regAddr, uint8_t val );
static uint8_t Drv_Chrg_Read(uint8_t regAddr );
static void Drv_Chrg_Intp_Handler(void );
/* Private variables ------------------------------------*/
chrg_ctrl_block_t  chrgCtrl;


void Drv_BATT_CHRG_INIT(void )
{
    Hal_BATT_CHRG_INIT();

    Hal_Chrg_Regist_Isr_Callback(Drv_Chrg_Intp_Handler);

    Drv_Chrg_Cfg();
}

void Drv_Chrg_Cfg(void )
{
    chrg_status0_t chrgStatus0;
    
    Drv_Chrg_Write(0x00, 0x17);

    
    chrgStatus0.status = Drv_Chrg_Read(CHRG_REG_STATUS0);

    chrgCtrl.chrgStat = (chrg_stat_t)chrgStatus0.bits.chrg_stat;
    chrgCtrl.vbusStat = (vbus_stat_t)chrgStatus0.bits.vbus_stat;
}

static void Drv_Chrg_Read_Status(void *arg )
{
    chrg_status0_t chrgStatus0;
    chrg_status1_t chrgStatus1;

    chrgStatus0.status = Drv_Chrg_Read(CHRG_REG_STATUS0);
    chrgStatus1.status = Drv_Chrg_Read(CHRG_REG_STATUS1);

    chrgCtrl.chrgStat = (chrg_stat_t )chrgStatus0.bits.chrg_stat;
    chrgCtrl.vbusStat = (vbus_stat_t )chrgStatus0.bits.vbus_stat;
    chrgCtrl.chrgFault = (chrg_fault_t )chrgStatus1.bits.chrg_fault;
    chrgCtrl.ntcFault = (ntc_fault_t )chrgStatus1.bits.ntc_fault;
}

static void Drv_Chrg_Intp_Handler(void )
{
    static uint8_t regRdTimer = TIMER_NULL;

    Drv_Timer_Delete(regRdTimer);
    
    regRdTimer = Drv_Timer_Regist_Oneshot(Drv_Chrg_Read_Status, 25, NULL);
}

uint8_t Drv_Chrg_Get_Usb_State(void )
{
    uint8_t regVal;
    
    if((chrg_stat_t)chrgCtrl.chrgStat != NOT_CHRG)
    {
        regVal = USB_PLUG_IN;
    }
    else
    {
        regVal = USB_PLUG_OUT;
    }

    return regVal;
}

static void Drv_Chrg_Write(uint8_t regAddr, uint8_t val )
{
    uint8_t buf[2] = {0};
    uint8_t i;

    buf[0] = regAddr;
    buf[1] = val;
    
    for(i=0;i<3;i++)
    {
        if(Hal_Chrg_Write(CHARGER_DEV_ADDR, buf, 2))
        {
            break;
        }
    }
}

static uint8_t Drv_Chrg_Read(uint8_t regAddr )
{
    uint8_t i;
    uint8_t val = 0;

    for(i=0;i<3;i++)
    {
        if(Hal_Chrg_Read(CHARGER_DEV_ADDR, regAddr, &val, 1))
        {
            break;
        }
    }

    return val;
}

