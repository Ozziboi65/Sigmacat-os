/*---------------------
|    INCLUDES         |
---------------------*/
#include "../commonKernel.h"
#include "DriverManager.h"
/*---------------------
|   DRIVER INCLUDES   |
---------------------*/
#include "Serial/serial_driver.h"
#include "PS2Keyboard/Ps2KeyboardDriver.h"
#include  "PS2Mouse/Ps2Mouse.h"
#include  "Time/Pit/Pit.h"

static struct Driver DriverList[] = {
	{"PS2Keyboard", Init_Ps2Driver, false},
	{"PS2Mouse", Init_Ps2MouseDriver, false},
	{"PitTimer", Init_Pit, false}

};



