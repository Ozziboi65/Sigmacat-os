#pragma once
#include <stdint.h>
#include "multiboot.h"

#define VGA_MEMORY ((volatile unsigned short*)0xB8000)
#define WHITE_ON_BLACK 0x0F00
#define GRAY_ON_BLACK 0x0700

#define RED_ON_BLACK 0x0C00
#define GREEN_ON_BLACK 0x0200
#define BLUE_ON_BLACK 0x0100
#define BLACK_ON_BLACK 0x0000



/*---------------------
|    VARIABLES         |
---------------------*/
extern bool ShallThyKernelRun;

extern int CurrentLine;




    /*==========================
    |        STRUCT            |
    ==========================*/



struct KernelInfo{
    bool KernelRunning;
    uint32_t Magic;
    struct multiboot_info* bootInfo;
};



struct HardwareInfo {
    uint32_t mem_lower, mem_upper;
    //const char* cmdline;                 // pointer into boot memory
    uint32_t mods_count;
    multiboot_mod_list* mods;            // pointer to modules array
    uint32_t mmap_length;
    multiboot_mmap_entry* mmap_addr;
    const char* boot_loader_name;

    uint32_t drive_length;

    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint64_t framebuffer_address;
    uint32_t framebuffer_pitch;
    uint8_t framebuffer_bpp;
    
};



extern struct HardwareInfo hardwareInfo;

extern struct KernelInfo kernelinfo;









//       This is where we THINK the cursor is based on how we have moved it
//       prolly unreliable as fuck tho
extern uint16_t CurrentCursorRow;
extern uint16_t CurrentCursorColumm;

/*---------------------
|     FUNCTION        |
---------------------*/






