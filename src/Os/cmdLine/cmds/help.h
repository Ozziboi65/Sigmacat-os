#include "../../../Kernel/Kernel_Services.h"
#include "../../../Kernel/commonKernel.h"
#include "../../../Os/Services/Graphics/graphics.h"

int HelpRun(struct Command *ToRun){
    PrintLn("Help:",255,255,255);
    PrintLn("help | Displays This Message",255,255,255);
    PrintLn("help-debug | Debug Command Help",255,255,255);
    PrintLn("clear | Clears The Screen",255,255,255);
    PrintLn("crash | Tries To Divide By Zero",255,255,255);
    return 0;
}


int HelpRunDebug(struct Command *ToRun){
    PrintLn("Debug Help:",255,255,255);
    PrintLn("debug-pit | Time Of Clock PIT",255,255,255);
    PrintLn("debug-sleep | Sleep For 3 Pit Ticks",255,255,255);
    return 0;
}