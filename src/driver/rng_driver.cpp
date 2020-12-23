
/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "rng_driver.hpp"

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

RngDriver::RngDriver(EventLoop& event_loop)
: event_loop{event_loop}, dev{RngDevice::getInstance()}
{
    dev.setRandCallback(bind(&RngDriver::completeRand, this, _1, _2));
}


/*******************************************************************************
 * OPERATOR IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE METHOD IMPLEMENTATIONS
 ******************************************************************************/

void RngDriver::completeRand(device::ErrorStatus&& status, uint32_t rand)
{
    event_loop.pushEvent(bind(rand_queue.front().callback, status, rand));
    rand_queue.pop_front();

    if (!rand_queue.empty()) {
        dev.resumeRand();
    }
}


/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/

void RngDriver::startRand(function<void(ErrorStatus&, uint32_t)> event_callback)
{
    dev.suspendRand();

    if (rand_queue.empty()) {
        rand_queue.push_front(RandOp{event_callback});
    } else {
        rand_queue.push_back(RandOp{event_callback});
    }

    dev.resumeRand();
}
