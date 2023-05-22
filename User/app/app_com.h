#ifndef _APP_COM_H
#define _APP_COM_H

#include "drv_task.h"
#include "drv_com.h"

typedef enum _com_tx_state_t
{
    COM_STAT_ENTRY = 0,
    COM_STAT_PROCESS,
    COM_STAT_EXIT    
}com_tx_state_t;

void App_Com_Init(void );


#endif 

