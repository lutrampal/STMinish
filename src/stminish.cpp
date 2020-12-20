
#include "device/error_status.hpp"
#include "driver/timer_driver.hpp"
#include "event_loop.hpp"

#include <chrono>
#include <functional>

using namespace stminish;
using namespace std;
using namespace std::placeholders;

void t1(device::ErrorStatus& status)
{
    uint32_t deadbeef = 0xdeadbeef;
}


void t2(TimerDriver::Timer& tim, device::ErrorStatus& status)
{
    uint32_t deadbeef = 0xdeadbeef;
    tim.cancelWait();
}

void t3(device::ErrorStatus& status)
{
    uint32_t deadbeef = 0xdeadbeef;
}

void t4(device::ErrorStatus& status)
{
    uint32_t deadbeef = 0xdeadbeef;
}

void t5(device::ErrorStatus& status)
{
    uint32_t deadbeef = 0xdeadbeef;
}


int main(void)
{
    EventLoop el;
    driver::TimerDriver td{el};

    function<void(device::ErrorStatus&)> callback = t3;
    TimerDriver::Timer tim = td.startWait(chrono::seconds(14), callback);
    callback               = t4;
    td.startWait(chrono::seconds(19), callback);
    callback = bind(t2, tim, _1);
    td.startWait(chrono::seconds(8), callback);
    callback = t1;
    td.startWait(chrono::seconds(5), callback);

    el.run();
}