/*---------------------
|    INCLUDES         |
---------------------*/
#include "../Kernel_Services.h"
#include "../commonKernel.h"
#include "../../Os/Services/Graphics/graphics.h"
#include <stdint.h>
#include "DriverManager.h"
#include "DriverList.h"
/*---------------------
|   DRIVER INCLUDES   |
---------------------*/



/*---------------------
|     VARIABLES       |
---------------------*/
int DriverCount = sizeof(DriverList) / sizeof(DriverList[0]);

void DriverInit(){
    PrintLn("Started Initializatiting Drivers", 255, 255, 255);
    
    for(int i = 0; i < DriverCount; i++){
        char DriverName[100];
        sprintf(DriverName, "Starting Driver: %s", DriverList[i].name);
        PrintLn(DriverName, 0, 255, 0);
        

        if (DriverList[i].init != 0) {
            int result = DriverList[i].init(&DriverList[i]);
            if (result == 0) {
                PrintLn("Init Sucess", 0, 255, 0);
                DriverList[i].success = true;
            } else {
                char DriverSucessText[67];
                sprintf(DriverSucessText,"Init Fail Return Code: %d",result);
                PrintLn(DriverSucessText, 255, 0, 0);
                DriverList[i].success = false;
            }
        } else {
            PrintLn("Missing Init Function", 255, 255, 255);
        }
    }
    
    PrintLn("Drivers Initializalised...", 255, 255, 255);
}

