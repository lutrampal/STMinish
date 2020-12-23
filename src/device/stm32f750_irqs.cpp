
/*******************************************************************************
 * Implementation file of IRQ helper functions for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../hardware/mcu.hpp"
#include "irqs.hpp"
#include "rng_device.hpp"
#include "timer_device.hpp"

using namespace stminish;
using namespace stminish::device;

#ifdef MCU_STM32F750
extern "C" {
/*******************************************************************************
 * EXTERN VARIABLE DEFINITIONS
 ******************************************************************************/

__attribute__((aligned(0x200))) __attribute__((
    section(".dtcm_vtable"))) volatile InterruptHandler g_vtable[nb_irqs];


/*******************************************************************************
 * EXTERN FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

void handleError(void)
{
    while (1) {};
}

void handleTIM2Event(void)
{
    if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF) {
        /* clear interrupt */
        TIM2->SR &= ~TIM_SR_UIF;
        try {
            TimerDevice::getInstance().completeWait();
        } catch (const std::exception& e) {
            // TODO: what can we do here?..
        }
    }
}

void handleRNGEvent(void)
{
    try {
        RngDevice::getInstance().completeRand();
    } catch (const std::exception& e) {
        // TODO: what can we do here?..
    }
}
}

#endif
