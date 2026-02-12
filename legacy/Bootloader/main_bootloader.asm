[BITS 16]          ; 16bit real mode
[ORG 0x7C00]       ; Bootloader entry point

;=======================================================
;           BOOTLOADER - Lowkirkeinly loads the OS
;=======================================================

KERNEL_OFFSET equ 0x1000    ; kernel offset

start:
    ; Set up segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Stack grows downward from bootloader

    ; Save boot drive number (BIOS passes it in DL)
    mov [BOOT_DRIVE], dl

    ; Print bootloader start message
    mov si, msg_boot_start
    call print_string

    ; Load kernel from disk
    call load_kernel

    ; Print success message
    mov si, msg_kernel_loaded
    call print_string

    ; Switch to 32-bit protected mode
    call switch_to_pm

    jmp $                   ; Should never reach here

;=======================================================
;                  DISK LOADING
;=======================================================

load_kernel:
    mov si, msg_loading
    call print_string

    mov bx, KERNEL_OFFSET   ; ES:BX = destination address (0x0000:0x1000)
    mov dh, 10             ; Number of sectors to read (5KB - just kernel, no margin)
    mov dl, [BOOT_DRIVE]    ; Drive number
    call disk_load
    ret

disk_load:
    pusha
    push dx                 ; Save DX (DH = number of sectors to read)

    mov ah, 0x02            ; BIOS read sector function
    mov al, dh              ; Number of sectors to read
    mov cl, 0x02            ; Start from sector 2 (sector 1 is bootloader)
    mov ch, 0x00            ; Cylinder 0
    mov dh, 0x00            ; Head 0

    int 0x13                ; BIOS disk interrupt
    jc disk_error           ; Jump if carry flag set (error)

    pop dx                  ; Restore DX
    cmp al, dh              ; Check if we read expected number of sectors
    jne sectors_error
    popa
    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp disk_loop

sectors_error:
    mov si, msg_sectors_error
    call print_string

disk_loop:
    jmp $                   ; Infinite loop on error

;=======================================================
;                  PRINT STRING
;=======================================================

print_string:
    pusha
.loop:
    lodsb                   ; Load byte at [SI] into AL, increment SI
    cmp al, 0
    je .done
    mov ah, 0x0E            ; BIOS teletype function
    mov bh, 0x00            ; Page number
    mov bl, 0x07            ; Text attribute (white on black)
    int 0x10                ; BIOS video interrupt
    jmp .loop
.done:
    popa
    ret

;=======================================================
;              SWITCH TO PROTECTED MODE
;=======================================================

switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT descriptor

    ; Enable protected mode by setting bit 0 of CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Far jump to flush pipeline and load CS with 32-bit code segment selector
    jmp CODE_SEG:init_pm

;=======================================================
;                   GDT (Global Descriptor Table)
;=======================================================

gdt_start:
    ; Null descriptor (required)
    dd 0x0
    dd 0x0

gdt_code:
    ; Code segment descriptor
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10011010b            ; Access byte: present, ring 0, code segment, executable, readable
    db 11001111b            ; Flags + Limit (bits 16-19): 4KB granularity, 32-bit
    db 0x0                  ; Base (bits 24-31)

gdt_data:
    ; Data segment descriptor
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10010010b            ; Access byte: present, ring 0, data segment, writable
    db 11001111b            ; Flags + Limit (bits 16-19): 4KB granularity, 32-bit
    db 0x0                  ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size (limit)
    dd gdt_start                ; GDT address

; Segment selector constants
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

;=======================================================
;              32-BIT PROTECTED MODE CODE
;=======================================================

[BITS 32]

init_pm:
    ; Update segment registers for protected mode
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack in protected mode (grows downward from 0x90000)
    mov esp, 0x90000

    ; Far jump to kernel entry point at 0x1000
    jmp CODE_SEG:0x1000

    ; Hang if kernel returns (should never reach here)
    cli
    hlt
    jmp $


;=======================================================
;                     DATA
;=======================================================

[BITS 16]

BOOT_DRIVE:         db 0
msg_boot_start:     db "[BOOT_LOADER] [INFO] Bootloader started...", 13, 10, 0
msg_loading:        db "[BOOT_LOADER] [INFO] Loading kernel from Floppy...", 13, 10, 0
msg_kernel_loaded:  db "[BOOT_LOADER] [INFO] Begging To Hand off to kernel...", 13, 10, 0

msg_disk_error:     db "[BOOT_LOADER] [ERROR] Disk read error!", 13, 10, 0
msg_sectors_error:  db "[BOOT_LOADER] [ERROR] Sector count mismatch!", 13, 10, 0



times 510-($-$$) db 0 ;fill empty with 0 so the fucking bootloader is happy
dw 0xAA55       ; offset