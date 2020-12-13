
/*******************************************************************************
 * This code is responsible for:
 * - Copying mutable data from flash to SRAM
 * - Initializing vector table and copying handler code to ITCM
 * - Initializing SDRAM
 * - Initializing QSPI
 * - Handing over to main()
 ******************************************************************************/

#include "irqs.hpp"

#include <cstdint>
#include <cstring>


#define BOOT_CODE_ATTR __attribute__((section(".boot_text")))


extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _sidtcm, _sdtcm, _edtcm,
    _siitcm, _sitcm, _eitcm, _estack;


extern "C"
{
    void __attribute__((naked)) __attribute__((section(".reset")))
    reset_handler(void);
}


/*******************************************************************************
 * STATIC CONSTANT DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION DECLARATIONS
 ******************************************************************************/

/** Copy program data from flash to ram, zero BSS. */
static void BOOT_CODE_ATTR m_init_data(void);
/** Relocate vector table, assign used IRQs and set others to error handler */
static void BOOT_CODE_ATTR m_init_vtable(void);

/*******************************************************************************
 * STATIC FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

static void m_init_data(void)
{
    size_t data_size =
        reinterpret_cast<size_t>(&_edata) - reinterpret_cast<size_t>(&_sdata);
    for (size_t i = 0; i < data_size; ++i)
    {
        reinterpret_cast<uint8_t*>(&_sdata)[i] =
            reinterpret_cast<uint8_t*>(&_sidata)[i];
    }

    size_t dtcm_size =
        reinterpret_cast<size_t>(&_edtcm) - reinterpret_cast<size_t>(&_sdtcm);
    for (size_t i = 0; i < dtcm_size; ++i)
    {
        reinterpret_cast<uint8_t*>(&_sdtcm)[i] =
            reinterpret_cast<uint8_t*>(&_sidtcm)[i];
    }

    size_t itcm_size =
        reinterpret_cast<size_t>(&_eitcm) - reinterpret_cast<size_t>(&_sitcm);
    for (size_t i = 0; i < itcm_size; ++i)
    {
        reinterpret_cast<uint8_t*>(&_sitcm)[i] =
            reinterpret_cast<uint8_t*>(&_siitcm)[i];
    }

    size_t bss_size =
        reinterpret_cast<size_t>(&_ebss) - reinterpret_cast<size_t>(&_sbss);
    for (size_t i = 0; i < bss_size; ++i)
    {
        reinterpret_cast<uint8_t*>(&_sbss)[i] = 0;
    }
}

static void m_init_vtable(void)
{
    SCB->VTOR = (uint32_t)&g_vtable;

    g_vtable[0] = reinterpret_cast<irq_handler_t>(_estack);
    g_vtable[1] = reset_handler;

    for (int i = 2; i < NB_IRQS; ++i) { g_vtable[i] = error_handler; }
}


/*******************************************************************************
 * EXTERN FUNCTION IMPLEMENTATIONS
 ******************************************************************************/

void reset_handler(void)
{
    __asm__("LDR r0, =_estack\n\t"
            "MOV sp, r0\n\t");

    m_init_data();
    m_init_vtable();
    // TODO: CPU speed, SDRAM, QSPI

    __asm__("B main");
}
