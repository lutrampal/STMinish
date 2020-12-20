
SECTIONS 
{
    .boot_code :
    {
        . = ALIGN(4);    
        KEEP (*(.reset))    
        *(.boot_code)
        *(.boot_code*)
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        KEEP (*(.init))
        KEEP (*(.fini))
        KEEP (*(.eh_frame))
        . = ALIGN(4);
    } >QSPI

    /* Sections required by the standard libraries. */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM :
    {
        *(.ARM.exidx*)
    } >FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

      _siitcm = .;
    .itcm : AT(_siitcm)
    {
        . = ALIGN(4);
        _sitcm = .;
        KEEP (*(.itcm_irqs))
        . = ALIGN(4);
        _eitcm = .;
    } >ITCMRAM


    /* Space for static writable, initialized to non 0 data */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata =  .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >SRAM

    /* Static, writable, initialized to 0 data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss);
        *(.bss*);
        *(COMMON);
        . = ALIGN(4);
        _ebss = .;
    } >SRAM

    /* DTCM RAM region.
     * Tightly-coupled memory on the data bus. */
    _sidtcm = .;
    .dtcm : AT(_sidtcm)
    {
        . = ALIGN(4);
        _sdtcm = .;
        KEEP (*(.dtcm_vtable))
        . = ALIGN(4);
        _edtcm = .;
    } >DTCMRAM_V
} 
