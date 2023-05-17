/********************************************************
* @file       hal_batt.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_batt.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Hal_Batt_Adc_Init(void );
static uint16_t Hal_Batt_Adc_Filter(uint16_t *buf, uint8_t num);
/* Private variables ------------------------------------*/

void Hal_Batt_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);

    gpio_mode_set(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_3);

    Hal_Batt_Adc_Init();

    Hal_Batt_Adc_Get_SampleVal(ADC_CONV_CHANNEL_9);
}

static void Hal_Batt_Adc_Init(void )
{
    adc_config_t   adc_config_struct;
    
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_ADC);   
    
    //Configure ADC CH0 GPIO as analog input.
    gpio_mode_set(GPIOB, GPIO_PIN_1, GPIO_MODE_ANALOG);
    
    adc_def_init(ADC1);   
    adc_config_struct_init(&adc_config_struct);  

    // Configure the ADC1 in continuous mode 
    adc_config_struct.adc_resolution = ADC_CONV_RES_12BITS;
    adc_config_struct.continuous_mode = ENABLE; 
    adc_config_struct.trigger_mode = ADC_TRIG_MODE_NONE;
    adc_config_struct.data_align = ADC_DATA_ALIGN_RIGHT;
    adc_config_struct.scan_direction = ADC_SCAN_DIR_UPWARD;
    adc_init(ADC1, &adc_config_struct); 
}

uint16_t Hal_Batt_Adc_Get_SampleVal(uint32_t channel )
{
    uint16_t sampleBuf[16];
    uint8_t i;
    
    // Set the ADC1 CH0 with 239.5 Cycles
    adc_channel_config(ADC1, channel , ADC_SAMPLE_TIMES_239_5); 
    adc_calibration_value_get(ADC1);    // ADC Calibration.  
    __ADC_ENABLE(ADC1);                     // Enable the ADC.    
    
    while(!__ADC_FLAG_STATUS_GET(ADC1, EOI)); // Wait the EOI flag.    
    adc_conversion_start(ADC1);         //ADC1 regular Software Start Conv. 

    for(i=0;i<sizeof(sampleBuf)/sizeof(uint16_t );i++)
    {
        while(__ADC_FLAG_STATUS_GET(ADC1, EOC) == RESET); // Check EOC flag     
        sampleBuf[i] = __ADC_CONV_VALUE_GET(ADC1);  
    }

    return Hal_Batt_Adc_Filter(sampleBuf, sizeof(sampleBuf)/sizeof(uint16_t ));
}

void Hal_Batt_Adc_Det_Enable(void )
{
    __GPIO_PIN_RESET(GPIOB, GPIO_PIN_3);
}

void Hal_Batt_Adc_Det_Disable(void )
{
    __GPIO_PIN_SET(GPIOB, GPIO_PIN_3);
}

static uint16_t Hal_Batt_Adc_Filter(uint16_t *buf, uint8_t num)
{
    uint8_t i, j;
    uint16_t tmp;
    uint32_t sum;

    /* sort the value from small to large */
    for(i = 0; i < num; i++)
    {
        for(j = 0; j < ((num - 1) - i); j++)
        {
            if(buf[j] > buf[j + 1])
            {
                tmp = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = tmp;
            }
        }
    }

    /* Remove the smallest and largest values, then take the average */
    sum = 0;
    for(i = 2; i < (num - 2); i++)
    {
        sum += buf[i];
    }
    tmp = (uint16_t) (sum / (num - 4));

    return (tmp);
}


