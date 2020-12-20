
/*******************************************************************************
 * Implementation file of GPIO config functions for STM32F750 MCU
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "../hardware/mcu.hpp"
#include "gpio_function.hpp"

using namespace stminish;
using namespace device;

#ifdef MCU_STM32F750
/*******************************************************************************
 * STATIC CONSTANT DEFINITIONS
 ******************************************************************************/

constexpr unsigned nb_pins_per_bank = 16;


/*******************************************************************************
 * STATIC FUNCTION DECLARATIONS
 ******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

/*******************************************************************************
 * EXTERN FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

void GpioFunctionConfigure(Bank bank, PinIndex pin, SelFunc sel, PinSpeed spd)
{
    if (pin > nb_pins_per_bank) {
        throw GpioFuncConfigFailure{bank, pin};
    }

    unsigned bank_idx = static_cast<unsigned>(bank);
    GPIO_TypeDef* hw_bank =
        reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE + bank_idx * 0x0400UL);

    RCC->AHB1ENR |= 0x1UL << (RCC_AHB1ENR_GPIOAEN_Pos + bank_idx);

    hw_bank->OSPEEDR &= ~(0b11 << (pin * 2));
    hw_bank->OSPEEDR |= (static_cast<unsigned>(spd) << (pin * 2));

    switch (sel) {
        case SelFunc::Input:
            hw_bank->MODER &= ~(0b11 << (pin * 2));
            hw_bank->MODER |= (0b00 << (pin * 2));
            break;

        case SelFunc::Output:
            hw_bank->MODER &= ~(0b11 << (pin * 2));
            hw_bank->MODER |= (0b01 << (pin * 2));
            break;

        default:
            hw_bank->MODER &= ~(0b11 << (pin * 2));
            hw_bank->MODER |= (0b10 << (pin * 2));

            if (pin <= 7) {
                hw_bank->AFR[0] &= ~(0b1111 << (pin * 4));
                hw_bank->AFR[0] |= (static_cast<unsigned>(sel) << (pin * 4));
            } else {
                hw_bank->AFR[1] &= ~(0b1111 << ((pin - 8) * 4));
                hw_bank->AFR[1] |=
                    (static_cast<unsigned>(sel) << ((pin - 8) * 4));
            }
            break;
    }
}


#endif
