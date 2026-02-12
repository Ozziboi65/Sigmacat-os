[BITS 32]

; External C functions
extern isr_handler
extern irq_handler

; Common ISR stub - saves state and calls C handler
isr_common_stub:
    pusha               ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax
    
    mov ax, 0x10        ; Kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp            ; Push pointer to Registers struct as argument
    call isr_handler    ; Call our C handler
    add esp, 4          ; Clean up pushed argument
    
    popa                ; Restore all general purpose registers
    add esp, 8          ; Clean up the pushed error code and ISR number
    iret                ; Return from interrupt (restores EFLAGS including IF)

; Common IRQ stub - saves state and calls C handler
irq_common_stub:
    pusha               ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax
    
    mov ax, 0x10        ; Kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp            ; Push pointer to Registers struct as argument
    call irq_handler    ; Call our C handler
    add esp, 4          ; Clean up pushed argument
    
    popa                ; Restore all general purpose registers
    add esp, 8          ; Clean up the pushed error code and IRQ number
    iret                ; Return from interrupt (restores EFLAGS including IF)

; Macro to create ISR stub without error code
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli
        push byte 0       ; Push dummy error code
        push byte %1      ; Push interrupt number
        jmp isr_common_stub
%endmacro

; Macro to create ISR stub with error code
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli
        push byte %1      ; Push interrupt number
        jmp isr_common_stub
%endmacro

; Macro to create IRQ stub
%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0       ; Push dummy error code
        push byte %2      ; Push IRQ number (32 + IRQ)
        jmp irq_common_stub
%endmacro

; CPU Exception handlers (0-31)
ISR_NOERRCODE 0     ; Divide by zero
ISR_NOERRCODE 1     ; Debug
ISR_NOERRCODE 2     ; Non-maskable interrupt
ISR_NOERRCODE 3     ; Breakpoint
ISR_NOERRCODE 4     ; Overflow
ISR_NOERRCODE 5     ; Bound range exceeded
ISR_NOERRCODE 6     ; Invalid opcode
ISR_NOERRCODE 7     ; Device not available
ISR_ERRCODE   8     ; Double fault (has error code)
ISR_NOERRCODE 9     ; Coprocessor segment overrun
ISR_ERRCODE   10    ; Invalid TSS (has error code)
ISR_ERRCODE   11    ; Segment not present (has error code)
ISR_ERRCODE   12    ; Stack segment fault (has error code)
ISR_ERRCODE   13    ; General protection fault (has error code)
ISR_ERRCODE   14    ; Page fault (has error code)
ISR_NOERRCODE 15    ; Reserved
ISR_NOERRCODE 16    ; x87 floating point exception
ISR_ERRCODE   17    ; Alignment check (has error code)
ISR_NOERRCODE 18    ; Machine check
ISR_NOERRCODE 19    ; SIMD floating point exception
ISR_NOERRCODE 20    ; Virtualization exception
ISR_NOERRCODE 21    ; Reserved
ISR_NOERRCODE 22    ; Reserved
ISR_NOERRCODE 23    ; Reserved
ISR_NOERRCODE 24    ; Reserved
ISR_NOERRCODE 25    ; Reserved
ISR_NOERRCODE 26    ; Reserved
ISR_NOERRCODE 27    ; Reserved
ISR_NOERRCODE 28    ; Reserved
ISR_NOERRCODE 29    ; Reserved
ISR_ERRCODE   30    ; Security exception (has error code)
ISR_NOERRCODE 31    ; Reserved

; Hardware interrupt requests (IRQ 0-15, mapped to interrupts 32-47)
IRQ 0,  32    ; Timer
IRQ 1,  33    ; Keyboard
IRQ 2,  34    ; Cascade (used internally by PICs)
IRQ 3,  35    ; COM2
IRQ 4,  36    ; COM1
IRQ 5,  37    ; LPT2
IRQ 6,  38    ; Floppy disk
IRQ 7,  39    ; LPT1
IRQ 8,  40    ; CMOS real-time clock
IRQ 9,  41    ; Free for peripherals / legacy SCSI / NIC
IRQ 10, 42    ; Free for peripherals / SCSI / NIC
IRQ 11, 43    ; Free for peripherals / SCSI / NIC
IRQ 12, 44    ; PS2 mouse
IRQ 13, 45    ; FPU / coprocessor / inter-processor
IRQ 14, 46    ; Primary ATA channel
IRQ 15, 47    ; Secondary ATA channel