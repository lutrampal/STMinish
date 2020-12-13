
/*******************************************************************************
 * Do not include direcly, first define MCU and BOARD then include mcu.hpp
 ******************************************************************************/

#ifndef _STM32F750_H
#define _STM32F750_H


#include <stm32f750xx.h>


#ifdef BOARD_STM32F7508_DK
    #include "stm32f7508-dk.hpp"
#else
    #error "Undefined board"
#endif

/** Offset to apply to each @ref IRQn_Type to get the actual handler index in
 * the vtable */
#define VTABLE_OFFSET (16)
/** Number of IRQS */
#define NB_IRQS (113)


#endif