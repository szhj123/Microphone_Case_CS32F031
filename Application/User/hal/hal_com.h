#ifndef _HAL_COM_H
#define _HAL_COM_H

#include "hal_cs32f031.h"

typedef void (*hal_com_rx_callback_t)(uint8_t );

void Hal_Com_Init(void );
void Hal_Com_Regist_Rx_Isr_Callback(hal_com_rx_callback_t rx0Callback, hal_com_rx_callback_t rx1Callback, hal_com_rx_callback_t rx2Callback );

void Hal_Com_Tx1_Enable(void );
void Hal_Com_Tx1_Disable(void );
void Hal_Com_Tx1_Send(uint8_t *buf, uint16_t length, hal_isr_callback_t callback );
void Hal_Com_Tx1_Isr_Handler(void );

void Hal_Com_Tx2_Enable(void );
void Hal_Com_Tx2_Disable(void );
void Hal_Com_Tx2_Send(uint8_t *buf, uint16_t length, hal_isr_callback_t callback );
void Hal_Com_Tx2_Isr_Handler(void );


#endif 

