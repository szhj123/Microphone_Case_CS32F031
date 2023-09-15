#ifndef _HAL_BATT_H
#define _HAL_BATT_H

#include "hal_cs32f031.h"

void Hal_Batt_Init(void );
uint16_t Hal_Batt_Adc_Get_SampleVal(uint32_t channel );
void Hal_Batt_Adc_Det_Enable(void );
void Hal_Batt_Adc_Det_Disable(void );

void Hal_Ebud_Tx1_Chrg_On(void );
void Hal_Ebud_Tx1_Chrg_Off(void );
void Hal_Ebud_Tx2_Chrg_On(void );
void Hal_Ebud_Tx2_Chrg_Off(void );
void Hal_Ebud_Rx_Chrg_On(void );
void Hal_Ebud_Rx_Chrg_Off(void );


#endif 

