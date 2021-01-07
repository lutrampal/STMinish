
/*******************************************************************************
 * Implementation file of RngDevice class for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "stm32f750_rng_device.hpp"

#include "../hardware/mcu.hpp"

using namespace std;
using namespace stminish::device;


/*******************************************************************************
 * CONSTRUCTORS & DESTRUCTOR
 ******************************************************************************/

Stm32f750RngDevice::Stm32f750RngDevice()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    /* PLLQ factor is set during boot */
    RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_CK48MSEL;
}

Stm32f750RngDevice::~Stm32f750RngDevice()
{
    NVIC_DisableIRQ(RNG_IRQn);
    RNG->CR = 0;
    RCC->AHB2ENR &= ~RCC_AHB2ENR_RNGEN;
}

/*******************************************************************************
 * OPERATOR IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/

bool Stm32f750RngDevice::suspendRand()
{
    NVIC_DisableIRQ(RNG_IRQn);
    RNG->CR &= ~RNG_CR_RNGEN & ~RNG_CR_IE;
    return true;
}

void Stm32f750RngDevice::resumeRand()
{
    RNG->CR |= RNG_CR_RNGEN | RNG_CR_IE;
    NVIC_EnableIRQ(RNG_IRQn);
}

void Stm32f750RngDevice::completeRand()
{
    NVIC_DisableIRQ(RNG_IRQn);
    RNG->CR &= ~RNG_CR_IE & ~RNG_CR_RNGEN;

    if ((RNG->SR & RNG_SR_SEIS) || (RNG->SR & RNG_SR_CEIS)) {
        callback(ErrorCode::HwProtocolError, 0);
    } else if (RNG->SR & RNG_SR_DRDY) {
        callback(ErrorCode::Success, RNG->DR);
    }
}
