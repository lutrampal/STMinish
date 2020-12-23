
#include "device/error_status.hpp"
#include "driver/rng_driver.hpp"
#include "driver/timer_driver.hpp"
#include "event_loop.hpp"

#include <chrono>
#include <functional>

using namespace stminish;
using namespace std;
using namespace std::placeholders;

void r1(device::ErrorStatus& status, uint32_t res)
{
    static unsigned cnt = 0;
    static unsigned stats[16];
    if (status) {
        return;
    }

    for (int i = 0; i < 8; ++i) {
        stats[res & 0xF]++;
        res >>= 4;
    }
    cnt++;

    if (cnt == 10000) {
        uint32_t badcocoa = 0xbadc0c0a;
    }
}

int main(void)
{
    EventLoop el;
    driver::RngDriver rngd{el};

    function<void(device::ErrorStatus&, uint32_t)> callback = r1;
    for (int i = 0; i < 10000; ++i) { rngd.startRand(callback); }

    el.run();
}