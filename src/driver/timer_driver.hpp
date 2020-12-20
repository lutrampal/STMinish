
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINNISH_DRIVER_TIMER_DRIVER_HPP
#define _STMINNISH_DRIVER_TIMER_DRIVER_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../device/error_status.hpp"
#include "../device/timer_device.hpp"
#include "../event_loop.hpp"

#include <chrono>
#include <functional>
#include <list>


namespace stminish
{
namespace driver
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class TimerDriver
{
  public:
    TimerDriver(EventLoop& event_loop);
    typedef unsigned Handle;

    class Timer
    {
      public:
        Timer(TimerDriver& owner, Handle handle);

        void cancelWait();

      private:
        TimerDriver& owner;
        Handle handle;
    };

    template<typename TRep, typename TPeriod>
    Timer startWait(const std::chrono::duration<TRep, TPeriod>& wait_time,
                    std::function<void(device::ErrorStatus&)> event_callback);

  private:
    struct WaitOp {
        Handle handle;
        device::TimerDevice::WaitTimeUnitDuration wait_time;
        std::function<void(device::ErrorStatus&)> callback;
    };

    EventLoop& event_loop;
    device::TimerDevice& dev;
    std::list<WaitOp> wait_queue;
    Handle next_handle;

    friend Timer;

    void completeWait(device::ErrorStatus&& status);
    void cancelWait(Handle handle);
};

}  // namespace driver
}  // namespace stminish

#include "timer_driver_impl.hpp"

#endif
