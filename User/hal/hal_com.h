#ifndef _HAL_COM_H
#define _HAL_COM_H

#include "hal_cs32f031.h"

void Hal_Com_Init(void );
void Hal_Com_Tx1_Enable(void );
void Hal_Com_Tx1_Disable(void );
void Hal_Com_Tx1_Data(uint8_t *buf, uint16_t length, hal_isr_callback_t callback );
void Hal_Com_Tx1_Isr_Handler(void );

#endif 

