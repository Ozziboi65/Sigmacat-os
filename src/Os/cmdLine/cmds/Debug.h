#include "../../../Kernel/Kernel_Services.h"
#include "../../../Kernel/commonKernel.h"
#include "../../../Kernel/Driver/Time/Pit/Pit.h"
#include "../../../Os/Services/Graphics/graphics.h"

int GetPitTime(struct Command *ToRun){
    char output[67];
    sprintf(output,"Current Tick: %d",time_tick);
    PrintLn(output,255,255,255);
    return 0;
}


int SleepFor(struct Command *ToRun){
    sleep_tick(5);
    return 0;
}