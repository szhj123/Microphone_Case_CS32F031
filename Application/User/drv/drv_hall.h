#ifndef _DRV_HALL_H
#define _DRV_HALL_H

#include "hal_hall.h"
#include "drv_timer.h"
#include "drv_event.h"

typedef void (*hall_callback_t)(void *);

#define HALL_CLOSE                0
#define HALL_OPEN                 (!HALL_CLOSE)

void Drv_Hall_Init(hall_callback_t callabck );
uint8_t Drv_Hall_Get_State(void );

#endif 

