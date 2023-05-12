#ifndef _APP_BATT_H
#define _APP_BATT_H

#include "drv_task.h"
#include "drv_batt.h"

typedef struct _batt_ctrl_block_t
{
    void (*handler)(void );

    uint16_t delayCnt;
}batt_ctrl_block_t;

void App_Batt_Init(void );

#endif 

