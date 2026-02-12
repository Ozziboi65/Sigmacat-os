#include "PortIO.h"

// Write a byte to an I/O port
void OutByte(uint16_t port, uint8_t value) {
    outb(port, value);
}

// Read a byte from an I/O port
uint8_t InByte(uint16_t port) {
    return inb(port);
}
