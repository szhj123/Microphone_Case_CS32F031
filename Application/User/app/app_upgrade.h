#ifndef _APP_UPGRADE_H
#define _APP_UPGRADE_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_flash.h"

typedef enum _upg_stat_t
{
    UPG_STAT_GET_VER = 0,
    UPG_STAT_COMPARE_VER,
    UPG_STAT_START,
    UPG_STAT_GET_FW_INFO,
    UPG_STAT_GET_DATA,    
}upg_stat_t;

typedef struct _upg_pata_t
{
    upg_stat_t stat;

    uint8_t    bldVer;
    uint8_t    appVer;
    uint8_t    hwVer;

    uint16_t   delayCnt;
}upg_para_t;

void App_Upg_Init(void );
void App_Upg_Set_Ver(uint8_t bldVer, uint8_t appVer, uint8_t hwVer );

#endif 

