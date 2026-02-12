#pragma once
#include <stdint.h>

/*
    Port I/O Functions
    Low-level hardware port access for x86 architecture
*/

// Write a byte to an I/O port
void OutByte(uint16_t port, uint8_t value);

// Read a byte from an I/O port
uint8_t InByte(uint16_t port);

// Inline versions for performance-critical code
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// I/O delay (for timing-sensitive operations)
static inline void io_wait() {
    outb(0x80, 0);
}
