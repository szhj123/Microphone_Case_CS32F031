#ifndef _HAL_HALL_H
#define _HAL_HALL_H

#include "hal_cs32f031.h"

void Hal_Hall_Init(void );
uint8_t Hal_Hall_Get_State(void );
void Hal_Hall_Regist_Isr_Callback(hal_isr_callback_t callback );
void Hal_Hall_Isr_Handler(void );

#endif 

