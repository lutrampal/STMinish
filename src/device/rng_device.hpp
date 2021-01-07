
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
    void setRandCallback(std::function<void(ErrorStatus&&, uint32_t)> callback);

    virtual bool suspendRand()  = 0;
    virtual void resumeRand()   = 0;
    virtual void completeRand() = 0;

  protected:
    std::function<void(ErrorStatus&&, uint32_t)> callback;
};

}  // namespace device
}  // namespace stminish

#endif
