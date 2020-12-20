
/*******************************************************************************
 * Implementation file of TimerDevice class for STM32F750 MCU, we'll use TIM2
 * as our general purpose timer
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../hardware/mcu.hpp"
#include "timer_device.hpp"

using namespace std;
using namespace stminish::device;


#ifdef MCU_STM32F750
/*******************************************************************************
 * CONSTRUCTORS & DESTRUCTOR
 ******************************************************************************/

TimerDevice::TimerDevice()
{
    NVIC_SetPriority(TIM2_IRQn, 0);
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* Suspend the timer when the core is halted by debug */
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;

    /* Disable TIM2 while we are configuring it */
    TIM2->CR1 &= ~TIM_CR1_CEN;
    /* Reset TIM2 */
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;

    /* Increment the time counter every PSC + 1 clock ticks */
    TIM2->PSC = (uint16_t)(core_clk_hz / ck_cnt - 1);
    /* Enable IRQ generation based on TIM2 events */
    TIM2->DIER |= TIM_DIER_UIE;
}

TimerDevice::~TimerDevice()
{
    TIM2->DIER &= ~TIM_DIER_UIE;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
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

TimerDevice& TimerDevice::getInstance()
{
    static TimerDevice instance;

    return instance;
}

void TimerDevice::setWaitCallback(std::function<void(ErrorStatus&&)> callback)
{
    this->callback = callback;
}

TimerDevice::WaitTimeUnitDuration TimerDevice::getRemainingWaitTime()
{
    return TimerDevice::WaitTimeUnitDuration{programmed_count - TIM2->CNT};
}

void TimerDevice::startWait(WaitTimeUnitDuration::rep count)
{
    NVIC_EnableIRQ(TIM2_IRQn);
    /* Set the event period:
     * A TIM2 event is generated when the counter is equal to ARR */
    TIM2->ARR        = count;
    programmed_count = count;
    /* URS is set to avoid generating interrupt when we set UG, OPM is used to
     * turn off the counter once it went off. */
    TIM2->CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    /* Reset the counter and apply new config */
    TIM2->EGR |= TIM_EGR_UG;
    /* Enable TIM2 */
    TIM2->CR1 |= TIM_CR1_CEN;
}

bool TimerDevice::suspendWait()
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    NVIC_DisableIRQ(TIM2_IRQn);
    return true;
}

bool TimerDevice::cancelWait()
{
    /* Nothing to do in particular aside from suspending, settings will be
     * erased by next startWait() call */
    TIM2->CR1 &= ~TIM_CR1_CEN;
    NVIC_DisableIRQ(TIM2_IRQn);
    return true;
}

void TimerDevice::resumeWait()
{
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TimerDevice::completeWait()
{
    /* No need to disable the timer as we've set TIM_CR1_OPM */
    callback(ErrorStatus{ErrorCode::Success});
}

#endif
