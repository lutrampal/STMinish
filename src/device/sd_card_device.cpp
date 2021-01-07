
/*******************************************************************************
 * Implementation file of SdCard class for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "sd_card_device.hpp"

#include "../hardware/mcu.hpp"
#include "gpio_function.hpp"
#include "sd_card_exceptions.hpp"
#include "sd_card_specs.hpp"

using namespace std;
using namespace stminish::device;
using namespace sdcard;


/*******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION DECLARATIONS
 ******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 * CONSTRUCTORS & DESTRUCTOR
 ******************************************************************************/

/*******************************************************************************
 * OPERATOR IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE METHOD IMPLEMENTATIONS
 ******************************************************************************/

void SdCardDevice::execAppCmd(uint8_t cmd_idx,
                              ExpectedCmdResp resp_type,
                              uint32_t arg,
                              uint16_t rca,
                              bool ignore_crc)
{
    /* Before sending the app cmd, we need to send CMD55 to announce it */
    execCmd(cmd55, ExpectedCmdResp::Short,
            static_cast<uint32_t>(rca) << cmd55_rca_pos);
    uint32_t card_status = getShortResponse();
    /* Check that no error occured and that the card is now expecting our
     * app cmd */
    if (((card_status & status_err_msk) != 0)
        || ((card_status & status_app_cmd) == 0)) {
        disableDevice();
        throw SdCardCmdFailure{cmd55, 0};
    }
    return execCmd(cmd_idx, resp_type, arg, ignore_crc);
}


/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/

void SdCardDevice::powerUp(TimerDevice& timer)
{
    if (!isSdCardDetected()) {
        throw NoSdCardException{};
    }

    setupGpios();
    uint32_t sdmmc_ck_hz = enableDevice();
    /* Wait at least 74 card clocks for power up to be complete
     * (cf §6.4.1.1 of Physical Layer Simplified Specification Version 8.00) */
    timer.usleep((74 * 1000000) / sdmmc_ck_hz
                 + 100);  // add a small margin to that

    /* Card init procedure
     * cf figure 4-2 of Physical Layer Simplified Specification Version 2.00 */

    /* 1) Send GO_IDLE_STATE (CMD0), no response expected */
    execCmd(cmd0);
    /* 2) Send SEND_IF_COND (CMD8), R7 response */
    uint8_t check_pattern = 0b01010101;
    uint32_t arg          = (0b0001 << cmd8_supply_voltage_pos)
                   | (check_pattern << cmd8_check_pattern_pos);
    execCmd(cmd8, ExpectedCmdResp::Short, arg);
    /* Check that the card can operate with supplied voltage. If so, the command
     * response will be identical to the sent argument. */
    if (getShortResponse() != arg) {
        disableDevice();
        throw SdCardCmdFailure{cmd8, arg};
    }
    /* 3) Send SD_SEND_OP_COND (ACMD41) once without argument to query OCR. We
     * expect R3 response format which doesn't use CRC so we'll ignore the
     * error. */
    execAppCmd(acmd41, ExpectedCmdResp::Short, 0, 0, true);
    uint32_t r3_res = getShortResponse();
    uint16_t ocr    = (r3_res & r3_ocr) >> r3_ocr_pos;
    /* 4) Send SD_SEND_OP_COND again now that we know OCR, wait for card to be
     * ready */
    uint32_t acmd41_arg = acmd41_hcs | (ocr << acmd41_ocr_pos);
    do {
        execAppCmd(acmd41, ExpectedCmdResp::Short, acmd41_arg, 0, true);
        r3_res = getShortResponse();
    } while ((r3_res & r3_busy) == 0);  // busy is high when init is done
    /* 5) Send ALL_SEND_CID (cmd2) */
    execCmd(cmd2, ExpectedCmdResp::Long);
    cid = getLongResponse();
    /* The last bit of the CID is always 1 and is not in the response */
    cid[0] |= 1;
    /* 6) Send SEND_RELATIVE_ADDR (cmd3) */
    execCmd(cmd3, ExpectedCmdResp::Short);
    uint32_t r6_res = getShortResponse();
    if ((r6_res & r6_err_msk) != 0) {
        disableDevice();
        throw SdCardCmdFailure(cmd3, 0);
    }
    rca = (r6_res & r6_rca) >> r6_rca_pos;
    // TODO : switch to data transfer mode (§4.3)
}

void SdCardDevice::powerDown()
{
    // TODO, maybe we may need to stop any running operation
    disableDevice();
}

void SdCardDevice::setReadCallback(
    std::function<void(ErrorStatus&&, size_t)> callback)
{
    this->read_callback = callback;
}

void SdCardDevice::startRead(uint8_t* src, uint8_t* dest, size_t nb)
{
    // TODO
}

bool SdCardDevice::suspendRead()
{
    // TODO
}

void SdCardDevice::resumeRead()
{
    // TODO
}

void SdCardDevice::completeRead()
{
    // TODO
}

void SdCardDevice::setWriteCallback(
    std::function<void(ErrorStatus&&, size_t)> callback)
{
    this->write_callback = callback;
}


void SdCardDevice::startWrite(uint8_t* src, uint8_t* dest, size_t nb)
{
    // TODO
}

bool SdCardDevice::suspendWrite()
{
    // TODO
}

void SdCardDevice::resumeWrite()
{
    // TODO
}

void SdCardDevice::completeWrite()
{
    // TODO
}
