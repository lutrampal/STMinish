
/*******************************************************************************
 * Do not include direcly, first define MCU and BOARD then include mcu.hpp
 ******************************************************************************/

#ifndef _STM32F750_H
#define _STM32F750_H


#ifdef BOARD_STM32F7508_DK
    #include "stm32f7508-dk.hpp"
#else
    #error "Undefined board"
#endif

/** Offset to apply to each @ref IRQn_Type to get the actual handler index
 * in the vtable */
constexpr unsigned vtable_offset = 16;
/** Number of IRQS */
constexpr unsigned nb_irqs = 113;

/** CPU speed */
constexpr unsigned core_clk_hz = 216000000;

/* The output clock of the PLL is given by:
 * VCO_out_clk = (VCO_in_clk / PLLM)*PLLN
 * PLL_clk = VCO_out_clk/PLLP */
constexpr unsigned pllm = hse_in_clk_hz / 1000000;
constexpr unsigned pllp = 2;
constexpr unsigned plln = (core_clk_hz * pllp * pllm) / hse_in_clk_hz;

#endif