#include <stdint.h>
#include "../../../interrupts/interruptsMain.h"

extern volatile uint32_t time_tick;

void timer_handler(Registers* regs);
int Init_Pit(struct Driver *driver);

void sleep_tick(uint16_t SleepFor);