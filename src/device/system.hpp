
/*******************************************************************************
 * Interface file for the system singleton that is responsible for bringing up
 * and down all hardware ressources. It also stores references to these
 * resources for use by other components.
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_SYSTEM_HPP
#define _STMINISH_DEVICE_SYSTEM_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "rng_device.hpp"
#include "sd_card_device.hpp"
#include "timer_device.hpp"

#include <memory>

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITIONS
 ******************************************************************************/

class System
{
  public:
    System(System const&) = delete;
    System& operator=(System const&) = delete;

    static System& getInstance();

    RngDevice& getRng();
    TimerDevice& getTimer();
    SdCardDevice& getSdCard();

  private:
    System();
    ~System();

    std::unique_ptr<RngDevice> rng;
    std::unique_ptr<TimerDevice> timer;
    std::unique_ptr<SdCardDevice> sd_card;
};

}  // namespace device
}  // namespace stminish

#endif
