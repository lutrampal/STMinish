
/*******************************************************************************
 * This file should be included in all modules that need direct access to the
 * hardware.
 * MCU and BOARD should be defined in make process before including this file.
 ******************************************************************************/

#ifndef _MCU_H
#define _MCU_H

namespace hardware
{
#ifdef MCU_STM32F750
    #include "stm32f750.hpp"
#else
    #error "Undefined MCU"
#endif
}  // namespace hardware


#endif
