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
 * $Date:        15. December 2014
 * $Revision:    V1.01
 *  
 * Project:      Flash Device Description for Chipsea CS32F031xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 
 
#include "..\FlashOS.H"        // FlashOS Structures



#if defined CS32F03x_32
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32F03x 32KB Flash",      // Device Name (32kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00008000,                 // Device Size in Bytes (32kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   6000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0400, 0x000000,           // Sector Size  1kB (32 Sectors)
   SECTOR_END
};

#elif defined CS32F03x_64
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32F03x 64KB Flash",      // Device Name (64kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00010000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   6000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0400, 0x000000,           // Sector Size  1kB (64 Sectors)
   SECTOR_END
};

#elif defined CS32F03x_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32F03x 128KB Flash",     // Device Name (128kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (128kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   6000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0400, 0x000000,           // Sector Size  1kB (128 Sectors)
   SECTOR_END
};

#elif defined CS32F03x_BOOT
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32F03x Flash Boot",      // Device Name
   ONCHIP,                     // Device Type
   0x1FFFEC00,                 // Device Start Address
   0x00000C00,                 // Device Size in Bytes (3kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0400, 0x000000,           // Sector Size 1kB (3 Sectors)
   SECTOR_END
};

#elif defined CS32F03x_OPTION
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32F03x Flash Options",   // Device Name
   ONCHIP,                     // Device Type
   0x1FFFF800,                 // Device Start Address
   0x00000010,                 // Device Size in Bytes (16)
   16,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0010, 0x000000,           // Sector Size 16B
   SECTOR_END
};

#elif defined CS32A039_64
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "CS32A039 64KB Flash",      // Device Name (64kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00010000,                 // Device Size in Bytes (64kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   6000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0400, 0x000000,           // Sector Size  1kB (64 Sectors)
   SECTOR_END
};
#endif
