
/*******************************************************************************
 * Implementation file of SdCard class for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../hardware/mcu.hpp"
#include "stm32f750_rng_device.hpp"
#include "stm32f750_sd_card_device.hpp"
#include "stm32f750_timer_device.hpp"
#include "system.hpp"

using namespace std;
using namespace stminish::device;


#ifdef MCU_STM32F750
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

System::System()
: rng{new Stm32f750RngDevice}, timer{new Stm32f750TimerDevice},
  sd_card{new Stm32f750SdCardDevice{}}
{
    sd_card->powerUp(*timer);
}

System::~System()
{
    sd_card->powerDown();
}

/*******************************************************************************
 * OPERATOR IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PROTECTED METHOD IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC METHOD IMPLEMENTATIONS
 ******************************************************************************/

System& System::getInstance()
{
    static System s;

    return s;
}

RngDevice& System::getRng()
{
    return *rng;
}

TimerDevice& System::getTimer()
{
    return *timer;
}

SdCardDevice& System::getSdCard()
{
    return *sd_card;
}

#endif
