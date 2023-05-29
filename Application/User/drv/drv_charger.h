#ifndef _DRV_CHARGER_H
#define _DRV_CHARGER_H

#include "hal_charger.h"

#define USB_PLUG_OUT                     0
#define USB_PLUG_IN                      (!USB_PLUG_OUT)

#define CHRG_REG_STATUS0                 0x08
#define CHRG_REG_STATUS1                 0x09
#define CHRG_REG_STATUS2                 0x0a


typedef union _chrg_status0_t
{
    uint8_t status;
    struct 
    {
        uint8_t vsys_stat:1;
        uint8_t therm_stat:1;
        uint8_t pg_stat:1;
        uint8_t chrg_stat:2;
        uint8_t vbus_stat:3;
    }bits;
}chrg_status0_t;

typedef union _chrg_status1_t
{
    uint8_t status;
    struct 
    {
        uint8_t ntc_fault:3;
        uint8_t bat_fault:1;
        uint8_t chrg_fault:2;
        uint8_t boost_fault:1;
        uint8_t wdt_fault:1;
    }bits;
}chrg_status1_t;

typedef enum _vbus_stat_t
{
    NO_INPUT = 0,
    USB_HOST_SDP_500mA = 1,
    ADAPTER_2_4A = 3,
    BOOST_MODE = 7
}vbus_stat_t;

typedef enum _chrg_stat_t
{
    NOT_CHRG = 0,
    PRE_CHRG,
    FAST_CHRG,
    CHRG_TERMINATION
}chrg_stat_t;

typedef enum _chrg_fault_t
{
    CHRG_NORMAL = 0,
    INPUT_FAULT,
    THERMAL_SHUTDOWN,
    CHRG_SAFETY_TIMER_EXPIRATION
}chrg_fault_t;

typedef enum _ntc_fault_t
{
    NTC_NORMAL = 0,
    WARM = 2,
    COOL = 3,
    COLD = 5,
    HOT  = 6
}ntc_fault_t;

typedef struct chrg_ctrl_block_t
{
    vbus_stat_t vbusStat;
    chrg_stat_t chrgStat;
    chrg_fault_t chrgFault;
    ntc_fault_t  ntcFault;
    
}chrg_ctrl_block_t;

void Drv_Chrg_Init(void );
void Drv_Chrg_Cfg(void );
void Drv_Chrg_Chg_Boost_Disable(void );
void Drv_Chrg_Chg_Enable(void );
void Drv_Chrg_Boost_Enable(void );
uint8_t Drv_Chrg_Get_Usb_State(void );
chrg_stat_t Drv_Chrg_Get_Charging_State(void );

#endif

