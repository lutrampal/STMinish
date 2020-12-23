
/*******************************************************************************
 * Implementation file of RngDevice class for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../hardware/mcu.hpp"
#include "rng_device.hpp"

using namespace std;
using namespace stminish::device;


#ifdef MCU_STM32F750
/*******************************************************************************
 * CONSTRUCTORS & DESTRUCTOR
 ******************************************************************************/

RngDevice::RngDevice()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    /* PLLQ factor is set during boot */
    RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_CK48MSEL;
}

RngDevice::~RngDevice()
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

RngDevice& RngDevice::getInstance()
{
    static RngDevice instance;

    return instance;
}

void RngDevice::setRandCallback(
    std::function<void(ErrorStatus&&, uint32_t)> callback)
{
    this->callback = callback;
}

bool RngDevice::suspendRand()
{
    NVIC_DisableIRQ(RNG_IRQn);
    RNG->CR &= ~RNG_CR_RNGEN & ~RNG_CR_IE;
    return true;
}

void RngDevice::resumeRand()
{
    RNG->CR |= RNG_CR_RNGEN | RNG_CR_IE;
    NVIC_EnableIRQ(RNG_IRQn);
}

void RngDevice::completeRand()
{
    NVIC_DisableIRQ(RNG_IRQn);
    RNG->CR &= ~RNG_CR_IE & ~RNG_CR_RNGEN;

    if ((RNG->SR & RNG_SR_SEIS) || (RNG->SR & RNG_SR_CEIS)) {
        callback(ErrorCode::HwProtocolError, 0);
    } else if (RNG->SR & RNG_SR_DRDY) {
        callback(ErrorCode::Success, RNG->DR);
    }
}

#endif
