
/*******************************************************************************
 * Interface file for timer devices
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_TIMER_DEVICE_HPP
#define _STMINISH_DEVICE_TIMER_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

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
    typedef std::chrono::duration<uint32_t, std::micro> WaitTimeUnitDuration;

    void setWaitCallback(std::function<void(ErrorStatus&&)> callback);
    virtual WaitTimeUnitDuration getRemainingWaitTime()     = 0;
    virtual void startWait(WaitTimeUnitDuration::rep count) = 0;
    virtual bool suspendWait()                              = 0;
    virtual bool cancelWait()                               = 0;
    virtual void resumeWait()                               = 0;
    virtual void completeWait()                             = 0;
    /* An active wait that is sometimes handy when initializing peripherals and
     * such. No check is performed to see whether or not a Wait operation is
     * currently running so this should only be used during init, before
     * entering the event loop. */
    virtual void usleep(WaitTimeUnitDuration::rep count) = 0;

  protected:
    WaitTimeUnitDuration::rep programmed_count;
    std::function<void(ErrorStatus&&)> callback;
};

}  // namespace device
}  // namespace stminish

#endif
