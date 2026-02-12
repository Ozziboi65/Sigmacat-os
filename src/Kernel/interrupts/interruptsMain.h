#ifndef INTERRUPTS_MAIN_H
#define INTERRUPTS_MAIN_H

#include <stdint.h>

// IDT Entry structure
struct IDT_Entry {
    uint16_t offset_low;     // Lower 16 bits of function address
    uint16_t selector;       // Kernel segment selector
    uint8_t  zero;          // Always zero
    uint8_t  type_attr;     // Type and attributes (P DPL S Type)
    uint16_t offset_high;   // Higher 16 bits of function address
} __attribute__((packed));

// Registers pushed by interrupt
struct Registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // GPRs pushed by pusha
    uint32_t int_no, err_code;                         // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;            // Pushed by CPU automatically
} __attribute__((packed));

// IDT Pointer structure
struct IDT_Ptr {
    uint16_t limit;         // Size of IDT
    uint32_t base;          // Base address of IDT
} __attribute__((packed));

// Function pointer type for interrupt handlers
typedef void (*InterruptHandlerFunc)(Registers* regs);

// Interrupt handler entry
struct InterruptHandler {
    InterruptHandlerFunc handler;   // Function to call
    bool active;                   // Is this handler active
};

// Main interrupt manager
struct InterruptManager {
    IDT_Entry idt[256];               // Interrupt Descriptor Table
    InterruptHandler handlers[256];   // Handler functions
    IDT_Ptr idt_ptr;                 // IDT pointer for LIDT
    bool interrupts_enabled;          // Are interrupts enabled
};

// Exception names
extern const char* exception_messages[32];

// Assembly ISR stubs (defined in assembly)
extern "C" {
    // Exception handlers (0-31)
    void isr0();  void isr1();  void isr2();  void isr3();
    void isr4();  void isr5();  void isr6();  void isr7();
    void isr8();  void isr9();  void isr10(); void isr11();
    void isr12(); void isr13(); void isr14(); void isr15();
    void isr16(); void isr17(); void isr18(); void isr19();
    void isr20(); void isr21(); void isr22(); void isr23();
    void isr24(); void isr25(); void isr26(); void isr27();
    void isr28(); void isr29(); void isr30(); void isr31();
    
    // IRQ handlers (32-47)
    void irq0();  void irq1();  void irq2();  void irq3();
    void irq4();  void irq5();  void irq6();  void irq7();
    void irq8();  void irq9();  void irq10(); void irq11();
    void irq12(); void irq13(); void irq14(); void irq15();
    
    // Common ISR handler
    void isr_handler(Registers* regs);
    void irq_handler(Registers* regs);
}

// Core interrupt functions
void interrupt_init();
void set_idt_gate(uint8_t num, uint32_t handler);
void install_handler(uint8_t interrupt, InterruptHandlerFunc handler);
void remove_handler(uint8_t interrupt);
void enable_interrupts();
void disable_interrupts();
bool are_interrupts_enabled();

// PIC (Programmable Interrupt Controller) functions
void pic_remap(int offset1, int offset2);
void pic_send_eoi(uint8_t irq);
void irq_set_mask(uint8_t irq_line);
void irq_clear_mask(uint8_t irq_line);


void Visual_exception_handler(Registers* regs);
void default_irq_handler(Registers* regs);

// Get interrupt manager instance
InterruptManager* get_interrupt_manager();





#endif // INTERRUPTS_MAIN_H