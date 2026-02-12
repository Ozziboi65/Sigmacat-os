/* Linker script for GRUB/Multiboot kernel */
/* Places Multiboot header at beginning for GRUB detection */

OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS
{
    /* GRUB loads kernel at 1MB (0x00100000) in protected mode */
    . = 0x00100000;

    /* Multiboot header MUST be in first 8KB */
    .multiboot ALIGN(4K) : {
        *(.multiboot)
    }

    /* Code section */
    .text ALIGN(4K) : {
        *(.text)
        *(.text.*)
    }

    /* Read-only data (constants, strings) */
    .rodata ALIGN(4K) : {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data */
    .data ALIGN(4K) : {
        *(.data)
        *(.data.*)
    }

    /* Uninitialized data (zeroed by GRUB) */
    .bss ALIGN(4K) : {
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
