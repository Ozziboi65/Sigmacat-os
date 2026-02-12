#pragma once

#include <stdint.h>




extern int CurrentLine;







/*
    Compare two strings
    Returns: 0 if equal
    negative if str1 < str2
    positive if str1 > str2
    
*/

int strcmp(const char *str1, const char *str2);

/*
    Custom sprintf - no standard library
    Supports: %d (int), %s (string), %x (hex), %c (char)
*/
int sprintf(char *buffer, const char *fmt, ...);

/*
    IO, used in Kernel_Services
*/


void SetCursorVis(bool ToggleTo);

void SetCursorPos(uint16_t row, uint16_t col);