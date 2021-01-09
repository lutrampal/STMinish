
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
                              uint32_t cmd_arg,
                              uint16_t rca,
                              bool ignore_crc)
{
    /* Before sending the app cmd, we need to send CMD55 to announce it */
    execCmd(cmd55, ExpectedCmdResp::Short,
            static_cast<uint32_t>(rca) << cmd_rca_pos);
    uint32_t card_status = getShortResponse();
    /* Check that no error occured and that the card is now expecting our
     * app cmd */
    if (((card_status & status_err_msk) != 0)
        || ((card_status & status_app_cmd) == 0)) {
        disableDevice();
        throw SdCardCmdFailure{cmd55, 0};
    }
    return execCmd(cmd_idx, resp_type, cmd_arg, ignore_crc);
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
    uint32_t cmd_arg      = (0b0001 << cmd8_supply_voltage_pos)
                       | (check_pattern << cmd8_check_pattern_pos);
    execCmd(cmd8, ExpectedCmdResp::Short, cmd_arg);
    /* Check that the card can operate with supplied voltage. If so, the command
     * response will be identical to the sent argument. */
    if (getShortResponse() != cmd_arg) {
        disableDevice();
        throw SdCardCmdFailure{cmd8, cmd_arg};
    }
    /* 3) Send SD_SEND_OP_COND (ACMD41) once without argument to query OCR. We
     * expect R3 response format which doesn't use CRC so we'll ignore the
     * error. */
    execAppCmd(acmd41, ExpectedCmdResp::Short, 0, 0, true);
    uint32_t cmd_res = getShortResponse();  // R3
    uint16_t ocr     = (cmd_res & r3_ocr) >> r3_ocr_pos;
    /* 4) Send SD_SEND_OP_COND again now that we know OCR, wait for card to be
     * ready */
    cmd_arg = acmd41_hcs | (ocr << acmd41_ocr_pos);
    do {
        execAppCmd(acmd41, ExpectedCmdResp::Short, cmd_arg, 0, true);
        cmd_res = getShortResponse();    // R3
    } while ((cmd_res & r3_busy) == 0);  // busy is high when init is done
    if ((cmd_res & r3_ccs) == 0) {
        /* Only SDHC cards are currently supported */
        disableDevice();
        throw UnsupportedSdCardException{};
    }
    /* 5) Send ALL_SEND_CID (cmd2) */
    execCmd(cmd2, ExpectedCmdResp::Long);
    cid = getLongResponse();
    /* The last bit of the CID is always 1 and is not in the response */
    cid[0] |= 1;
    /* 6) Send SEND_RELATIVE_ADDR (cmd3) */
    execCmd(cmd3, ExpectedCmdResp::Short);
    cmd_res = getShortResponse();  // R6
    if ((cmd_res & r6_err_msk) != 0) {
        disableDevice();
        throw SdCardCmdFailure{cmd3, 0};
    }
    rca = (cmd_res & r6_rca) >> r6_rca_pos;
    /* We're out of card identification mode, we may increase the clock speed */
    // TODO: here we could enable high-speed mode
    setMaxClockSpeed();
    /* 7) Get Card Specific Data (CSD) register */
    cmd_arg = rca << cmd_rca_pos;
    execCmd(cmd9, ExpectedCmdResp::Long, cmd_arg);
    csd             = getLongResponse(); /* CSD register (r2 response) */
    uint32_t c_size = (csd[1] & r2_1_csize) >> r2_1_csize_pos;
    /* Cf def of C_SIZE p87 of spec v2.00 */
    dev_sz_kB = (c_size + 1) * 512;
    /* 8) select card and switch to data transfer mode (CMD7) */
    execCmd(cmd7, ExpectedCmdResp::Short, cmd_arg);
    if (getShortResponse() & status_err_msk) {
        disableDevice();
        throw SdCardCmdFailure{cmd7, cmd_arg};
    }
    /* 9) Set bus width to max bits (ACMD6) */
    cmd_arg = getMaxBusWidth() << acmd6_bus_width_pos;
    execAppCmd(acmd6, ExpectedCmdResp::Short, cmd_arg, rca);
    cmd_res = getShortResponse();
    if (cmd_res & status_err_msk) {
        disableDevice();
        throw SdCardCmdFailure{acmd6, cmd_arg};
    }
    if (static_cast<SdCardState>((cmd_res & status_current_state)
                                 >> status_current_state_pos)
        != SdCardState::Tran) {
        disableDevice();
        throw InvalidSdCardStateException{
            SdCardState::Tran,
            static_cast<SdCardState>((cmd_res & status_current_state)
                                     >> status_current_state_pos)};
    }

    setMaxControllerBusWidth();
}

void SdCardDevice::powerDown()
{
    // TODO, maybe we may need to stop any running operation
    disableDevice();
}

void SdCardDevice::setBlkReadCallback(
    std::function<void(ErrorStatus&&, size_t)> callback)
{
    this->blk_read_callback = callback;
}

void SdCardDevice::startBlkRead(uint32_t src_blk, uint8_t* dest, size_t nb_blk)
{
    // TODO
}

bool SdCardDevice::suspendBlkRead()
{
    // TODO
}

void SdCardDevice::resumeBlkRead()
{
    // TODO
}

void SdCardDevice::completeBlkRead()
{
    // TODO
}

void SdCardDevice::setBlkWriteCallback(
    std::function<void(ErrorStatus&&, size_t)> callback)
{
    this->blk_write_callback = callback;
}


void SdCardDevice::startBlkWrite(uint8_t* src, uint32_t dest_blk, size_t nb_blk)
{
    // TODO
}

bool SdCardDevice::suspendBlkWrite()
{
    // TODO
}

void SdCardDevice::resumeBlkWrite()
{
    // TODO
}

void SdCardDevice::completeBlkWrite()
{
    // TODO
}
