
/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "timer_driver.hpp"

#include "../device/irqs.hpp"
#include "driver_exceptions.hpp"

#include <algorithm>


using namespace std;
using namespace std::placeholders;
using namespace stminish::driver;
using namespace stminish::device;


/*******************************************************************************
 * CONSTRUCTORS & DESTRUCTOR
 ******************************************************************************/

TimerDriver::Timer::Timer(TimerDriver& owner, Handle handle)
: owner{owner}, handle{handle}
{
}

TimerDriver::TimerDriver(EventLoop& event_loop, device::TimerDevice& dev)
: event_loop{event_loop}, dev{dev}
{
    dev.setWaitCallback(bind(&TimerDriver::completeWait, this, _1));
}


/*******************************************************************************
 * OPERATOR IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE METHOD IMPLEMENTATIONS
 ******************************************************************************/

void TimerDriver::completeWait(device::ErrorStatus&& status)
{
    event_loop.pushEvent(bind(wait_queue.front().callback, status));
    wait_queue.pop_front();

    if (!wait_queue.empty()) {
        dev.startWait(wait_queue.front().wait_time.count());
    }
}

void TimerDriver::cancelWait(Handle handle)
{
    /* Disable Timer IRQ: We don't want the timer callback accessing internal
     * class data while we're removing this wait op */
    if (!dev.suspendWait()) {
        throw CancelAsyncOpFailure{};
    }

    if (wait_queue.empty()) {
        /* Nothing to cancel, maybe the timer already went off and it was the
         * only one? */
        dev.resumeWait();
        throw CancelAsyncOpFailure{};
    }

    if (wait_queue.front().handle == handle) {
        TimerDevice::WaitTimeUnitDuration canceled_wait_time =
            dev.getRemainingWaitTime();
        /* This wait operation has already started, ask the device for
         * cancelation */
        if (!dev.cancelWait()) {
            /* Too late */
            dev.resumeWait();
            throw CancelAsyncOpFailure{};
        }
        /* The operation was cancelled and the completion handler won't be
         * called, signal the abortion event to the loop */
        event_loop.pushEvent(
            bind(wait_queue.front().callback, ErrorStatus{ErrorCode::Aborted}));
        wait_queue.pop_front();

        if (!wait_queue.empty()) {
            dev.startWait(
                (wait_queue.front().wait_time + canceled_wait_time).count());
        }
    } else {
        /* The wait operation hasn't started, or maybe it was already executed.
         * We'll remove it from the wait queue & add its wait time to the
         * request that was supposed to run after it */
        auto it =
            find_if(++wait_queue.begin(), wait_queue.end(),
                    [handle](const WaitOp& op) { return op.handle == handle; });
        if (it == wait_queue.end()) {
            /* The handle does not exist, the wait operation was already
             * executed */
            dev.resumeWait();
            throw CancelAsyncOpFailure{};
        } else {
            TimerDevice::WaitTimeUnitDuration canceled_wait_time =
                it->wait_time;
            /* The operation was cancelled and the completion handler won't be
             * called, signal the abortion event to the loop */
            event_loop.pushEvent(
                bind(it->callback, ErrorStatus{ErrorCode::Aborted}));
            it = wait_queue.erase(it);
            it->wait_time += canceled_wait_time;
        }
    }

    dev.resumeWait();
}


/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/

void TimerDriver::Timer::cancelWait()
{
    owner.cancelWait(handle);
}
