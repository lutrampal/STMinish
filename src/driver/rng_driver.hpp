
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINISH_DRIVER_RNG_DRIVER_HPP
#define _STMINISH_DRIVER_RNG_DRIVER_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../device/error_status.hpp"
#include "../device/rng_device.hpp"
#include "../event_loop.hpp"

#include <functional>
#include <list>


namespace stminish
{
namespace driver
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class RngDriver
{
  public:
    RngDriver(EventLoop& event_loop, device::RngDevice& dev);

    void startRand(
        std::function<void(device::ErrorStatus&, uint32_t)> event_callback);

  private:
    struct RandOp {
        std::function<void(device::ErrorStatus&, uint32_t)> callback;
    };

    EventLoop& event_loop;
    device::RngDevice& dev;
    std::list<RandOp> rand_queue;

    void completeRand(device::ErrorStatus&& status, uint32_t rand);
};

}  // namespace driver
}  // namespace stminish

#endif
