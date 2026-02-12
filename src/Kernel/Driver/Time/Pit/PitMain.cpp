#include "Pit.h"
#include <stdint.h>
#include "../../../IO/PortIO.h"

#include "../../../Kernel_Services.h"

#include "../../../interrupts/interruptsMain.h"
#include "../../../../Os/Services/Graphics/graphics.h"

#define IO_Port 0x40
#define Cmd_Register 0x43
#define handle_number 32

volatile uint32_t time_tick = 0;

int Init_Pit(struct Driver *driver){
    uint16_t freq = 1000;
    uint16_t divisor = 1193182 / freq;  


    disable_interrupts();

    outb(Cmd_Register, 0x36);

    outb(IO_Port, divisor & 0xFF);
    outb(IO_Port, (divisor >> 8) & 0xFF);


    install_handler(handle_number, timer_handler);

    irq_clear_mask(0);

    enable_interrupts();


    return 0;

}

void sleep_tick(uint16_t SleepFor){

    disable_interrupts();
    uint32_t StartTick = time_tick;
    enable_interrupts();

    uint32_t sleepEndTick = StartTick + SleepFor;

    char buf[122];
    sprintf(buf,"BecanTick: %d",time_tick);
    PrintLn(buf,0,255,0);



    while(time_tick < sleepEndTick){
        asm volatile("hlt");
    }




    sprintf(buf,"EndTick: %d",time_tick);
    PrintLn(buf,255,0,0);

    int Diff = time_tick - StartTick;
    sprintf(buf,"Difference: %d",Diff);
    PrintLn(buf,255,255,0);

}

void timer_handler(Registers* regs){
    time_tick++;


}