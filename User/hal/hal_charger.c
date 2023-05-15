/********************************************************
* @file       hal_bq24296.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_charger.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define I2C_LONG_TIMEOUT         ((uint32_t)(10 * I2C_FLAG_TIMEOUT))
#define I2C_TIMING              0x10420F13
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Hal_Charger_Intp_Init(void );
static void Hal_Charger_I2c_Init(void );
/* Private variables ------------------------------------*/
hal_isr_callback_t hal_intp1_isr_callback = NULL;
static __IO uint32_t max_delay = I2C_LONG_TIMEOUT;


void Hal_Charger_Init(void )
{
    Hal_Charger_Intp_Init();

    Hal_Charger_I2c_Init();
}

static void Hal_Charger_Intp_Init(void )
{
    nvic_config_t nvic_config_struct;

    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOF);    // Enable GPIOC clock
    __RCU_APB2_CLK_ENABLE(RCU_APB2_PERI_SYSCFG); // Enable SYSCFG clock

    // Configure PC8 and PC13 pins as input
    gpio_mode_set(GPIOF, GPIO_PIN_1, GPIO_MODE_IN_FLOAT);

    syscfg_exti_line_config(SYSCFG_EXTI_PORT_PF, SYSCFG_EXTI_PIN_1); // Connect EXTI1 Line to PF1 pin

    // Configure EXTI8 line
    __EXTI_INTR_ENABLE(EXTI_LINE_1);
    __EXTI_EDGE_ENABLE(EXTI_EDGE_FALLING, EXTI_LINE_1);
    __EXTI_EDGE_ENABLE(EXTI_EDGE_RISING, EXTI_LINE_1);

    // Enable and set EXTI4_15 Interrupt
    nvic_config_struct.IRQn = IRQn_EXTI0_1;
    nvic_config_struct.priority = 0x00;
    nvic_config_struct.enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);
}

static void Hal_Charger_I2c_Init(void )
{
    __RCU_AHB_CLK_ENABLE(RCU_AHB_PERI_GPIOA);
    __RCU_APB1_CLK_ENABLE(RCU_APB1_PERI_I2C1);

    // I2C Pin Config
    gpio_mf_config(GPIOA, GPIO_PIN_9, GPIO_MF_SEL4); // PA9--I2C_SCL
    gpio_mf_config(GPIOA, GPIO_PIN_10, GPIO_MF_SEL4); // PA10--I2C_SDA
    gpio_mode_set(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_MODE_MF_OD_PU(GPIO_SPEED_MEDIUM)); // PA9--I2C_SCL; PA10--I2C_SDA

    i2c_config_t i2c_config_struct;
    
    // I2C configuration
    i2c_config_struct.mode = I2C_BUS_MODE_I2C;
    i2c_config_struct.analog_flag = I2C_ANALOG_FILTER_ENABLE;
    i2c_config_struct.digital_value = 0x00;
    i2c_config_struct.slave_addr1 = 0x00;
    i2c_config_struct.ack_flag = I2C_ACK_ENABLE;
    i2c_config_struct.ack_addr = I2C_ACK_ADDR_7BITS;
    i2c_config_struct.tmr_value = I2C_TIMING;
    i2c_init(I2C1, &i2c_config_struct);

    __I2C_ENABLE(I2C1);
}

uint32_t Hal_Charger_Write(uint32_t devAddr, uint8_t *buf, uint8_t length )
{
    uint32_t data_num = 0;

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, BUSY) != RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }
    i2c_slave_address_config(I2C1, devAddr);
    i2c_request_direction_config(I2C1, I2C_DIRECTION_TX);
    i2c_bytes_number_config(I2C1, length);
    __I2C_START_GENERATE(I2C1);

    while (data_num != length)
    {
        max_delay = I2C_LONG_TIMEOUT;
        while (__I2C_FALG_STATUS_GET(I2C1, TXINT) == RESET)
        {
            if ((max_delay--) == 0)
                return 0;
        }
        __I2C_DATA_SEND(I2C1, (uint8_t)(buf[data_num]));
        data_num++;
    }

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, CMP) == RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }

    __I2C_STOP_GENERATE(I2C1);
    while (__I2C_FALG_STATUS_GET(I2C1, STOP) == RESET) // Wait until STOPF flag is set
    {
        if ((max_delay--) == 0)
            return 0;
    }
    __I2C_FLAG_CLEAR(I2C1, I2C_FLAG_STOP);

    return 1;
}

uint32_t Hal_Charger_Read(uint32_t devAddr, uint8_t regAddr, uint8_t *buf, uint16_t length )
{
    uint32_t data_num;

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, BUSY) != RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }

    i2c_slave_address_config(I2C1, devAddr);
    i2c_request_direction_config(I2C1, I2C_DIRECTION_TX);
    i2c_bytes_number_config(I2C1, 1);
    __I2C_START_GENERATE(I2C1);

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, TXINT) == RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }
    __I2C_DATA_SEND(I2C1, regAddr);

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, CMP) == RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }

    i2c_slave_address_config(I2C1, devAddr);
    i2c_request_direction_config(I2C1, I2C_DIRECTION_RX);
    /* Update CR2 : set Slave Address , set read request, generate Start and set end mode */
    i2c_bytes_number_config(I2C1, length);
    __I2C_START_GENERATE(I2C1);
    data_num = 0;

    while (data_num != length) // Wait until all data are received
    {
        max_delay = I2C_LONG_TIMEOUT;
        while (__I2C_FALG_STATUS_GET(I2C1, RXNE) == RESET) // Wait until RXNE flag is set
        {
            if ((max_delay--) == 0)
                return 0;
        }
        buf[data_num] = __I2C_DATA_RECV(I2C1);
        data_num++;
    }

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, CMP) == RESET)
    {
        if ((max_delay--) == 0)
            return 0;
    }
    __I2C_STOP_GENERATE(I2C1);

    max_delay = I2C_LONG_TIMEOUT;
    while (__I2C_FALG_STATUS_GET(I2C1, STOP) == RESET) // Wait until STOPF flag is set
    {
        if ((max_delay--) == 0)
            return 0;
    }
    __I2C_FLAG_CLEAR(I2C1, I2C_FLAG_STOP);
    return 1;
}



void Hal_Charger_Regist_Isr_Callback(hal_isr_callback_t callback )
{
    hal_intp1_isr_callback = callback;
}

void Hal_Charger_Isr_Handler(void )
{
    if(hal_intp1_isr_callback != NULL)
    {
        hal_intp1_isr_callback();
    }
}

