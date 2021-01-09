
/*******************************************************************************
 * Interface file for timer devices
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_SD_CARD_EXCEPTIONS_HPP
#define _STMINISH_DEVICE_SD_CARD_EXCEPTIONS_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "sd_card_specs.hpp"

namespace stminish
{
namespace device
{
/*******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/

struct SdCardException : std::exception {
  protected:
    SdCardException()
    {
    }
};

struct NoSdCardException : SdCardException {
    const char* what()
    {
        return "No SD card was detected in the slot";
    }
};

struct SdCardCmdFailure : SdCardException {
    uint8_t idx;
    uint32_t arg;

    SdCardCmdFailure(uint8_t idx, uint32_t arg): idx{idx}, arg{arg}
    {
    }

    virtual const char* what()
    {
        return "SD card command failed";
    }
};

struct InvalidSdCardStateException : SdCardException {
    sdcard::SdCardState expected;
    sdcard::SdCardState actual;

    InvalidSdCardStateException(sdcard::SdCardState expected,
                                sdcard::SdCardState actual)
    : expected{expected}, actual{actual}
    {
    }

    const char* what()
    {
        return "SD card is not in the state it is suppposed to be";
    }
};

struct UnsupportedSdCardException : SdCardException {
    const char* what()
    {
        return "Inserted SD Card is not supported (only SDHC cards are "
               "supported)";
    }
};

struct SdCardCmdTimeout : SdCardCmdFailure {
    SdCardCmdTimeout(uint8_t idx, uint32_t arg): SdCardCmdFailure(idx, arg)
    {
    }

    const char* what() override
    {
        return "SD card command failed with timeout";
    }
};

struct SdCardCmdCrcCheckFailure : SdCardCmdFailure {
    SdCardCmdCrcCheckFailure(uint8_t idx, uint32_t arg)
    : SdCardCmdFailure(idx, arg)
    {
    }

    const char* what() override
    {
        return "CRC check failed for SD card command response";
    }
};

}  // namespace device
}  // namespace stminish

#endif
