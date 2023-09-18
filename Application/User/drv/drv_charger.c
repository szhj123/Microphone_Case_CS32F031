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
#define CHARGER_DEV_ADDR                 (0x1a << 1)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Chrg_Write(uint8_t regAddr, uint8_t val );
static uint8_t Drv_Chrg_Read(uint8_t regAddr );
static void Drv_Chrg_Intp_Handler(void );
/* Private variables ------------------------------------*/
chrg_ctrl_block_t  chrgCtrl;

void Drv_Chrg_Init(void )
{
    Hal_Batt_Chrg_Init();

    Hal_Chrg_Regist_Isr_Callback(Drv_Chrg_Intp_Handler);

    Drv_Chrg_Cfg();
}

void Drv_Chrg_Cfg(void )
{    
    Drv_Chrg_Write(0x00, 0x17);//input current limit: 2400mA
    Drv_Chrg_Write(0x01, 0x06);//sys_min: 3.2v
    Drv_Chrg_Write(0x02, 0xa8);//ichg: 0xa8,1020mA
    Drv_Chrg_Write(0x03, 0xfa);//iprechg:240mA, iterm: 120mA
    Drv_Chrg_Write(0x04, 0x58);//chg vol limit: 4.208v, bat rechg: 100mV
    Drv_Chrg_Write(0x05, 0x8f);
    Drv_Chrg_Write(0x06, 0xe5);//ovp thresh: 6.5v, vindpm:4.4v
    Drv_Chrg_Write(0x07, 0x4c);    
}

inline void Drv_Chrg_Chg_Boost_Disable(void )
{
    BOOST_DISABLE;
}

inline void Drv_Chrg_Boost_Enable(void )
{
    BOOST_ENABLE;
}

void Drv_Chrg_Enter_Ship_Mode(void )
{
    Drv_Chrg_Write(0x07, 0x64);    
}

void Drv_Chrg_Exit_Ship_Mode(void )
{
    Drv_Chrg_Write(0x07, 0x4c);    
}

void Drv_Chrg_Chg_Enable(void )
{
    Drv_Chrg_Write(0x01, 0x16);
}

void Drv_Chrg_Chg_Disable(void )
{
    Drv_Chrg_Write(0x01, 0x06);
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
    static uint8_t timerRdReg = TIMER_NULL;

    Drv_Timer_Delete(timerRdReg);
    
    timerRdReg = Drv_Timer_Regist_Oneshot(Drv_Chrg_Read_Status, 50, NULL);
}

uint8_t Drv_Chrg_Get_Usb_State(void )
{
    uint8_t regVal;

    #if 1
    if((vbus_stat_t)chrgCtrl.vbusStat != NO_INPUT)
    {
        regVal = USB_PLUG_IN;
    }
    else
    {
        regVal = USB_PLUG_OUT;
    }
    #else
    if(Hal_Chrg_Get_Usb_Stat())
    {
        regVal = USB_PLUG_IN;
    }
    else
    {
        regVal = USB_PLUG_OUT;
    }
    #endif 

    return regVal;
}

chrg_stat_t Drv_Chrg_Get_Charging_State(void )
{    
    return chrgCtrl.chrgStat;
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

