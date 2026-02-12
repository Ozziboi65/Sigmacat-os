
#ifndef KERNEL_DRIVERS_H
#define KERNEL_DRIVERS_H

void DriverInit();

struct Driver {
	const char* name;
    int (*init)(struct Driver *Driver_Instance); //Pointer To init of the driver and 
	bool success;
};



//variables
extern int DriverCount;


#endif // KERNEL_DRIVERS_H

