; Multiboot-compliant kernel entry for GRUB bootloader
; Multiboot Specification: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

[BITS 32]

; Multiboot header constants
MULTIBOOT_MAGIC         equ 0x1BADB002          ; Magic number for Multiboot
MULTIBOOT_ALIGN         equ 1 << 0              ; Align loaded modules on page boundaries
MULTIBOOT_MEMINFO       equ 1 << 1              ; Provide memory map
MULTIBOOT_VIDEO         equ 1 << 2              ; Request video mode info
MULTIBOOT_FLAGS         equ MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO | MULTIBOOT_VIDEO
MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

extern _kernel_main             ; MinGW 32-bit emits leading underscore

global _start
global kernel_entry
global _kernel_entry

; Multiboot header - MUST be in first 8KB of kernel
section .multiboot
align 4
multiboot_header:
    dd MULTIBOOT_MAGIC          ; Magic number
    dd MULTIBOOT_FLAGS          ; Flags
    dd MULTIBOOT_CHECKSUM       ; Checksum (magic + flags + checksum = 0)
    ; Address fields (unused when not using MULTIBOOT_AOUT_KLUDGE)
    dd 0                        ; header_addr
    dd 0                        ; load_addr
    dd 0                        ; load_end_addr
    dd 0                        ; bss_end_addr
    dd 0                        ; entry_addr
    ; Video mode fields (required when MULTIBOOT_VIDEO is set)
    dd 0                        ; mode_type: 0 = linear graphics mode
    dd 1024                     ; width: preferred width
    dd 768                      ; height: preferred height
    dd 32                       ; depth: preferred bits per pixel

; BSS section for stack
section .bss
align 16
stack_bottom:
    resb 16384                  ; 16KB stack
stack_top:

; GDT - Global Descriptor Table
section .data
align 16
gdt_start:
    ; Null descriptor (required, index 0)
    dd 0x00000000
    dd 0x00000000

    ; Code segment descriptor (selector 0x08)
    ; Base=0, Limit=0xFFFFF, 4K granularity, 32-bit, ring 0
    dw 0xFFFF       ; Limit low (bits 0-15)
    dw 0x0000       ; Base low (bits 0-15)
    db 0x00         ; Base mid (bits 16-23)
    db 10011010b    ; Access: Present, Ring 0, Code, Exec/Read
    db 11001111b    ; Flags: 4K gran, 32-bit + Limit high (bits 16-19)
    db 0x00         ; Base high (bits 24-31)

    ; Data segment descriptor (selector 0x10)
    ; Base=0, Limit=0xFFFFF, 4K granularity, 32-bit, ring 0
    dw 0xFFFF       ; Limit low
    dw 0x0000       ; Base low
    db 0x00         ; Base mid
    db 10010010b    ; Access: Present, Ring 0, Data, Read/Write
    db 11001111b    ; Flags: 4K gran, 32-bit + Limit high
    db 0x00         ; Base high
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; GDT size (limit)
    dd gdt_start                  ; GDT base address

section .text

_start:
kernel_entry:
_kernel_entry:
    ; GRUB puts us in 32-bit protected mode with:
    ; - EAX = 0x2BADB002 (Multiboot magic)
    ; - EBX = physical address of Multiboot info structure

    ; Save multiboot params before we clobber registers
    mov ecx, eax
    mov edx, ebx

    ; Load our own GDT (GRUB's may become invalid)
    lgdt [gdt_descriptor]

    ; Reload code segment with a far jump
    jmp 0x08:.reload_segments

.reload_segments:
    ; Reload data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Set up stack
    mov esp, stack_top
    mov ebp, esp
    
    ; Push Multiboot parameters for kernel_main
    push edx                    ; Multiboot info structure pointer
    push ecx                    ; Multiboot magic value
    
    ; Call the C kernel main function
    call _kernel_main
    
    ; If kernel returns, hang
    cli
.hang:
    hlt
    jmp .hang

