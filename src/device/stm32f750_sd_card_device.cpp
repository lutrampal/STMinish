
/*******************************************************************************
 * Implementation file of SdCard class for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "stm32f750_sd_card_device.hpp"

#include "../hardware/mcu.hpp"
#include "gpio_function.hpp"
#include "sd_card_exceptions.hpp"
#include "sd_card_specs.hpp"
#include "timer_device.hpp"

using namespace std;
using namespace stminish::device;
using namespace sdcard;


/*******************************************************************************
 * PRIVATE TYPE DEFINITIONS
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

/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

bool Stm32f750SdCardDevice::isSdCardDetected()
{
    gpioFunctionConfigure(USD_DETECT_GPIO_Port, USD_DETECT_Pin, SelFunc::Input,
                          PinSpeed::VeryHigh, PullMode::PullUp);
    return (USD_DETECT_GPIO_Port->IDR & (1 << USD_DETECT_Pin)) == 0;
}

void Stm32f750SdCardDevice::setupGpios()
{
    gpioFunctionConfigure(SDMMC_CK_GPIO_Port, SDMMC_CK_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
    gpioFunctionConfigure(SDMMC_CMD_GPIO_Port, SDMMC_CMD_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
    gpioFunctionConfigure(SDMMC_D0_GPIO_Port, SDMMC_D0_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
    gpioFunctionConfigure(SDMMC_D1_GPIO_Port, SDMMC_D1_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
    gpioFunctionConfigure(SDMMC_D2_GPIO_Port, SDMMC_D2_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
    gpioFunctionConfigure(SDMMC_D3_GPIO_Port, SDMMC_D3_Pin, SelFunc::Alt12,
                          PinSpeed::VeryHigh, PullMode::NoPull);
}

void Stm32f750SdCardDevice::execCmd(uint8_t cmd_idx,
                                    ExpectedCmdResp resp_type,
                                    uint32_t arg,
                                    bool ignore_crc)
{
    SDMMC1->ARG = arg;
    SDMMC1->CMD =
        SDMMC_CMD_CPSMEN | (cmd_idx << SDMMC_CMD_CMDINDEX_Pos)
        | (static_cast<unsigned>(resp_type) << SDMMC_CMD_WAITRESP_Pos);

    switch (resp_type) {
        case ExpectedCmdResp::None:
            while (((SDMMC1->STA & SDMMC_STA_CMDSENT) == 0)
                   || ((SDMMC1->STA & SDMMC_STA_CMDACT) != 0U)) {}
            SDMMC1->CMD &= ~SDMMC_CMD_CPSMEN;
            break;
        default:
            while (((SDMMC1->STA
                     & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND
                        | SDMMC_STA_CTIMEOUT))
                    == 0U)
                   || ((SDMMC1->STA & SDMMC_STA_CMDACT) != 0U)) {}
            SDMMC1->CMD &= ~SDMMC_CMD_CPSMEN;
            if (SDMMC1->STA & SDMMC_STA_CTIMEOUT) {
                SDMMC1->ICR = 0xFFFFFFFF;  // clear all flags
                disableDevice();
                throw SdCardCmdTimeout{cmd_idx, arg};
            }
            if (SDMMC1->STA & SDMMC_STA_CCRCFAIL) {
                SDMMC1->ICR = 0xFFFFFFFF;  // clear all flags
                if (!ignore_crc) {
                    disableDevice();
                    throw SdCardCmdCrcCheckFailure{cmd_idx, arg};
                }
            }
    }

    SDMMC1->ICR |= SDMMC_ICR_CMDSENTC | SDMMC_ICR_CMDRENDC;
}

uint32_t Stm32f750SdCardDevice::getShortResponse()
{
    return SDMMC1->RESP1;
}

array<uint32_t, 4> Stm32f750SdCardDevice::getLongResponse()
{
    return array<uint32_t, 4>{SDMMC1->RESP4, SDMMC1->RESP3, SDMMC1->RESP2,
                              SDMMC1->RESP1};
}

/* Return card clock to setup delay for power up sequence */
uint32_t Stm32f750SdCardDevice::enableDevice()
{
    RCC->APB2ENR |= RCC_APB2ENR_SDMMC1EN;
    RCC->APB2RSTR |= RCC_APB2RSTR_SDMMC1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SDMMC1RST;
    /* PLLQ factor is set during boot in order to get a 48 MHz clock */
    RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_SDMMC1SEL;

    /* SDMMC_CK must be less than 400kHz while in id phase */
    uint32_t clkdiv      = 255;
    uint32_t sdmmc_ck_hz = 48000000 / (clkdiv + 2);
    SDMMC1->CLKCR =
        SDMMC_CLKCR_CLKEN | (clkdiv << SDMMC_CLKCR_CLKDIV_Pos)
        | SDMMC_CLKCR_HWFC_EN;  // TODO: check behavior of HW flow control
    /* Power on the card */
    SDMMC1->POWER |= 0b11 << SDMMC_POWER_PWRCTRL_Pos;

    return sdmmc_ck_hz;
}

void Stm32f750SdCardDevice::disableDevice()
{
    SDMMC1->CLKCR &= ~SDMMC_CLKCR_CLKEN;
    SDMMC1->POWER &= ~SDMMC_POWER_PWRCTRL;
    RCC->APB2ENR &= ~RCC_APB2ENR_SDMMC1EN;
}

void Stm32f750SdCardDevice::setMaxClockSpeed()
{
    /* Disable clock in order to change settings */
    SDMMC1->CLKCR &= ~SDMMC_CLKCR_CLKEN;
    /* Set clock to 24 MHz */
    SDMMC1->CLKCR &= ~SDMMC_CLKCR_CLKDIV;
    SDMMC1->CLKCR |= SDMMC_CLKCR_CLKEN;
}

uint8_t Stm32f750SdCardDevice::getMaxBusWidth()
{
    return max_bus_width;
}

void Stm32f750SdCardDevice::setMaxControllerBusWidth()
{
    SDMMC1->CLKCR &= ~SDMMC_CLKCR_WIDBUS;
    SDMMC1->CLKCR |= 0b01 << SDMMC_CLKCR_WIDBUS_Pos;
}


/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/
