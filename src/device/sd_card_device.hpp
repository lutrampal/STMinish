
/*******************************************************************************
 * Base class interface file for SD Card devices.
 * The SD specification standardizes the initialization and utilization of SD
 * cards, thus the high level algorithms for read/write etc.. can be
 * implemented in this base class. Subclasses will have to implement low level
 * operations to allow the base class to run the commands.
 ******************************************************************************/

#ifndef _STMINISH_DEVICE_SD_CARD_DEVICE_HPP
#define _STMINISH_DEVICE_SD_CARD_DEVICE_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "error_status.hpp"
#include "timer_device.hpp"

#include <array>
#include <functional>

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITIONS
 ******************************************************************************/

class SdCardDevice
{
  public:
    void powerUp(TimerDevice& timer);
    void powerDown();
    void
        setBlkReadCallback(std::function<void(ErrorStatus&&, size_t)> callback);
    void startBlkRead(uint32_t src_blk, uint8_t* dest, size_t nb_blk);
    bool suspendBlkRead();
    bool cancelBlkRead();
    void resumeBlkRead();
    void completeBlkRead();

    void setBlkWriteCallback(
        std::function<void(ErrorStatus&&, size_t)> callback);
    void startBlkWrite(uint8_t* src, uint32_t dest_blk, size_t nb_blk);
    bool suspendBlkWrite();
    bool cancelBlkWrite();
    void resumeBlkWrite();
    void completeBlkWrite();

  protected:
    typedef std::array<uint32_t, 4> LongResponse;
    enum class ExpectedCmdResp { Short = 1, None, Long };
    virtual bool isSdCardDetected()               = 0;
    virtual void setupGpios()                     = 0;
    virtual void execCmd(uint8_t cmd_idx,
                         ExpectedCmdResp resp_type = ExpectedCmdResp::None,
                         uint32_t arg              = 0,
                         bool ignore_crc           = false) = 0;
    virtual uint32_t getShortResponse()           = 0;
    virtual LongResponse getLongResponse()        = 0;
    /* Return card clock to setup delay for power up sequence, needs a timer
     * because the specs says we have to wait some time for power up */
    virtual uint32_t enableDevice()         = 0;
    virtual void disableDevice()            = 0;
    virtual void setMaxClockSpeed()         = 0;
    virtual void setMaxControllerBusWidth() = 0;
    virtual uint8_t getMaxBusWidth()        = 0;

  private:
    LongResponse cid;
    LongResponse csd;
    uint16_t rca;
    unsigned dev_sz_kB;


    void execAppCmd(uint8_t cmd_idx,
                    ExpectedCmdResp resp_type = ExpectedCmdResp::None,
                    uint32_t arg              = 0,
                    uint16_t rca              = 0,
                    bool ignore_crc           = false);

    std::function<void(ErrorStatus&&, size_t)> blk_read_callback;
    std::function<void(ErrorStatus&&, size_t)> blk_write_callback;
};

}  // namespace device
}  // namespace stminish

#endif
