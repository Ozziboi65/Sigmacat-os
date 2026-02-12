#include "Ps2KeyboardDriver.h"
#include "KeyList.h"
#include "../../Kernel_Services.h"
#include "../../CommonKernel.h"
#include "../../IO/PortIO.h"
#include "../../Input/KernelInputHandler.h"

#include "../../../Os/Services/Graphics/graphics.h"

#include <stdint.h> 

#define DataPort 0x60
#define StatusPort 0x64 //  0 if no data 1 if some data

bool Shifted;

static const char* ReadInput();
static bool Ps2KeyboardPoll(InputReport* out);

int Init_Ps2Driver(struct Driver *driver){ 
    RegisterInputSource("PS2Keyboard", INPUT_KEYBOARD, Ps2KeyboardPoll);
    return 0;
}

static const char* CompareKey(uint16_t ScanCode){
    for(int i = 0; i < sizeof(keymap_FI)/sizeof(keymap_FI[0]); i++){
        if(keymap_FI[i].code == ScanCode){
            if(Shifted) return keymap_FI[i].shift;
            return keymap_FI[i].normal;
        }
    }
    return "\0";
}


static const char* ReadInput(){

    uint8_t Status = InByte(StatusPort);


    if(Status & 0x01){
    
        if(!(Status & 0x20)){

        

            uint16_t Value;

            Value = InByte(DataPort);

            if(Value ==42) Shifted = true;
            if(Value ==170) Shifted = false;




            if((Value & 0x80) == 0){ 
                const char* result = CompareKey(Value);

                return result;
            }
            return "\0";
        }

        return "\0";
    }

    return "\0";

}

static bool Ps2KeyboardPoll(InputReport* out) {
    const char* key = ReadInput();
    if (key && key[0] != '\0') {
        out->type = INPUT_KEYBOARD;
        out->data.keyboard.key = key;
        return true;
    }
    return false;
}




