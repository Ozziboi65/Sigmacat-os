#include "interruptsMain.h"
#include "../Kernel_Services.h"
#include "../../Os/Services/Graphics/graphics.h"
#include "../IO/PortIO.h"

// Global interrupt manager instance
static InterruptManager interrupt_mgr;

// error mesages
const char* exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// Get interrupt manager instance
InterruptManager* get_interrupt_manager() {
    return &interrupt_mgr;
}

// Set an IDT gate
void set_idt_gate(uint8_t num, uint32_t handler) {
    interrupt_mgr.idt[num].offset_low = handler & 0xFFFF;
    interrupt_mgr.idt[num].selector = 0x08;  // Code segment selector
    interrupt_mgr.idt[num].zero = 0;
    interrupt_mgr.idt[num].type_attr = 0x8E;  // Present, DPL=0, 32-bit interrupt gate
    interrupt_mgr.idt[num].offset_high = (handler >> 16) & 0xFFFF;
}

// Remap PIC to avoid conflicts with exceptions
void pic_remap(int offset1, int offset2) {
    // Initialize PIC1
    outb(0x20, 0x11);  // ICW1: Init + ICW4 needed
    io_wait();
    outb(0xA0, 0x11);  // ICW1: Init + ICW4 needed
    io_wait();
    
    outb(0x21, offset1); // ICW2: Master PIC vector offset
    io_wait();
    outb(0xA1, offset2); // ICW2: Slave PIC vector offset
    io_wait();
    
    outb(0x21, 0x04);  // ICW3: Master has slave on IRQ2
    io_wait();
    outb(0xA1, 0x02);  // ICW3: Slave cascade identity
    io_wait();
    
    outb(0x21, 0x01);  // ICW4: 8086 mode
    io_wait();
    outb(0xA1, 0x01);  // ICW4: 8086 mode
    io_wait();
    
    // Mask all IRQs initially - unmask only what we need later
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

// Send End of Interrupt signal to PIC
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);  // Send EOI to slave PIC
    }
    outb(0x20, 0x20);      // Send EOI to master PIC
}

// Set IRQ mask (disable IRQ)
void irq_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
    
    if (irq_line < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

// Clear IRQ mask (enable IRQ)
void irq_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
    
    if (irq_line < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}


void Visual_exception_handler(Registers* regs) {
    asm volatile("cli");
    
    char buf[128];
    

    FillScreenAndClear(0,0,0);
    CurrentLine = 0;


    PrintLn("         333333333333333   ",255,255,255);
    PrintLn("        3:::::::::::::::33 ",255,255,255);
    PrintLn("        3::::::33333::::::3",255,255,255);
    PrintLn("        3333333     3:::::3",255,255,255);
    PrintLn("                    3:::::3",255,255,255);
    PrintLn(" ::::::             3:::::3",255,255,255);
    PrintLn(" ::::::     33333333:::::3 ",255,255,255);
    PrintLn(" ::::::     3:::::::::::3  ",255,255,255);
    PrintLn("            33333333:::::3 ",255,255,255);
    PrintLn("                    3:::::3",255,255,255);
    PrintLn("                    3:::::3",255,255,255);
    PrintLn(" ::::::             3:::::3",255,255,255);
    PrintLn(" :::::: 3333333     3:::::3",255,255,255);
    PrintLn(" :::::: 3::::::33333::::::3",255,255,255);
    PrintLn("        3:::::::::::::::33 ",255,255,255);
    PrintLn("         333333333333333   ",255,255,255);
    PrintLn("PANIC!!!",255,0,0);
    

    if (regs->int_no < 32) {
        sprintf(buf, "Exception: %s (#%d)", exception_messages[regs->int_no], regs->int_no);
    } else {
        sprintf(buf, "Exception: (#%d)", regs->int_no);
    }
    
    PrintLn(buf, 255, 255, 0);

    

    

    sprintf(buf, "EIP: 0x%x     CS: 0x%x", regs->eip, regs->cs);
    PrintLn(buf, 255, 255, 255);
    
    sprintf(buf, "EAX: 0x%x  EBX: 0x%x", regs->eax, regs->ebx);
    PrintLn(buf, 200, 200, 200);
    sprintf(buf, "ECX: 0x%x  EDX: 0x%x", regs->ecx, regs->edx);
    PrintLn(buf, 200, 200, 200);
    sprintf(buf, "ESI: 0x%x  EDI: 0x%x", regs->esi, regs->edi);
    PrintLn(buf, 200, 200, 200);
    sprintf(buf, "EBP: 0x%x  ESP: 0x%x", regs->ebp, regs->esp);
    PrintLn(buf, 200, 200, 200);
    sprintf(buf, "EFLAGS: 0x%x", regs->eflags);
    PrintLn(buf, 200, 200, 200);
    

    for(;;) {
        asm volatile("hlt");
    }


}


void default_irq_handler(Registers* regs) {
    pic_send_eoi(regs->int_no - 32);
}


void install_handler(uint8_t interrupt, InterruptHandlerFunc handler) {
    interrupt_mgr.handlers[interrupt].handler = handler;
    interrupt_mgr.handlers[interrupt].active = true;
}

void remove_handler(uint8_t interrupt) {
    interrupt_mgr.handlers[interrupt].handler = nullptr;
    interrupt_mgr.handlers[interrupt].active = false;
}


void enable_interrupts() {
    interrupt_mgr.interrupts_enabled = true;
    asm volatile("sti");
}


void disable_interrupts() {
    interrupt_mgr.interrupts_enabled = false;
    asm volatile("cli");
}


bool are_interrupts_enabled() {
    return interrupt_mgr.interrupts_enabled;
}


void interrupt_init() {

    interrupt_mgr.interrupts_enabled = false;
    

    for (int i = 0; i < 256; i++) {
        interrupt_mgr.handlers[i].handler = nullptr;
        interrupt_mgr.handlers[i].active = false;
    }
    
    // Set up IDT pointer
    interrupt_mgr.idt_ptr.limit = sizeof(IDT_Entry) * 256 - 1;
    interrupt_mgr.idt_ptr.base = (uint32_t)&interrupt_mgr.idt;
    
    // Remap PIC to avoid conflicts with CPU exceptions
    pic_remap(0x20, 0x28);  // Master PIC: IRQ 0-7 -> INT 0x20-0x27
                            // Slave PIC: IRQ 8-15 -> INT 0x28-0x2F
    

    set_idt_gate(0, (uint32_t)isr0);   set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);   set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);   set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);   set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);   set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10); set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12); set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14); set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16); set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18); set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20); set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22); set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24); set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26); set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28); set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30); set_idt_gate(31, (uint32_t)isr31);
    

    set_idt_gate(32, (uint32_t)irq0);  set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);  set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);  set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);  set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);  set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10); set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12); set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14); set_idt_gate(47, (uint32_t)irq15);
    

    for (int i = 0; i < 32; i++) {
        install_handler(i, Visual_exception_handler);
    }
    for (int i = 32; i < 48; i++) {
        install_handler(i, default_irq_handler);
    }
    

    asm volatile("lidt %0" :: "m"(interrupt_mgr.idt_ptr));
    

    irq_clear_mask(2);
    

    enable_interrupts();
}


extern "C" void isr_handler(Registers* regs) {
    if (interrupt_mgr.handlers[regs->int_no].active) {
        interrupt_mgr.handlers[regs->int_no].handler(regs);
    }
}


extern "C" void irq_handler(Registers* regs) {

    if (interrupt_mgr.handlers[regs->int_no].active) {
        interrupt_mgr.handlers[regs->int_no].handler(regs);
    }
    

    pic_send_eoi(regs->int_no - 32);
}