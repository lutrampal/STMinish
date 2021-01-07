
/*******************************************************************************
 * Interface file for random number generator device of STM32F750
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_STM32F750_RNG_DEVICE_HPP
#define _STMINISH_DEVICE_STM32F750_RNG_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "error_status.hpp"
#include "rng_device.hpp"

#include <functional>


namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class Stm32f750RngDevice : public RngDevice
{
  public:
    Stm32f750RngDevice();
    ~Stm32f750RngDevice();

    bool suspendRand() override;
    void resumeRand() override;
    void completeRand() override;
};

}  // namespace device
}  // namespace stminish

#endif
