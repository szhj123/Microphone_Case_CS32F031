#ifndef _APP_EVENT_H
#define _APP_EVENT_H

#include "drv_task.h"
#include "drv_timer.h"
#include "drv_event.h"

typedef enum _app_event_t
{
    APP_EVENT_USB_PLUG_OUT = 0,
    APP_EVENT_USB_PLUG_IN,
    APP_EVENT_BATT_LEVEL,
    APP_EVENT_EBUD_TX1_CHRG_OFF,
    APP_EVENT_EBUD_TX2_CHRG_OFF,
    APP_EVENT_EBUD_RX_CHRG_OFF,
    APP_EVENT_HALL_STATE, 
    APP_EVENT_COM_CASE,
    APP_EVENT_COM_UPG,
    APP_EVENT_SYS_SLEEP
    
}app_event_t;

void App_Event_Init(void );

#endif 

