#ifndef _DRV_BATT_H
#define _DRV_BATT_H

#include "hal_batt.h"

void Drv_Batt_Init(void );
uint16_t Drv_Batt_Get_Vol(void );
uint16_t Drv_Ebud_Get_Tx1_Cur(void );
uint16_t Drv_Ebud_Get_Tx2_Cur(void );
uint16_t Drv_Ebud_Get_Rx_Cur(void );

#endif 

