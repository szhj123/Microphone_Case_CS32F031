/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        15. April 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for Chipsea CS32F031xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char    vu8;
typedef          unsigned char     u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define FWDT_BASE       0x40003000
#define FLASH_BASE      0x40022000

#define FWDT            ((fwdt_reg_t *) FWDT_BASE)
#define FLASH           ((fmc_reg_t*) FLASH_BASE)


/** 
  * @struct fwdt_reg_t
  * @brief FWDT(Free Watchdog Timer) Registers structure definition
  */
typedef struct
{
  vu32 CCODE;   //!< FWDT Control code register,  Address offset: 0x00. 
  vu32 PDIV;    //!< FWDT Prescaler register,     Address offset: 0x04. 
  vu32 UVAL;    //!< FWDT Reload register,        Address offset: 0x08. 
  vu32 STS;     //!< FWDT Status register,        Address offset: 0x0C. 
  vu32 WINVAL; //!< FWDT Window register,          Address offset: 0x10. 
} fwdt_reg_t;

/** 
  * @struct fmc_reg_t
  * @brief FMC (FLASH Memory Control )Registers structure definition 
  */
typedef struct
{
  vu32 WCR;          //!<FMC Wait control register,              Address offset: 0x00 
  vu32 CCODE;         //!<FMC Control code register              Address offset: 0x04 
  vu32 OBCCODE;      //!<FMC Option Byte Control code register     Address offset: 0x08 
  vu32 STS;          //!<FMC status register,                    Address offset: 0x0C 
  vu32 CTR;          //!<FMC Control register,                   Address offset: 0x10
  vu32 ERSADR;       //!<Flash erase address register,           Address offset: 0x14 
  vu32 RESERVED;     //!< Reserved,                                             0x18 
  vu32 OBSTS;        //!<Option byte status register,            Address offset: 0x1C
  vu32 WPSTS;        //!<Write protection status register,       Address offset: 0x20 
} fmc_reg_t;


// Flash Keys
#define RDPRT_KEY               ((unsigned int)    0x55AA)

/** 
  * @brief FMC CCODE Bit definition
  */
#define FMC_CCODE_VALUE1            ((unsigned int) 0x45670123)        //!< Flash program erase key1 
#define FMC_CCODE_VALUE2            ((unsigned int) 0xCDEF89AB)        //!< Flash program erase key2
                                                                                                                                          
#define FMC_OBCCODE_VALUE1          ((unsigned int) 0x45670123)        //!< Flash option key1
#define FMC_OBCCODE_VALUE2          ((unsigned int) 0xCDEF89AB)        //!< Flash option key2

/** 
  * @brief FMC_CTR register Bit definition
  */
#define  FMC_CTR_PG                ((unsigned int) 0x00000001)        //!< Main flash program
#define  FMC_CTR_PERS              ((unsigned int) 0x00000002)        //!< Flash page erase
#define  FMC_CTR_CHIPERS           ((unsigned int) 0x00000004)        //!< Mass flash CHIP ERASE
#define  FMC_CTR_OBPG              ((unsigned int) 0x00000010)        //!< Option byte program command

#define  FMC_CTR_OBERS             ((unsigned int) 0x00000020)        //!< Option bytes erase command
#define  FMC_CTR_START             ((unsigned int) 0x00000040)        //!< Start of erase
#define  FMC_CTR_LOCK              ((unsigned int) 0x00000080)        //!< Lock 
#define  FMC_CTR_OBWEN             ((unsigned int) 0x00000200)  
#define  FMC_CTR_OBUPDATE          ((unsigned int) 0x00002000)       
/** 
  * @brief FMC_STS register Bit definition
  */
#define  FMC_STS_BUSY                 ((unsigned int) 0x00000001)       //!< Busy
#define  FMC_STS_PGERR                ((unsigned int) 0x00000004)       //!< Programming Error 
#define  FMC_STS_WPERR                ((unsigned int) 0x00000010)       //!< Write Protection Error 
#define  FMC_STS_ENDF                 ((unsigned int) 0x00000020) 

#define  FLASH_STS_ERR               (FMC_STS_PGERR | FMC_STS_WPERR)


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  FLASH->CCODE = FMC_CCODE_VALUE1;                        	// Unlock Flash
  FLASH->CCODE = FMC_CCODE_VALUE2;

#ifdef CS32F03x_OPTION
  FLASH->OBCCODE = FMC_OBCCODE_VALUE1;                   	// Unlock Option Bytes
  FLASH->OBCCODE = FMC_OBCCODE_VALUE2;
#endif	
	
  FLASH->WCR  = 0x00000000;                             	// Zero Wait State, no Prefetch
  FLASH->STS  |= FLASH_STS_ERR;                          	// Reset Error Flags

  if ((FLASH->OBSTS & 0x04) == 0x00) {                    	// Test if FWDT is running (FWDT in HW mode)
    // Set FWDT time out to ~32.768 second
    FWDT->CCODE  = 0x5555;                                 	// Enable write access to FWDT_PDIV and FWDT_UVAL     
    FWDT->PDIV  = 0x06;                                   	// Set prescaler to 256  
    FWDT->UVAL = 4095;                                   	// Set reload value to 4095
  }

  return (0);
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int UnInit (unsigned long fnc) {

  FLASH->CTR |=  FMC_CTR_LOCK;                          // Lock Flash

#ifdef CS32F03x_OPTION  
  FLASH->CTR &= ~FMC_CTR_OBWEN;                        // Lock Option Bytes
#endif
  
  return (0);
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void) {

  FLASH->STS  |= FLASH_STS_ERR;                     	// Reset Error Flags
	
#ifdef CS32F03x_OPTION	
  FLASH->CTR |=  FMC_CTR_OBERS;                    		// Option Byte Erase Enabled 
  FLASH->CTR |=  FMC_CTR_START;                       	// Start Erase

  while (FLASH->STS & FMC_STS_BUSY) {
    FWDT->CCODE = 0xAAAA;                          		// Reload FWDT
  }

  FLASH->CTR &= ~FMC_CTR_OBERS;                      	// Option Byte Erase Disabled 

  // Unprotect Flash 
  FLASH->CTR |=  FMC_CTR_OBPG;                       	// Option Byte Programming Enabled

  M16(0x1FFFF800) = 0x55AA;                          	// Program Half Word: RDPRT Key
  while (FLASH->STS & FMC_STS_BUSY) {
    FWDT->CCODE = 0xAAAA;                            	// Reload FWDT
  }

  FLASH->CTR &= ~FMC_CTR_OBPG;                      	// Option Byte Programming Disabled
														
#else	
  FLASH->CTR |=  FMC_CTR_CHIPERS;             			// Mass Erase Enabled
  FLASH->CTR |=  FMC_CTR_START;                    		// Start Erase

  while (FLASH->STS & FMC_STS_BUSY) {
    FWDT->CCODE = 0xAAAA;                         		// Reload FWDT
  }

  FLASH->CTR &= ~FMC_CTR_CHIPERS;                   	// Mass Erase Disabled

#endif  
  
  if (FLASH->STS & FLASH_STS_ERR) {        				// Check for Errors                 
    FLASH->STS |= FLASH_STS_ERR;
    return (1);                                         // Failed
  }

  return (0);                                           // Done

}

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseSector (unsigned long adr) {

  FLASH->STS  |= FLASH_STS_ERR;                    		// Reset Error Flags

#ifdef CS32F03x_OPTION	
  FLASH->CTR |=  FMC_CTR_OBERS;                      	// Option Byte Erase Enabled 	
#else
  FLASH->CTR  |=  FMC_CTR_PERS;                   		// Page Erase Enabled 
  FLASH->ERSADR  =  adr;                             	// Page Address
#endif	
	
  FLASH->CTR  |=  FMC_CTR_START;                   		// Start Erase

  while (FLASH->STS  & FMC_STS_BUSY) {
    FWDT->CCODE = 0xAAAA;                            	// Reload FWDT
  }

#ifdef CS32F03x_OPTION	  
  FLASH->CTR &= ~FMC_CTR_OBERS;                      	// Option Byte Erase Disabled 
#else  
  FLASH->CTR &= ~FMC_CTR_PERS;                      	// Page Erase Disabled 
#endif
  
  if (FLASH->STS & FLASH_STS_ERR)  {               		// Check for Errors 
    FLASH->STS |= FLASH_STS_ERR;
    return (1);                                      	// Failed
  }

  return (0);                                        	// Done
}

/*  
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) {
  return (1);                                         	// Always Force Erase
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 1) & ~1;                                   // Adjust size for Half Words
  
  FLASH->STS |= FMC_STS_PGERR;                     		// Reset Error Flags

  while (sz) {
	  
#ifdef CS32F03x_OPTION
	FLASH->CTR |= FMC_CTR_OBPG;                       	// Option Byte Programming Enabled
#else	  
    FLASH->CTR |= FMC_CTR_PG;                        	// Programming Enabled
#endif
	  
    M16(adr) = *((u16 *)buf);                         	// Program Half Word
    while (FLASH->STS & FMC_STS_BUSY) {
      FWDT->CCODE = 0xAAAA;                           	// Reload FWDT
	}

#ifdef CS32F03x_OPTION
    FLASH->CTR &= ~FMC_CTR_OBPG;                      	// Option Byte Programming Disabled
#else	
	FLASH->CTR &= ~FMC_CTR_PG;                        	// Programming Disabled
#endif
	
	if (FLASH->STS & FLASH_STS_ERR) {      				// Check for Errors                    
      FLASH->STS |= FLASH_STS_ERR;
      return (1);                                       // Failed
	}

    adr += 2;                                           // Go to next Half Word
    buf += 2;
    sz  -= 2;
  }

  return (0);                                           // Done
}

/*  
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u16 temp = 0x00;
  
  sz = (sz + 1) & ~1;									// Adjust size for Half Words
	
  while (sz) {

#ifdef CS32F03x_OPTION 
	temp = M16(buf) & 0x00FF;
	temp |= (~temp) << 8;
#else	  
	temp = M16(buf);
#endif
	  
	if (M16(adr) != temp) {								// Compare
		 return (adr);									// Return Failed Address
    }	
	
    adr += 2;                                           // Go to next Half Word
    buf += 2;
    sz  -= 2;
  }	  
	
  return (adr);                                  		// Finished without Errors  	
}

