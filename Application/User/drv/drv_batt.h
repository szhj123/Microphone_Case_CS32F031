#ifndef _DRV_BATT_H
#define _DRV_BATT_H

#include "hal_batt.h"

#define EBUD_CHRG_DISABLE                 0
#define EBUD_CHRG_ENABLE                  (!EBUD_CHRG_DISABLE)

void Drv_Batt_Init(void );
uint16_t Drv_Batt_Get_Vol(void );
uint16_t Drv_Ebud_Get_Tx1_Cur(void );
uint16_t Drv_Ebud_Get_Tx2_Cur(void );
uint16_t Drv_Ebud_Get_Rx_Cur(void );

void Drv_Ebud_Tx1_Chrg_En(uint8_t enable );
void Drv_Ebud_Tx2_Chrg_En(uint8_t enable );
void Drv_Ebud_Rx_Chrg_En(uint8_t enable );


#endif 

