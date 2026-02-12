#pragma once
#include <stdint.h>

// Key mapping entry — driver-specific, used by KeyList.h
struct Key {
    uint16_t code;      // Scan code (e.g. 0x1E)
    const char* normal; // "a" or "ä" (supports multi-char)
    const char* shift;  // "A" or "Ä" (supports multi-char)
};

int Init_Ps2Driver(struct Driver *driver);