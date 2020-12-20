
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINNISH_DEVICE_TIMER_DEVICE_HPP
#define _STMINNISH_DEVICE_TIMER_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "context.hpp"
#include "error_status.hpp"

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

class TimerDevice
{
  public:
    static TimerDevice& getInstance();
    ~TimerDevice();

    TimerDevice(TimerDevice& other) = delete;
    void operator=(const TimerDevice&) = delete;

    typedef std::chrono::duration<uint32_t, std::micro> WaitTimeUnitDuration;

    void setWaitCallback(std::function<void(ErrorStatus&&)> callback);
    WaitTimeUnitDuration getRemainingWaitTime();
    void startWait(WaitTimeUnitDuration::rep count);
    bool suspendWait();
    bool cancelWait();
    void resumeWait();
    void completeWait();

  private:
    TimerDevice();

    /* We want one TIM2 tick per 1 µs (=> CK_CNT = 1000000 Hz) */
    unsigned ck_cnt = 1000000;
    WaitTimeUnitDuration::rep programmed_count;
    std::function<void(ErrorStatus&&)> callback;
};

}  // namespace device
}  // namespace stminish

#endif
