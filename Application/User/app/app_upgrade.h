#ifndef _APP_UPGRADE_H
#define _APP_UPGRADE_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_flash.h"

#define FW_DATA_PACK_MAX_SIZE               64
#define UPG_TIMER_OUT                       1000

#define SIRK_DATA_PACK_MAX_SIZE             8


typedef enum _upg_stat_t
{
    UPG_STAT_GET_VER = 0,
    UPG_STAT_COMPARE_VER,
    UPG_STAT_START,
    UPG_STAT_GET_FW_INFO,
    UPG_STAT_GET_FW_DATA,    
    UPG_STAT_GET_FW_CRC,
    UPG_STAT_EXIT,
}upg_stat_t;

typedef enum _sirk_stat_t
{
    SIRK_STAT_GET = 0,
    SIRK_STAT_COMPARE,
    SIRK_STAT_SET,
    SIRK_STAT_EXIT
}sirk_stat_t;

typedef struct _upg_pata_t
{
    upg_stat_t stat;
    uint8_t    responseFlag;

    uint8_t    verBld;
    uint8_t    verApp;
    uint8_t    verHw;
    uint8_t    verFlag;

    uint16_t   delayCnt;

    uint32_t   fwSize;
    uint32_t   fwOffset;
    uint8_t    fwDataBuf[FW_DATA_PACK_MAX_SIZE];
    uint8_t    fwDataLen;
    uint16_t   fwCrc;
}upg_para_t;

typedef struct _sirk_para_t
{
    sirk_stat_t stat;
    uint8_t     sirkBufLeft[SIRK_DATA_PACK_MAX_SIZE];
    uint8_t     sirkBufRight[SIRK_DATA_PACK_MAX_SIZE];
    uint8_t     sirkBufMiddle[SIRK_DATA_PACK_MAX_SIZE];
    uint8_t     sirkBufRandom[SIRK_DATA_PACK_MAX_SIZE];

    uint8_t     sirkLeftCrc;
    uint8_t     sirkRightCrc;
    uint8_t     sirkMiddleCrc;

    uint8_t     sirkResponseLeft;
    uint8_t     sirkResponseRight;
    uint8_t     sirkResponseMiddle;
}sirk_para_t;

void App_Upg_Init(void );
void App_Upg_Set_Fw_Ver(uint8_t verBld, uint8_t verApp, uint8_t verHw );
void App_Upg_Set_Fw_Size(uint32_t fwSize );
void App_Upg_Set_Fw_Data(uint8_t *buf, uint8_t length );
void App_Upg_Set_Fw_CRC(uint16_t fwCrc );
uint16_t App_Upg_Get_Fw_Data_Offset(void );

#endif 

