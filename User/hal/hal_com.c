/********************************************************
* @file       hal_com.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_com.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define UART_PP_R_PORT                 GPIOA
#define UART_PP_R_PIN                  GPIO_PIN_15
#define UART_SW_R_PORT                 GPIOA
#define UART_SW_R_PIN                  GPIO_PIN_0
#define CHRG_SW_R_PORT                 GPIOB
#define CHRG_SW_R_PIN                  GPIO_PIN_8
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Hal_Com_Gpio_Init(void );
static void Hal_Com_Uart1_Init(void );
/* Private variables ------------------------------------*/
static hal_isr_callback_t hal_com_uart1_isr_callback = NULL;
static uint8_t *pUart1Buf = NULL;
static uint16_t uart1Length;

void Hal_Com_Init(void )
{
    Hal_Com_Gpio_Init();

    Hal_Com_Uart1_Init();
}

static void Hal_Com_Gpio_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);
    //pb8, charge_sw_r, output
    //gpio_mode_set(GPIOB, GPIO_PIN_8, GPIO_MODE_OUT_PP(SPEED));

    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    //pa0, uart_sw_r, output
    gpio_mode_set(UART_SW_R_PORT, UART_SW_R_PIN, GPIO_MODE_OUT_PP(SPEED));
    //pa15,uart_pp_r, pullup
    gpio_mode_set(UART_PP_R_PORT, UART_PP_R_PIN, GPIO_MODE_IN_PU);
}

static void Hal_Com_Uart1_Init(void )
{
    nvic_config_t nvic_config_struct;
    usart_config_t usart_config_struct;

    // Clock Config
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_USART1);

    // GPIO MF Config
    gpio_mf_config(GPIOA, GPIO_PIN_2, GPIO_MF_SEL1);

    gpio_mode_set(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_OD(SPEED));

    // USART Config
    __USART_DEF_INIT(USART1);
    usart_config_struct.baud_rate = 115200;
    usart_config_struct.data_width = USART_DATA_WIDTH_8;
    usart_config_struct.stop_bits = USART_STOP_BIT_1;
    usart_config_struct.parity = USART_PARITY_NO;
    usart_config_struct.flow_control = USART_FLOW_CONTROL_NONE;
    usart_config_struct.usart_mode = USART_MODE_RX | USART_MODE_TX;
    usart_init(USART1, &usart_config_struct);

    USART1->CTR3 |= USART_CTR3_HDEN;

    /* Enable the USART Interrupt */
    nvic_config_struct.IRQn = IRQn_USART1;
    nvic_config_struct.priority = 0;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);

    __USART_ENABLE(USART1); // Enable USART

    __USART_INTR_ENABLE(USART1, RXNE); // Enable the USART Receive interrupt
}


void Hal_Com_R_Tx_Enable(void )
{
    __GPIO_PIN_SET(UART_SW_R_PORT, UART_SW_R_PIN);

    gpio_mode_set(CHRG_SW_R_PORT, CHRG_SW_R_PIN, GPIO_MODE_IN_FLOAT);
}

void Hal_Com_R_Tx_Disable(void )
{
    __GPIO_PIN_RESET(UART_SW_R_PORT, UART_SW_R_PIN);

    gpio_mode_set(CHRG_SW_R_PORT, CHRG_SW_R_PIN, GPIO_MODE_OUT_PP(SPEED));

    __GPIO_PIN_RESET(CHRG_SW_R_PORT, CHRG_SW_R_PIN);
}

void Hal_Com_R_Tx_Data(uint8_t *buf, uint16_t length, hal_isr_callback_t callback )
{
    pUart1Buf = buf;
    uart1Length = length;

    
}

void Hal_Com_Uart1_isr_Handler(void )
{
    
}

