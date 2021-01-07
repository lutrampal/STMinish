
/*******************************************************************************
 * Interface file for timer device of STM32F750
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_STM32F750_TIMER_DEVICE_HPP
#define _STMINISH_DEVICE_STM32F750_TIMER_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "error_status.hpp"
#include "timer_device.hpp"

#include <chrono>
#include <cstdint>
#include <functional>

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class Stm32f750TimerDevice : public TimerDevice
{
  public:
    Stm32f750TimerDevice();
    ~Stm32f750TimerDevice();

    WaitTimeUnitDuration getRemainingWaitTime() override;
    void startWait(WaitTimeUnitDuration::rep count) override;
    bool suspendWait() override;
    bool cancelWait() override;
    void resumeWait() override;
    void completeWait() override;
    /* An active wait that is sometimes handy when initializing peripherals and
     * such. No check is performed to see whether or not a Wait operation is
     * currently running so this should only be used during init, before
     * entering the event loop. */
    void usleep(WaitTimeUnitDuration::rep count) override;

  private:
    /* We want one TIM2 tick per 1 µs (=> CK_CNT = 1000000 Hz) */
    unsigned ck_cnt = 1000000;
};

}  // namespace device
}  // namespace stminish

#endif
