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
#define TX1_PP_PORT                 GPIOB
#define TX1_PP_PIN                  GPIO_PIN_7
#define TX1_SW_PORT                 
#define TX1_SW_PIN                  
#define TX1_CHRG_SW_PORT            GPIOB
#define TX1_CHRG_SW_PIN             GPIO_PIN_4
#define TX1_PORT                    GPIOB
#define TX1_PIN                     GPIO_PIN_6

#define TX2_PP_PORT                 GPIOA
#define TX2_PP_PIN                  GPIO_PIN_15
#define TX2_SW_PORT                 GPIOA
#define TX2_SW_PIN                  GPIO_PIN_0
#define TX2_CHRG_SW_PORT            GPIOB
#define TX2_CHRG_SW_PIN             GPIO_PIN_8
#define TX2_PORT                    GPIOA
#define TX2_PIN                     GPIO_PIN_2

#define TX6_PORT                    GPIOA
#define TX6_PIN                     GPIO_PIN_4


/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Hal_Com_Gpio_Init(void );
static void Hal_Com_Uart1_Init(void );
static void Hal_Com_Uart2_Init(void );
static void Hal_Com_Uart6_Init(void );
/* Private variables ------------------------------------*/
static hal_com_rx_callback_t hal_rx0_isr_callback = NULL;
static hal_com_rx_callback_t hal_rx1_isr_callback = NULL;
static hal_com_rx_callback_t hal_rx2_isr_callback = NULL;
static hal_com_rx_callback_t hal_rx6_isr_callback = NULL;

static hal_isr_callback_t hal_tx1_isr_callback = NULL;
static uint8_t *pTx1Buf = NULL;
static uint16_t tx1Length;

static hal_isr_callback_t hal_tx2_isr_callback = NULL;
static uint8_t *pTx2Buf = NULL;
static uint16_t tx2Length;

static hal_isr_callback_t hal_tx6_isr_callback = NULL;
static uint8_t *pTx6Buf = NULL;
static uint16_t tx6Length;


void Hal_Com_Init(void )
{
    Hal_Com_Gpio_Init();

    Hal_Com_Uart1_Init();

    Hal_Com_Uart2_Init();

    Hal_Com_Uart6_Init();
}

void Hal_Com_Regist_Rx_Isr_Callback(hal_com_rx_callback_t rx0Callback, hal_com_rx_callback_t rx1Callback, hal_com_rx_callback_t rx2Callback , hal_com_rx_callback_t rx6Callback )
{
    hal_rx0_isr_callback = rx0Callback;
    
    hal_rx1_isr_callback = rx1Callback;
    
    hal_rx2_isr_callback = rx2Callback;

    hal_rx6_isr_callback = rx6Callback;
}


static void Hal_Com_Gpio_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);
    //pb8, charge_sw_r, output
    //gpio_mode_set(GPIOB, GPIO_PIN_8, GPIO_MODE_OUT_PP(SPEED));

    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    //pa0, uart_sw_r, output
    //gpio_mode_set(TX1_SW_PORT, TX1_SW_PIN, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));
    //pa15,uart_pp_r, pullup
    gpio_mode_set(TX1_PP_PORT, TX1_PP_PIN, GPIO_MODE_IN_PU);
    
    gpio_mode_set(TX2_PP_PORT, TX2_PP_PIN, GPIO_MODE_IN_PU);

    gpio_mode_set(TX2_SW_PORT, TX2_SW_PIN, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));
}

static void Hal_Com_Uart1_Init(void )
{
    nvic_config_t nvic_config_struct;
    usart_config_t usart_config_struct;

    // Clock Config
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOB);
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_USART1);

    // GPIO MF Config
    gpio_mf_config(TX1_PORT, TX1_PIN, GPIO_MF_SEL0);
    
    gpio_mode_set(TX1_PORT, TX1_PIN, GPIO_MODE_MF_OD_PU(GPIO_SPEED_HIGH));
    

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

}

static void Hal_Com_Uart2_Init(void )
{
    nvic_config_t nvic_config_struct;
    usart_config_t usart_config_struct;

    // Clock Config
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    __RCU_APB1_CLK_ENABLE(RCU_APB1_PERI_USART2);

    // GPIO MF Config
    gpio_mf_config(TX2_PORT, TX2_PIN, GPIO_MF_SEL1);
    
    gpio_mode_set(TX2_PORT, TX2_PIN, GPIO_MODE_MF_OD_PU(GPIO_SPEED_HIGH));
    

    // USART Config
    __USART_DEF_INIT(USART2);
    usart_config_struct.baud_rate = 115200;
    usart_config_struct.data_width = USART_DATA_WIDTH_8;
    usart_config_struct.stop_bits = USART_STOP_BIT_1;
    usart_config_struct.parity = USART_PARITY_NO;
    usart_config_struct.flow_control = USART_FLOW_CONTROL_NONE;
    usart_config_struct.usart_mode = USART_MODE_RX | USART_MODE_TX;
    usart_init(USART2, &usart_config_struct);

    USART2->CTR3 |= USART_CTR3_HDEN;

    /* Enable the USART Interrupt */
    nvic_config_struct.IRQn = IRQn_USART2;
    nvic_config_struct.priority = 0;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);

    __USART_ENABLE(USART2); // Enable USART
}

void Hal_Com_Uart6_Init(void )
{
    nvic_config_t nvic_config_struct;
    usart_config_t usart_config_struct;
    // Clock Config
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_USART6);

    __RCU_PERI_RST_ENABLE(USART6);
    // GPIO MF Config
    gpio_mf_config(TX6_PORT, TX6_PIN, GPIO_MF_SEL5);
    
    gpio_mode_set(TX6_PORT, TX6_PIN, GPIO_MODE_MF_OD_PU(GPIO_SPEED_HIGH));
    

    // USART Config
    __USART_DEF_INIT(USART6);
    usart_config_struct.baud_rate = 115200;
    usart_config_struct.data_width = USART_DATA_WIDTH_8;
    usart_config_struct.stop_bits = USART_STOP_BIT_1;
    usart_config_struct.parity = USART_PARITY_NO;
    usart_config_struct.flow_control = USART_FLOW_CONTROL_NONE;
    usart_config_struct.usart_mode = USART_MODE_RX | USART_MODE_TX;
    usart_init(USART6, &usart_config_struct);

    USART6->CTR3 |= USART_CTR3_HDEN;

    /* Enable the USART Interrupt */
    nvic_config_struct.IRQn = IRQn_USART6_8;
    nvic_config_struct.priority = 0;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);

    __USART_ENABLE(USART6); // Enable USART

    __USART_INTR_ENABLE(USART6, RXNE); // Enable the USART Receive interrupt
}


void Hal_Com_Tx1_Enable(void )
{
    //__GPIO_PIN_SET(TX1_SW_PORT, TX1_SW_PIN);

    gpio_mode_set(TX1_CHRG_SW_PORT, TX1_CHRG_SW_PIN, GPIO_MODE_IN_FLOAT);
}

void Hal_Com_Tx1_Disable(void )
{
    //__GPIO_PIN_RESET(TX1_SW_PORT, TX1_SW_PIN);

    gpio_mode_set(TX1_CHRG_SW_PORT, TX1_CHRG_SW_PIN, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    __GPIO_PIN_RESET(TX1_CHRG_SW_PORT, TX1_CHRG_SW_PIN);

    __USART_INTR_DISABLE(USART1, RXNE); // Enable the USART Receive interrupt
}

void Hal_Com_Tx1_Send(uint8_t *buf, uint16_t length, hal_isr_callback_t callback )
{
    pTx1Buf = buf;
    
    tx1Length = length;

    hal_tx1_isr_callback = callback;

    __USART_INTR_ENABLE(USART1, TXE); // Enable the USART transmit  interrupt
    
    __USART_INTR_DISABLE(USART1, RXNE); // Enable the USART Receive interrupt
}

void Hal_Com_Tx1_Isr_Handler(void )
{
    if (__USART_FLAG_STATUS_GET(USART1, TXE) == SET)
    {   
        if(tx1Length > 0)
        {
            __USART_DATA_SEND(USART1, *pTx1Buf);

            pTx1Buf++;
            
            tx1Length--;
        }
        else 
        {
            pTx1Buf = NULL;

            if(hal_tx1_isr_callback != NULL)
            {
                hal_tx1_isr_callback();

                hal_tx1_isr_callback = NULL;
            }
            /* Disable the USART transmit data register empty interrupt */
            __USART_INTR_DISABLE(USART1, TXE);
            
            __USART_INTR_ENABLE(USART1, RXNE); // Enable the USART Receive interrupt

        }
    }

    if (__USART_FLAG_STATUS_GET(USART1, RXNE) == SET)
    {
        uint8_t recvVal  = 0;
        
        recvVal = (uint8_t)__USART_DATA_RECV(USART1);

        if(hal_rx1_isr_callback != NULL)
        {
            hal_rx1_isr_callback(recvVal);
        }
    }
}

void Hal_Com_Tx2_Enable(void )
{
    __GPIO_PIN_SET(TX2_SW_PORT, TX2_SW_PIN);

    gpio_mode_set(TX2_CHRG_SW_PORT, TX2_CHRG_SW_PIN, GPIO_MODE_IN_FLOAT);
}

void Hal_Com_Tx2_Disable(void )
{
    __GPIO_PIN_RESET(TX2_SW_PORT, TX2_SW_PIN);

    gpio_mode_set(TX2_CHRG_SW_PORT, TX2_CHRG_SW_PIN, GPIO_MODE_OUT_PP(GPIO_SPEED_HIGH));

    __GPIO_PIN_RESET(TX2_CHRG_SW_PORT, TX2_CHRG_SW_PIN);

    __USART_INTR_DISABLE(USART2, RXNE); // Enable the USART Receive interrupt
}

void Hal_Com_Tx2_Send(uint8_t *buf, uint16_t length, hal_isr_callback_t callback )
{
    pTx2Buf = buf;
    
    tx2Length = length;

    hal_tx2_isr_callback = callback;

    __USART_INTR_ENABLE(USART2, TXE); // Enable the USART transmit  interrupt
    
    __USART_INTR_DISABLE(USART2, RXNE); // Enable the USART Receive interrupt
}

void Hal_Com_Tx2_Isr_Handler(void )
{
    if (__USART_FLAG_STATUS_GET(USART2, TXE) == SET)
    {   
        if(tx2Length > 0)
        {
            __USART_DATA_SEND(USART2, *pTx2Buf);

            pTx2Buf++;
            
            tx2Length--;
        }
        else 
        {
            pTx2Buf = NULL;

            if(hal_tx2_isr_callback != NULL)
            {
                hal_tx2_isr_callback();

                hal_tx2_isr_callback = NULL;
            }
            /* Disable the USART transmit data register empty interrupt */
            __USART_INTR_DISABLE(USART2, TXE);
            
            __USART_INTR_ENABLE(USART2, RXNE); // Enable the USART Receive interrupt
        }
    }

    if (__USART_FLAG_STATUS_GET(USART2, RXNE) == SET)
    {
        uint8_t recvVal  = 0;
        
        recvVal = (uint8_t)__USART_DATA_RECV(USART2);

        if(hal_rx2_isr_callback != NULL)
        {
            hal_rx2_isr_callback(recvVal);
        }

        //__USART_INTR_DISABLE(USART2, RXNE);
    }
}

void Hal_Com_Tx6_Send(uint8_t *buf, uint16_t length, hal_isr_callback_t callback )
{
    pTx6Buf = buf;
    
    tx6Length = length;

    hal_tx6_isr_callback = callback;

    __USART_INTR_ENABLE(USART6, TXE); // Enable the USART transmit  interrupt

    __USART_INTR_DISABLE(USART6, RXNE);
}


void Hal_Com_Tx6_Isr_Handler(void )
{
    if (__USART_FLAG_STATUS_GET(USART6, TXE) == SET)
    {   
        if(tx6Length > 0)
        {
            __USART_DATA_SEND(USART6, *pTx6Buf);

            pTx6Buf++;
            
            tx6Length--;
        }
        else 
        {
            while ((__USART_FLAG_STATUS_GET(USART6, TC) == RESET));
            
            __USART_INTR_DISABLE(USART6, TXE);

            __USART_INTR_ENABLE(USART6, RXNE);
            
            pTx6Buf = NULL;

            if(hal_tx6_isr_callback != NULL)
            {
                hal_tx6_isr_callback();

                hal_tx6_isr_callback = NULL;
            }
            /* Disable the USART transmit data register empty interrupt */
            
        }
    }

    if (__USART_FLAG_STATUS_GET(USART6, RXNE) == SET)
    {
        uint8_t recvVal  = 0;
        
        recvVal = (uint8_t)__USART_DATA_RECV(USART6);

        if(hal_rx6_isr_callback != NULL)
        {
            hal_rx6_isr_callback(recvVal);
        }
    }
}



