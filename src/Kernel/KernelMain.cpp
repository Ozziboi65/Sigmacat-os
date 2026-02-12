/*---------------------
|    INCLUDES         |
---------------------*/
#include "Kernel_Services.h"	
#include "commonKernel.h"
#include "../Os/cmdLine/common.h"
#include "Input/KernelInputHandler.h"
#include "multiboot.h"
#include "interrupts/interruptsMain.h"
#include "Driver/DriverManager.h"





#include "../Os/Services/Graphics/graphics.h"

// grub info
static struct multiboot_info* mboot_info = nullptr;

HardwareInfo hardwareInfo;
KernelInfo kernelinfo;


















void kernel_main(uint32_t magic, struct multiboot_info* mbi) {

	kernelinfo.Magic = magic;
	kernelinfo.bootInfo = mbi;



	if(magic == MULTIBOOT_MAGIC){

		hardwareInfo.mem_lower = mbi->mem_lower;
		hardwareInfo.mem_upper = mbi->mem_upper;
		hardwareInfo.mmap_length = mbi->mmap_length;
		hardwareInfo.drive_length = mbi->drives_length;
		hardwareInfo.mmap_addr = reinterpret_cast<multiboot_mmap_entry*>(mbi->mmap_addr);
		hardwareInfo.boot_loader_name = reinterpret_cast<const char*>(mbi->boot_loader_name);

		hardwareInfo.framebuffer_width = mbi->framebuffer_width;
		hardwareInfo.framebuffer_height = mbi->framebuffer_height;
		hardwareInfo.framebuffer_address = mbi->framebuffer_addr;
		hardwareInfo.framebuffer_pitch = mbi->framebuffer_pitch;
		hardwareInfo.framebuffer_bpp = mbi->framebuffer_bpp;
		
	}

	if(magic != MULTIBOOT_MAGIC){

		return;
	}

	InitGraphics(hardwareInfo.framebuffer_address, hardwareInfo.framebuffer_width, hardwareInfo.framebuffer_height, hardwareInfo.framebuffer_pitch, hardwareInfo.framebuffer_bpp);

	// Initialize interrupt system
	interrupt_init();

	SetUpCmd();





	const char* CharInput;

	DriverInit();

	


	while(kernelinfo.KernelRunning == true){

		
		PollAllInputSources();

		InputReport input;
		while (PopInputEvent(&input)) {
			if (input.type == INPUT_KEYBOARD) {
				UpdateMain(input.data.keyboard.key);
			}

		}

	}



}






extern "C" void KernelInit(uint32_t magic, struct multiboot_info* mbi){

	//DriverInit();

	kernelinfo.KernelRunning = true;

	
    kernel_main(magic, mbi); //begin the kernel

}








extern "C" void _kernel_main(uint32_t magic, struct multiboot_info* mbi) {
	KernelInit(magic, mbi);
}


