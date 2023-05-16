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
#define BATT_VOL_5                  3450
#define BATT_VOL_3                  3000

typedef enum _batt_level_t
{
    BATT_LEVEL_0 = 0,
    BATT_LEVEL_1_20,
    BATT_LEVEL_21_50,
    BATT_LEVEL_51_80,
    BATT_LEVEL_81_100
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
    BATT_CHRG_PROCESS
}batt_chrg_stat_t;

typedef struct _batt_ctrl_block_t
{
    batt_dischrg_stat_t dischrgStat;
    batt_chrg_stat_t chrgStat;
    batt_level_t battLevel;
	batt_level_t battSaveLevel;
    uint16_t delayCnt;
    uint16_t battVol;
    uint16_t battSaveVol;
    uint16_t battErrVol;
}batt_ctrl_block_t;

void App_Batt_Init(void );
batt_level_t App_Batt_Cal_Level(uint16_t battVol );
void App_Batt_Send_Level(void );

#endif 

