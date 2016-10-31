#include "LargeInt.h"
#include <assert.h>

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
}

char Char_ByteToHex(char hex)
{
	char rest = hex % 0xf0;
	hex = (hex - rest) >> 4;
	if (hex >= 0 && hex < 10)
		return hex + 48;
	else if (hex >= 10 && hex < 16)
		return hex + 97 - 10;
}

//char * symbol as [char X, char Y] = 0xXY
char Char_2HexToByte(char symbol[2])
{
	return Char_HexToByte(symbol[0]) << 4 | Char_HexToByte(symbol[1]);
}



struct LargeInt * NEW_LargeInt_from_str(const char * string, int size)
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

char * LargeIntToString(struct LargeInt * LInt)
{
	char * result = (char*)malloc(LInt->size * 2);
	int i, j = 0;
	for (i = LInt->size - 1; i >= 0; i++)
	{
		result[j] = Char_ByteToHex(LInt->LInt[i]);
	}
}