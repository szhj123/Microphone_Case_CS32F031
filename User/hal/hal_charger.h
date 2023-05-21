#ifndef _HAL_CHARGER_H
#define _HAL_CHARGER_H

#include "hal_cs32f031.h"

void Hal_Batt_Chrg_Init(void );
void Hal_Chrg_Regist_Isr_Callback(hal_isr_callback_t callback );
uint32_t Hal_Chrg_Write(uint32_t devAddr, uint8_t *buf, uint8_t length );
uint32_t Hal_Chrg_Read(uint32_t devAddr, uint8_t regAddr, uint8_t *buf, uint16_t length );
void Hal_Chrg_Isr_Handler(void );

#endif 

