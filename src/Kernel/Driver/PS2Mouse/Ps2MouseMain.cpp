#include "Ps2Mouse.h"
#include "../../Kernel_Services.h"
#include "../../CommonKernel.h"
#include "../../IO/PortIO.h"
#include "../../../Os/Services/Graphics/graphics.h"

#include "../../interrupts/interruptsMain.h"

#include <stdint.h>

#define Status_register 0x64
#define IsData 0x01
#define DataPort 0x60 
#define handle_number 44 

static bool LastLeftClick = false;
static bool LastRightClick = false;
uint8_t Cfg;


static struct MouseInput ReadMouseInput();
static bool Ps2MousePoll(InputReport* out);

int Init_Ps2MouseDriver(struct Driver *driver) {
    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x64, 0xA8);

    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x64, 0x20);
    while (!(inb(0x64) & 0x01));  // wait for output buffer clear
    Cfg = InByte(0x60);
    Cfg |= 0x02;
    Cfg &= ~0x20;
    
    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x64, 0x60);
    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x60, Cfg);

    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x64, 0xD4);
    while (inb(0x64) & 0x02);  // wait for input buffer clear
    OutByte(0x60, 0xF4);

    while (!(inb(0x64) & 0x01));  // Wait for mouse response
    uint8_t response = inb(0x60);
    










    if(response ==0xFA) {
        RegisterInputSource("PS2Mouse", INPUT_MOUSE, Ps2MousePoll);
    }

    if(response !=0xFA) {
        return 1;
    }  


    //install_handler(handle_number, MouseHandle);

    //irq_clear_mask(12);


    return 0;
}

static struct MouseInput ReadMouseInput() {
    struct MouseInput input = {0};
    
    uint8_t status = inb(Status_register);

    if (status & IsData) {
        if(status & 0x20){
            static int ByteCounter = 0;
            static uint8_t packet[3];

            uint8_t Data = InByte(DataPort);

            if(ByteCounter == 0 && !(Data & 0x08)){
                return input;
            }

            packet[ByteCounter] = Data;
            ByteCounter++;

            if(ByteCounter == 3){
                if(packet[0] & 0x08){

                    
                    if (packet[0] & 0x01) {

                        bool currLeft = (packet[0] & 0x01) != 0;
                        if (currLeft && !LastLeftClick) {

                            input.LeftClick = true;
                        }

                        LastLeftClick = currLeft;

                    } else {
                        // not pressed
                        LastLeftClick = false;
                        input.LeftClick = false;
                    }

                    


                    if (packet[0] & 0x02) {

                        bool currRight = (packet[0] & 0x02) != 0;
                        if (currRight && !LastRightClick) {
                            input.RightClick = true;
                        }

                        LastRightClick = currRight;

                    } else {
                        // not pressed
                        LastRightClick = false;
                        input.RightClick = false;
                    }




                
                ByteCounter = 0; 
                }
            }

            return input;
        }
        return input;
    }

    return input;
}

static bool Ps2MousePoll(InputReport* out) {
    struct MouseInput mouseData = ReadMouseInput();
    if (mouseData.X_delta != 0 || mouseData.Y_delta != 0 ||
        mouseData.LeftClick || mouseData.RightClick || mouseData.MiddleClick) {
        out->type = INPUT_MOUSE;
        out->data.mouse = mouseData;
        return true;
    }
    return false;
}

/*
void MouseHandle(Registers* regs){

}
*/








