#include "common.h"
#include "../../Kernel/commonKernel.h"
#include "../../Kernel/Kernel_Services.h"
#include "cmds/commandlist.h"
#include "../Services/Graphics/graphics.h"

/*==========================
|        Variables         |
==========================*/
char OutputKey[2]; // the input character we get from kernel
char TypedCmd[256]; // the current command the user is typing
char CmdOutput[259]; // The command line text we show to user prints ">" and the TypedCmd
int CurrentChar = 0;    // the amount of characters typed
int InputLine; // line we typing on

/*==========================
|        HARDWARE          |
==========================*/
uint32_t MinMem = 0;





void SetUpCursor(){
    CurrentCursorRow = 1;
    CurrentCursorColumm = 1;
    SetCursorVis(true);
    SetCursorPos(1,1);
}

void SetUpCmd(){
    SetUpCursor();
    PrintLn("_=============================_",255,255,255);
    PrintLn("Type help For List Of Commands",255,255,255);
    PrintLn("_=============================_",255,255,255);
    InputLine = CurrentLine + 1;
}










void UpdateMain(const char* InputKey){
    if(InputKey[0] != '\0'){ 
        NewInput(InputKey);
        InputLine = CurrentLine + 1;
        SetCursorPos(InputLine,CurrentChar +1);
        sprintf(CmdOutput, ">%s ", TypedCmd);
    }
}

void FindCmd(){
    int i = 0;
    int listSize = sizeof(CmdList) / sizeof(CmdList[0]);

    while(i < listSize){





        if(strcmp(TypedCmd, CmdList[i].CommandName) == 0){
            int result = CmdList[i].init(&CmdList[i]);
            if(result != 0){
                // cmd fail
            }
            return;
        }

        i++;
    }
    char noCommandChar[250];

    sprintf(noCommandChar,"%s   Is Not An Valid Command...", TypedCmd);
    PrintLn(noCommandChar,255,0,0);
}


void ClearInputString(){
    int charAmount = CurrentChar;
    for(int i = 0; i < charAmount; i++){
        TypedCmd[i] = '\0';
        CmdOutput[i] = '\0';
        CurrentChar = 0;
    }
}

void EnterPressed(){
    CurrentLine += 2;
    // Clear the input line before moving on
    ClearInputLine();
    PrintLn(CmdOutput,255,255,255);
    FindCmd();
    ClearInputString();
}


void NewInput(const char* Input){
    if(strcmp(Input, "!ENTER!") == 0){
        EnterPressed();
    }
    else if(strcmp(Input, "!BACK!") == 0){
        if(CurrentChar > 0){
            int test = CurrentChar - 1;
            RemoveDrawInput(test);
            CurrentChar--;
            TypedCmd[CurrentChar] = '\0';
        }
    }
    else{// normal character
        int i = 0;
        while(Input[i] != '\0' && CurrentChar < 255){
            DrawInput(Input[i]);
            TypedCmd[CurrentChar] = Input[i];
            CurrentChar++;
            i++;
        }
        TypedCmd[CurrentChar] = '\0';
    }
}

void DrawInput(char Input){
    int DrawCord = (CurrentChar * 10) + 200;
    DrawCharacter(Input,DrawCord,50,255,255,255);
}



void RemoveDrawInput(int target){
    int DrawCord = (target * 10) + 200;
    DrawCharacter(' ',DrawCord,50,0,0,0);
}

