/**
  * @file    system_cs32f0xx.c
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer System Source File.
  *          It contains the system clock configuration for CS32F0xx devices, and is 
  *          generated by the clock configuration .
  * 1.  The file provides two functions and one global variable to be called from user application: 
  *        SystemInit(): Setups the system clock (System clock source, PLL Multiplier and Divider  
  *                 factors,  AHB/APBx prescalers and Flash settings).It is called at startup just after                    
  *                reset and before branch to main program(in "startup_cs32f0xx.s" file).
  *        SystemCoreClock variable: Contains the core clock (HCLK), it can be used by the user 
  *                             application to setup the SysTick timer or configure other parameters.
  *        SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must be called 
  *                              whenever the core clock is changed  during program execution.
  * 2. After each device reset the HRC (8 MHz Range) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_cs32f0xx.s" file, to configure the 
  *    system clock before to branch to main program.
  * 3. When the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HRC still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.
  * 4. The default value of HXT crystal is set to 8MHz, refer to "HXT_VALUE" define
  *    in "cs32f0xx.h" file. When HXT is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HXT
  *    value to your own configuration.
  *
  * 5.The system clock Configuration as follows:
  *-----------------------------------------------------------------------------
  *        System Clock source     | PLL(HRC)
  *-----------------------------------------------------------------------------
  *        SYSCLK                       | 48000000 Hz
  *-----------------------------------------------------------------------------
  *        HCLK                          | 48000000 Hz
  *-----------------------------------------------------------------------------
  *        AHB Prescaler              | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler             | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler             | 1
  *-----------------------------------------------------------------------------
  *        HRC Frequency             | 8000000 Hz
  *-----------------------------------------------------------------------------
  *        PLL MUL                      | 12
  *-----------------------------------------------------------------------------
  *        VDD                           | 3.3 V
  *-----------------------------------------------------------------------------
  *       FMC wait conuter           | 1 
  *-----------------------------------------------------------------------------
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2021.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2021 ChipSea</center></h2>
  *
  */

#include "cs32f0xx.h"

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup cs32f0xx_system
  * @{
  */  


#define ENABLE_FWDT_FIX_001   // Comments this macro the disbale FWDT unintention reset issue fix  


uint32_t SystemCoreClock    = 48000000;
static __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};


static void SetSysClock(void);


/**
  * @fn void SystemInit (void)
  * @brief  Setup the microcontroller system. Initialize the Embedded Flash Interface,
  *        the PLL and update the SystemCoreClock variable.     
  * @param  None
  * @return None
  */
void SystemInit (void)
{   
#if defined(CS32F03X_RA) && defined(ENABLE_FWDT_FIX_001)    
    if ((RCU->STS & (RCU_STS_PORRSTF|RCU_STS_SWRSTF)) == RCU_STS_PORRSTF)       // only PORRSTF
    {        
        /* Trigger software reset */
        NVIC_SystemReset();
    } 
    else if ((RCU->STS & (RCU_STS_PORRSTF|RCU_STS_SWRSTF)) == (RCU_STS_PORRSTF|RCU_STS_SWRSTF)) // PORRSTF and SWRST
    {        
        /* Clear RESET flag in RCU_STS */        
        RCU->STS |= RCU_STS_CRSTF;
    }      
#endif 
 
    /* Set HRCON bit */
    RCU->CTR |= (uint32_t)0x00000001;

    /* Reset SYSSW[1:0], HCLKPDIV[3:0], PCLKPDIV[2:0], ADCPDIV, CKOSEL[2:0], CKOPDIV[2:0] and CKOPLLND bits */
    RCU->CFG &= (uint32_t)0x08FFB80C;
  
    /* Reset HXTEN, HXTME and PLLEN bits */
    RCU->CTR &= (uint32_t)0xFEF6FFFF;

    /* Reset HXTBPS bit */
    RCU->CTR &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSEL, PLLHXTPDIV and PLLMUF[3:0] bits */
    RCU->CFG &= (uint32_t)0xFFC0FFFF;

    /* Reset PLLPDIV[3:0] bits */
    RCU->CFG2 &= (uint32_t)0xFFFFFFF0;

    /* Reset USARTSEL[1:0], I2CSEL, CECSEL and ADCSEL bits */
    RCU->CFG3 &= (uint32_t)0xFFFFFEAC;

    /* Reset HRC14 bit */
    RCU->CTR2 &= (uint32_t)0xFFFFFFFE;

    /* Disable all interrupts */
    RCU->INTR = 0x00000000;

    /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
    SetSysClock();
}

/**
  * @fn void SystemCoreClockUpdate (void)
  * @brief  Update SystemCoreClock according to Clock Register Values
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  * @param  None
  * @return None
  */
void SystemCoreClockUpdate (void)
{
    uint32_t temp = 0;
    uint32_t pllmull = 0;
    uint32_t pllsource = 0;
    uint32_t prediv1factor = 0;

    temp = RCU->CFG & RCU_CFG_SYSSS;
  
    switch (temp)
    {
        case 0x00:  /* HRC used as system clock */
            SystemCoreClock = HRC_VALUE;
            break;
	  
        case 0x04:  /* HXT used as system clock */
            SystemCoreClock = HXT_VALUE;
            break;
	  
        case 0x08:  /* PLL used as system clock */
            pllmull = RCU->CFG & RCU_CFG_PLLMUF;
            pllsource = RCU->CFG & RCU_CFG_PLLSEL;
            pllmull = ( pllmull >> 18) + 2;
            
            if (pllsource == 0x00)
            {
                /* HRC oscillator clock divided by 2 selected as PLL clock entry */
                SystemCoreClock = (HRC_VALUE >> 1) * pllmull;
            }
            else
            {
                prediv1factor = (RCU->CFG2 & RCU_CFG2_PLLPDIV1) + 1;
                /* HXT oscillator clock selected as PLLPDIV clock entry */
                SystemCoreClock = (HXT_VALUE / prediv1factor) * pllmull; 
            }      
            break;
	  
        default: 
            SystemCoreClock = HRC_VALUE;
            break;
    }
  
    /* Get HCLK prescaler */
    temp = AHBPrescTable[((RCU->CFG & RCU_CFG_HCLKPDIV) >> 4)];
    /* HCLK clock frequency */
    SystemCoreClock >>= temp;  
}

/**
  * @fn static void SetSysClock(void)
  * @brief  Configures the System clock frequency, AHB/APBx prescalers  
  *            and Flash settings. 
  * @param  None
  * @return None
  */
static void SetSysClock(void)
{
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->WCR = FMC_WCR_WE | FMC_WCR_WCNT;
 
    /* HCLK = SYSCLK */
    RCU->CFG |= (uint32_t)RCU_CFG_HCLKPDIV_DIV1;
      
    /* PCLK = HCLK */
    RCU->CFG |= (uint32_t)RCU_CFG_PCLKPDIV_DIV1;

    /* PLL configuration = HRC/2 * 12 = 48 MHz */
    RCU->CFG &= (uint32_t)((uint32_t)~(RCU_CFG_PLLSEL | RCU_CFG_PLLSEL_HRC_DIV2 | RCU_CFG_PLLMUF));
    RCU->CFG |= (uint32_t)(RCU_CFG_PLLSEL_HRC_DIV2 | RCU_CFG_PLLMUF12);
            
    /* Enable PLL */
    RCU->CTR |= RCU_CTR_PLLEN;

    /* Wait till PLL is ready */
    while((RCU->CTR & RCU_CTR_PLLSTAB) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCU->CFG &= (uint32_t)((uint32_t)~(RCU_CFG_SYSSW));
    RCU->CFG |= (uint32_t)RCU_CFG_SYSSW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCU->CFG & (uint32_t)RCU_CFG_SYSSS) != (uint32_t)RCU_CFG_SYSSS_PLL)
    {
    }

}

/**
  * @}
  */

/**
  * @}
  */

