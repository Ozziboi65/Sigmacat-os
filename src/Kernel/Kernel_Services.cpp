/*---------------------
|   INCLUDES          |
---------------------*/
#include "commonKernel.h"
#include <stdarg.h>
#include "../Os/Services/Graphics/graphics.h"
#include "interrupts/interruptsMain.h"
#include "IO/PortIO.h"

/*---------------------
|      VARIABLES      |
---------------------*/
int CurrentLine = 1;
uint16_t CurrentCursorRow = 0;
uint16_t CurrentCursorColumm = 0;


/*---------------------
|   HELPER FUNCTIONS  |
---------------------*/



// Compare two strings
int strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2)) {
		str1++;
		str2++;
	}
	return *(unsigned char *)str1 - *(unsigned char *)str2;
}



// Convert integer to string (base 10 or 16)
static int IntToString(int value, char *buffer, int base)
{
	char tmp[32];
	int i = 0;
	int len = 0;
	int neg = 0;
	
	if (value == 0) {
		buffer[0] = '0';
		return 1;
	}
	
	if (value < 0 && base == 10) {
		neg = 1;
		value = -value;
	}
	
	while (value > 0) {
		int digit = value % base;
		if (digit < 10)
			tmp[i++] = '0' + digit;
		else
			tmp[i++] = 'a' + (digit - 10);
		value /= base;
	}
	
	if (neg)
		buffer[len++] = '-';
	
	// Reverse the string
	while (i > 0)
		buffer[len++] = tmp[--i];
	
	return len;
}

// Convert unsigned integer to string (base 10 or 16)
static int UIntToString(unsigned int value, char *buffer, int base)
{
	char tmp[32];
	int i = 0;
	int len = 0;
	
	if (value == 0) {
		buffer[0] = '0';
		return 1;
	}
	
	while (value > 0) {
		unsigned int digit = value % base;
		if (digit < 10)
			tmp[i++] = '0' + digit;
		else
			tmp[i++] = 'a' + (digit - 10);
		value /= base;
	}
	
	// Reverse the string
	while (i > 0)
		buffer[len++] = tmp[--i];
	
	return len;
}

// Custom sprintf implementation
int sprintf(char *buffer, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	int len = 0;
	
	while (*fmt) {
		if (*fmt == '%' && *(fmt + 1)) {
			fmt++;
			switch (*fmt) {
				case 'd': {
					// Integer
					int value = va_arg(args, int);
					len += IntToString(value, buffer + len, 10);
					break;
				}
				case 'x': {
					// Hexadecimal
					unsigned int value = va_arg(args, unsigned int);
					len += UIntToString(value, buffer + len, 16);
					break;
				}
				case 's': {
					// String
					const char *str = va_arg(args, const char *);
					while (*str) {
						buffer[len++] = *str++;
					}
					break;
				}
				case 'c': {
					// Character
					int ch = va_arg(args, int);
					buffer[len++] = (char)ch;
					break;
				}
				case '%': {
					// Literal %
					buffer[len++] = '%';
					break;
				}
				default:
					buffer[len++] = '%';
					buffer[len++] = *fmt;
					break;
			}
			fmt++;
		} else {
			buffer[len++] = *fmt++;
		}
	}
	
	va_end(args);
	buffer[len] = '\0';
	return len;
}


/*
		USED TO TOGGLE THE CURSOR VISIBILITY
*/
void SetCursorVis(bool ToggleTo){
	if(!ToggleTo){
		OutByte(0x3D4, 0x0A);
		OutByte(0x3D5, 0x20);  // Set bit 5 to disable cursor
	}

	if(ToggleTo){
		OutByte(0x3D4, 0x0A);	
		OutByte(0x3D5, 0x0E);  // Cursor start line 14 (enables cursor, hides it in top of cell)
		OutByte(0x3D4, 0x0B);
		OutByte(0x3D5, 0x0F);  // Cursor end line 15
	}
}

void SetCursorPos(uint16_t row, uint16_t col){
	uint16_t pos = row * 80 + col;

    CurrentCursorRow = row;
    CurrentCursorColumm = col;

	OutByte(0x3D4, 0x0E);
	OutByte(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

	OutByte(0x3D4, 0x0F);
	OutByte(0x3D5,(uint8_t)(pos & 0xFF));




}




