
/*******************************************************************************
 * Interface file for random number generator devices
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_RNG_DEVICE_HPP
#define _STMINISH_DEVICE_RNG_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "error_status.hpp"

#include <functional>


namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class RngDevice
{
  public:
    static RngDevice& getInstance();
    ~RngDevice();

    RngDevice(RngDevice& other) = delete;
    void operator=(const RngDevice&) = delete;

    void setRandCallback(std::function<void(ErrorStatus&&, uint32_t)> callback);
    bool suspendRand();
    void resumeRand();
    void completeRand();

  private:
    RngDevice();

    std::function<void(ErrorStatus&&, uint32_t)> callback;
};

}  // namespace device
}  // namespace stminish

#endif
