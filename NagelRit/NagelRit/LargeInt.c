#include "LargeInt.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
/*
#define max(a,b) \
		({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		 _a > _b ? _a : _b;})

#define min(a,b) \
		({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b;})
*/

char Char_SymToByte(char symbol)
{
	return (symbol - 48);
}

char Char_HexToByte(char symbol)
{
	if (symbol > 0x2f && symbol < 0x3a) //bereich von "0" bis "9"
		return symbol - 48;
	else if (symbol > 0x40 && symbol < 0x47)//bereich von "A" bis "F"
		return symbol - 65 + 10;
	else if (symbol > 0x60 && symbol < 0x67)//bereich von "a" bis "f"
		return symbol - 97 + 10;
	assert(0);
	return 0;
}

short Char_ByteToHex(char hex)
{
	char low = hex & 0x0f;
	hex = hex >> 4 & 0x0f;
	if (hex >= 0 && hex < 10)
		hex += 48;
	else if (hex >= 10 && hex < 16)
		hex += 65 - 10;
	if (low >= 0 && low < 10)
		low += 48;
	else if (low >= 10 && low < 16)
		low += 65 - 10;
	return (short)low << 8 | (short)hex;
}

//char * symbol as [char X, char Y] = 0xXY
char Char_2HexToByte(char symbol[2])
{
	return Char_HexToByte(symbol[0]) << 4 | Char_HexToByte(symbol[1]);
}



struct LargeInt * NEW_LargeInt_from_str(const char * string, unsigned int size)
{
	if (size == 0) {
		while (string[size++] != 0);
		size--;
	}

	assert(size > 2);
	//first to chars are 0x
	assert(string[0] == '0' && string[1] == 'x');
	//hex prefix

	struct LargeInt * result = (struct LargeInt *)malloc(sizeof(struct LargeInt));
	size = (size - 1) / 2; //(size - 2) /2 + 0.5 (ceil)
	result->LInt = (char *)malloc(size);
	result->size = size;

	string += 2;
	char calc = 0;
	for (size = 0; size < result->size; size++)
	{
		calc = Char_2HexToByte(string + size);
		result->LInt[result->size - size - 1] = calc;
		string++;
	}

	return result;
}


struct LargeInt * add(struct LargeInt* lia, struct LargeInt* lib)
{
	int iterator_max = max(lia->size, lib->size);
	struct LargeInt * longerOne = (lia->size > lib->size) ? lia : lib;
	char carry = 0;
	struct LargeInt * result = (struct LargeInt *) malloc(sizeof(struct LargeInt));
	result->LInt = (char*)malloc(iterator_max + 1);
	int sumcheck;
	for (int i = 0; i <= iterator_max; ++i)
	{
		if (i < min(lia->size, lib->size))
		{
			sumcheck = lia->LInt[i] + lib->LInt[i] + carry;
			result->LInt[i] = (char)(sumcheck % 256);
			if (sumcheck > 255) {
				carry = 0x01;
			}
			//printf("%d\n", sumcheck);
		}
		else if (i < iterator_max) {
			sumcheck = longerOne->LInt[i] + carry;
			result->LInt[i] = (char)(sumcheck % 256);
			if (sumcheck > 255) {
				carry = 0x01;
			}
			//printf("%d\n", sumcheck);
		}
		else if (i == iterator_max) {
			if (carry == 0x0) {
				realloc(result->LInt, iterator_max);
				result->size = iterator_max;
			}
			else {
				result->LInt[i] = carry;
				result->size = iterator_max + 1;
			}
		}
	}
	return result;
}

char * LargeIntToString_Hex(struct LargeInt * lint)
{
	char * result = (char*)malloc(lint->size * 2 + 3);
	result[lint->size * 2 + 2] = 0;
	result[0] = '0';
	result[1] = 'x';
	result += 2;
	int i, j = 0;
	for (i = lint->size - 1; i >= 0; i--)
	{
		*(short*)(result + j) = Char_ByteToHex(lint->LInt[i]);
		j += 2;
	}
	return result - 2;
}

char * LargeIntToString_Dec(struct LargeInt * lint)
{

}