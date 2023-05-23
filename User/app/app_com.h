#ifndef _APP_COM_H
#define _APP_COM_H

#include "drv_task.h"
#include "drv_com.h"
#include "drv_event.h"

typedef enum _com_tx_state_t
{
    COM_STAT_INIT = 0,
    COM_STAT_TX,
    COM_STAT_TX_WATI_DONE,
    COM_STAT_RX_RESPONSE,
    COM_STAT_EXIT    
}com_tx_state_t;

void App_Com_Init(void );


#endif 

