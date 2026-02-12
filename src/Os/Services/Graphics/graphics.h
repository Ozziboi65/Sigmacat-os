#include <stdint.h>

void SetPixel(uint32_t X, uint16_t Y, uint8_t Red, uint8_t Green, uint8_t Blue);

void InitGraphics(uint64_t framebuffer_addr, uint32_t h, uint32_t v, uint32_t p, uint8_t bpp);

void DrawCharacter(char Character,uint32_t Start_X, uint32_t Start_Y, uint8_t red, uint8_t green, uint8_t blue);

void RemoveDrawInput(int target);

void PrintLn(const char* Input, uint8_t Red, uint8_t Green, uint8_t Blue);

void ClearInputLine();

void ClearLine(int lineNumber);

void ClearScreen();

void FillScreenAndClear(uint8_t Red, uint8_t Green, uint8_t Blue);
