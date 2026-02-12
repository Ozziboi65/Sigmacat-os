#include "graphics.h"
#include <stdint.h>
#include "Fonts/font8x8_basic.h"
#include "../../../Kernel/commonKernel.h"

uint8_t* framebuffer = nullptr;
uint32_t horizontal;
uint32_t vertical;
uint32_t pitch;
uint8_t bitsPerPixel;



int amountToPrint = 0;

int Char_Start_X = 200;
int Char_Start_Y = 200;

#define BgColor 0,0,0
#define CharacterPadding 8
#define LinePadding 6
#define LineStart  200


void InitGraphics(uint64_t framebuffer_addr, uint32_t h, uint32_t v, uint32_t p, uint8_t bpp){

    framebuffer = reinterpret_cast<uint8_t*>(framebuffer_addr);

    horizontal = h;
    vertical = v;
    pitch = p;
    bitsPerPixel = bpp;

    


}




void SetPixel(uint32_t X, uint16_t Y, uint8_t Red, uint8_t Green, uint8_t Blue){

    uint32_t offset = Y * pitch + X * (bitsPerPixel / 8);
    
    framebuffer[offset + 0] = Blue;
    framebuffer[offset + 1] = Green;
    framebuffer[offset + 2] = Red;
}

void DrawCharacter(char Character,uint32_t Start_X, uint32_t Start_Y, uint8_t red, uint8_t green, uint8_t blue){
    int x = 0;
    int y = 0;
    bool bit_state;// if the bit we are looking at is not there or is


    int input = (int)Character;

    int currentCharX = 0;

    int char_X = Start_X;
    int char_Y = Start_Y;




    for(int x = 0; x < 8; x++){
        const unsigned char *character_bit_map = (const unsigned char *)font8x8_basic[input];


        unsigned char pixels = character_bit_map[x];


        for(int y = 0; y < 8; y++){



            bit_state = (pixels & (1u << y)) != 0;



            int cooler_x = y + char_X; // switched x and y so it rotates and not sideways
            int cooler_y = x + char_Y; // switched x and y so it rotates and not sideways


            if (bit_state) SetPixel(cooler_x,cooler_y,blue,green,red); // its BGR not RGB
            else SetPixel(cooler_x,cooler_y,BgColor); 
        }  
    }

    
}




void ClearInputLine() {

    for(int y = 50; y < 58; y++) {
        for(int x = 200; x < horizontal; x++) {
            SetPixel(x, y, BgColor);
        }
    }
}

void ClearLine(int lineNumber) {

    int line_Y = LineStart + (lineNumber * (8 + LinePadding));
    
    for(int y = line_Y; y < line_Y + 8; y++) {
        for(int x = 0; x < horizontal; x++) {
            SetPixel(x, y, BgColor);
        }
    }
}

void PrintLn(const char* Input, uint8_t Red, uint8_t Green, uint8_t Blue){

    CurrentLine++;

    if(Input[0] != '\0'){

        while(Input[amountToPrint] != '\0') amountToPrint++;

        for(int i = 0; i<amountToPrint; i++){
            char CharToPrint = Input[i];


            DrawCharacter(CharToPrint,Char_Start_X,Char_Start_Y,Blue,Green,Red);
            Char_Start_X += CharacterPadding;  
            
        }



    }

    Char_Start_X = LineStart;
    Char_Start_Y += 8 + LinePadding; 
    amountToPrint = 0; 
}

void ClearScreen(){
    for(int i = 0; i < horizontal; i++){

        CurrentLine = 2;
        
        for(int a = 0; a < vertical; a++){
            SetPixel(i,a,BgColor);
        }


    }

    Char_Start_X = LineStart;
    Char_Start_Y = LineStart;
}

void FillScreenAndClear(uint8_t Red, uint8_t Green, uint8_t Blue){
    for(int i = 0; i < horizontal; i++){

        CurrentLine = 0;

        for(int a = 0; a < vertical; a++){
            SetPixel(i,a,Red,Green,Blue);
        }




    }


    Char_Start_X = LineStart;
    Char_Start_Y = LineStart;
}

