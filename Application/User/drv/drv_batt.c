/********************************************************
* @file       drv_batt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_batt.h"
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define ADC_REF_VOL                 2815//mv
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Batt_Init(void )
{
    Hal_Batt_Init();
}

uint16_t Drv_Batt_Get_Vol(void )
{
    uint32_t adcVal = 0;
    uint16_t battVol ;
    
    adcVal = Hal_Batt_Adc_Get_SampleVal(ADC_CONV_CHANNEL_9);

    battVol =  ((adcVal * ADC_REF_VOL * 2) >> 12);

    return battVol;
}

uint16_t Drv_Ebud_Get_Tx1_Cur(void )
{
    uint32_t adcVal = 0;
    uint16_t ebudCur = 0;
        
    adcVal = Hal_Batt_Adc_Get_SampleVal(ADC_CONV_CHANNEL_6);
    
    ebudCur =  ((adcVal * ADC_REF_VOL * 10) >> 12);

    return ebudCur;
}

uint16_t Drv_Ebud_Get_Tx2_Cur(void )
{
    uint32_t adcVal = 0;
    uint16_t ebudCur = 0;
        
    adcVal = Hal_Batt_Adc_Get_SampleVal(ADC_CONV_CHANNEL_8);
    
    ebudCur =  ((adcVal * ADC_REF_VOL * 10) >> 12);

    return ebudCur;
}

uint16_t Drv_Ebud_Get_Rx_Cur(void )
{
    uint32_t adcVal = 0;
    uint16_t ebudCur = 0;
        
    adcVal = Hal_Batt_Adc_Get_SampleVal(ADC_CONV_CHANNEL_7);
    
    ebudCur =  ((adcVal * ADC_REF_VOL * 10) >> 12);

    return ebudCur;
}



