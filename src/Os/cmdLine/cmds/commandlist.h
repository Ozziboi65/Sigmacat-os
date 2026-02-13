    /*==========================
    |        INCLUDES          |
    ==========================*/

// Declare struct Command BEFORE including handlers so they see the full type
struct Command{
    const char* CommandName; // clear
    int (*init)(struct Command *ToRun); //Pointer To the function to run
};

#include "clear.h"
#include "help.h"
#include "follow.h"
#include "FuckTheCpu.h"
#include "Debug.h"
#include "whoami.h"

static struct Command CmdList[] = {

    {"help", HelpRun},
    {"who2follow", WhoToFollowRun},
    {"clear", ClearRun},
    {"crash", FuckUcpu},
    {"help-debug", HelpRunDebug},
    {"whoami", WhoAmI},


    {"debug-pit", GetPitTime},
    {"debug-sleep", SleepFor}


};
