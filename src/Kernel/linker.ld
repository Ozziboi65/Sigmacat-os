/* Linker script for the kernel */
/* Ensures all sections (.text, .rodata, .data, .bss) are included in the binary */

OUTPUT_FORMAT(binary)
ENTRY(_start)

SECTIONS
{
    /* Kernel is loaded at 0x1000 by the bootloader */
    . = 0x1000;

    /* Code section */
    .text : {
        *(.text)
        *(.text.*)
    }

    /* Read-only data (constants, strings) */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data */
    .data : {
        *(.data)
        *(.data.*)
    }

    /* Uninitialized data (zeroed) */
    .bss : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }

    /* Discard unnecessary sections */
    /DISCARD/ : {
        *(.comment)
        *(.note.*)
        *(.eh_frame)
    }
}
