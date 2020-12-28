// File Created by TheOSDev (ExoVerse , Mara )
// https://github.com/ThatOSDev

#pragma once

#define DECIMAL 10
#define HEX     16

#define NULL  0

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