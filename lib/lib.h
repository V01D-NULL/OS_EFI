// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev

#pragma once

#define DECIMAL 10
#define HEX     16

#define NULL  (void*)0

#define STDOUT(string) SysTable->ConOut->OutputString(SysTable->ConOut, (CHAR16*)string)

#include <stdarg.h> //Contains macros such as va_list, va_start & va_end
#include "../efi/efi.h"
#include "../efi/ErrorCodes.h"

EFI_SYSTEM_TABLE* SysTable = 0;
EFI_HANDLE ImgHandle = 0;

void EFI_lib_init(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    SysTable = SystemTable;
    ImgHandle = ImageHandle;
}

void* memcpy(void* dst, const void* src, unsigned long long size)
{
    for ( unsigned long long i = 0; i < size; i++ )
        ((unsigned char*) dst)[i] = ((const unsigned char*) src)[i];
    return dst;
}

// Thanks to Poncho for this
void itoa(long int n, unsigned short *buffer)
{
    unsigned char isNeg = 0;
    if(n < 0)
    {
        isNeg = 1;
        n *= -1;
        buffer[0] = '-';
    }
	unsigned char        size;
	long int   SizeTest = n;
    while(SizeTest / 10 > 0)
    {
        SizeTest /= 10;
        size++;
    }
	
    unsigned char  index = 0;
	while(n / 10 > 0)
    {
        unsigned char remainder = n % 10;
        n /= 10;
        buffer[isNeg + size - index] = remainder + '0';
        index++;
    }
	unsigned char rem = n % 10;
	buffer[isNeg + size - index] = rem + '0';
    buffer[isNeg + size + 1] = 0;
}

// My original code for > 0
void uitoa(unsigned long int n, unsigned short *buffer, int basenumber)
{
	unsigned long int hold;
	int i, j;
	hold = n;
	i = 0;
	
	do{
		hold = n % basenumber;
		buffer[i++] = (hold < 10) ? (hold + '0') : (hold + 'a' - 10);
	} while(n /= basenumber);
	buffer[i--] = 0;
	
	for(j = 0; j < i; j++, i--)
	{
		hold = buffer[j];
		buffer[j] = buffer[i];
		buffer[i] = hold;
	}
}

signed int EFI_strlen(const char* string)
{
    int result = 0;
    while (*string++ != '\0')
    {
        result++;
    }
    return result;
}

int EFI_Printf(const char *fmt, ...)
{
    if (SysTable == 0)
    {
        return -1; //Cannot get a handle to SystemTable, therefore printing isn't possible. EFI_lib_init() must be called from the bootloader source file
    }
    va_list ap;
    va_start(ap, fmt);

    for (int i = 0; i < EFI_strlen(fmt); i++)
    {
        switch (fmt[i])
        {
            case '%':
            {
                switch (fmt[i+1])
                {
                    case 'c':
                    {
                        char character = va_arg(ap, int);
                        STDOUT(character);
                        i+=2;
                        break;
                    }
                    case 's':
                    {
                        char *string = va_arg(ap, char*);
                        STDOUT(string);
                        i+=2;
                        break;
                    }
                    //Todo:
                    // Add cases for x, X, d, i, etc. Just convert the number to a decimal or hexadecimal string and print that
                    default:
                        va_end(ap);
                        break;
                }
            }
            default:
                va_end(ap);
                STDOUT(fmt[i]);
        }
    }
    va_end(ap);
    return 0; //Success
}