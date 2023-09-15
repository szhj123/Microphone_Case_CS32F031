#ifndef _APP_BATT_H
#define _APP_BATT_H

#include "drv_task.h"
#include "drv_event.h"
#include "drv_charger.h"
#include "drv_batt.h"

#define BATT_VOL_100                4200
#define BATT_VOL_90                 4060
#define BATT_VOL_80                 3980
#define BATT_VOL_70                 3920
#define BATT_VOL_60                 3870
#define BATT_VOL_50                 3820
#define BATT_VOL_40                 3790
#define BATT_VOL_30                 3770
#define BATT_VOL_20                 3730
#define BATT_VOL_15                 3700
#define BATT_VOL_10                 3680
#define BATT_VOL_5                  3400
#define BATT_VOL_3                  3200

typedef enum _batt_level_t
{
    BATT_LEVEL_0 = 0,
    BATT_LEVEL_1_10 = 5,
    BATT_LEVEL_11_20 = 10,
    BATT_LEVEL_21_30 = 20,
    BATT_LEVEL_31_40 = 30,
    BATT_LEVEL_41_50 = 40,
    BATT_LEVEL_51_60 = 50,
    BATT_LEVEL_61_70 = 60,
    BATT_LEVEL_71_80 = 70,
    BATT_LEVEL_81_90 = 80,
    BATT_LEVEL_91_99 = 90,
    BATT_LEVEL_100 = 100
}batt_level_t;

typedef enum _batt_batt_dischrg_stat_t
{
    BATT_DISCHRG_INIT = 0,
    BATT_DISCHRG_PROCESS
}batt_dischrg_stat_t;

typedef enum _batt_chrg_stat_t
{
    BATT_CHRG_INIT = 0,
    BATT_CHRG_GET_VOL_ERR,
    BATT_CHRG_PROCESS,
    BATT_CHRG_DONE
}batt_chrg_stat_t;

typedef enum _ebud_charging_stat_t
{
    EBUD_CHRG_NONE = 0,
    EBUD_CHRG_PROCESS,
    EBUD_CHRG_DONE, 
    EBUD_CHRG_OVER_CUR
}ebud_charging_stat_t;

typedef enum _ebud_chrg_off_reason_t
{
    REASON_NONE = 0,
    REASON_CHRG_FULL,
    REASON_BATT_LOW
}ebud_chrg_off_reason_t;

typedef struct _batt_ctrl_block_t
{
    batt_dischrg_stat_t dischrgStat;
    batt_chrg_stat_t chrgStat;
    batt_level_t battLevel;
	batt_level_t battSaveLevel;

    ebud_charging_stat_t ebudTx1Stat;
    ebud_charging_stat_t ebudTx2Stat;
    ebud_charging_stat_t ebudRxStat;

    ebud_chrg_off_reason_t ebudTx1ChrgOffReason;
    ebud_chrg_off_reason_t ebudTx2ChrgOffReason;
    ebud_chrg_off_reason_t ebudRxChrgOffReason;
    
    uint16_t delayCnt;
    uint16_t battVol;
    uint16_t battSaveVol;
    uint16_t battErrVol;

    uint16_t ebudTx1Cur;
    uint16_t ebudTx2Cur;
    uint16_t ebudRxCur;

    uint8_t  ebudTx1Level;
    uint8_t  ebudTx2Level;
    uint8_t  ebudRxLevel;
}batt_ctrl_block_t;

void App_Batt_Init(void );
uint16_t App_Batt_Get_Vol(void );
batt_level_t App_Batt_Cal_Level(uint16_t battVol );
batt_level_t App_Batt_Get_Level(void );
ebud_charging_stat_t App_Ebud_Get_All_Chrg_Stat(void );
void App_Ebud_Set_Level(uint8_t devType, uint8_t level );

void App_Ebud_Set_Tx1_Chrg_Stat(void );
void App_Ebud_Set_Tx2_Chrg_Stat(void );
void App_Ebud_Set_Rx_Chrg_Stat(void );
ebud_charging_stat_t  App_Ebud_Get_Tx1_Chrg_Stat(void );
ebud_charging_stat_t  App_Ebud_Get_Tx2_Chrg_Stat(void );
ebud_charging_stat_t  App_Ebud_Get_Rx_Chrg_Stat(void );


#endif 

