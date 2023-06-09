#ifndef _HAL_CS32F031_H
#define _HAL_CS32F031_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs32f0xx.h"
#include "cs32f0xx_conf.h"

typedef union _word_t
{
    uint32_t val;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte_t;
}word_t;


typedef void (*hal_isr_callback_t)(void );


#endif 

