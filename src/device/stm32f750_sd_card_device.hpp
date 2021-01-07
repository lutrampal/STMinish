
/*******************************************************************************
 * Interface file for SD Card devices
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_STM32F750_SD_CARD_DEVICE_HPP
#define _STMINISH_DEVICE_STM32F750_SD_CARD_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "sd_card_device.hpp"
#include "timer_device.hpp"

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITIONS
 ******************************************************************************/

class Stm32f750SdCardDevice : public SdCardDevice
{
  protected:
    bool isSdCardDetected() override;
    void setupGpios() override;
    void execCmd(uint8_t cmd_idx,
                 ExpectedCmdResp resp_type = ExpectedCmdResp::None,
                 uint32_t arg              = 0,
                 bool ignore_crc           = false) override;
    uint32_t getShortResponse() override;
    std::array<uint32_t, 4> getLongResponse() override;
    /* Return card clock to setup delay for power up sequence */
    uint32_t enableDevice() override;
    void disableDevice() override;
};

}  // namespace device
}  // namespace stminish

#endif
