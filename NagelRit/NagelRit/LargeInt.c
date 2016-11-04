#include "LargeInt.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER

#define max(a,b) \
		({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		 _a > _b ? _a : _b;})

#define min(a,b) \
		({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b;})
#endif


char Char_SymToByte(char symbol)
{
	return (symbol - 48);
}

/* recieves a char, and returns its corresponding value as a byte
   '3' -> 0x3
   'F' -> 0xf
   this is needed, for creating the bitfield from a string
*/
char Char_HexToByte(char symbol)
{
	//printf("%c %d\n",symbol,(int)symbol);
	if (symbol > 0x2f && symbol < 0x3a) //bereich von "0" bis "9"
		return symbol - '0';
	else if (symbol > 0x40 && symbol < 0x47)//bereich von "A" bis "F"
		return symbol - 65 + 10;
	else if (symbol > 0x60 && symbol < 0x67)//bereich von "a" bis "f"
		return symbol - 97 + 10;
	assert(0);
	return 0;
}

/* recieves a char, inwhich two hex-digits are stored, and returns
   it as two chars in a twi bytes
   0xf4 -> (short)"F4"
   short is not optimal here, as the size may vary from 2 bytes
*/
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

/* parses two chars into their corresponding value in hex, and
   concatenates them into one byte
   "AF" -> (char)0xaf
*/
char Char_2HexToByte(char symbol[2])
{
	return Char_HexToByte(symbol[0]) << 4 | Char_HexToByte(symbol[1]);
}


/* recieves a string, which will be parsed into their corresponding hex
   values and returns a pointer to the struct, containing a pointer to
   the bitfield stored as a char array, and the size
   ("0x1234",0) -> {&(0x1234),2}

   TODO:support for an uneven amount of chars, as only char_2HexToByte
		is called
*/
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
		calc = Char_2HexToByte((char*)(string + size));
		result->LInt[result->size - size - 1] = calc;
		string++;
	}

	return result;
}

/* adds two LargeInts and returns their sum as a new LargeInt which has
   to be properly destructed as well.
*/
struct LargeInt * add(struct LargeInt* lia, struct LargeInt* lib)
{
	unsigned int iterator_max = max(lia->size, lib->size);
	struct LargeInt * longerOne = (lia->size > lib->size) ? lia : lib;
	char carry = 0;
	struct LargeInt * result = (struct LargeInt *) malloc(sizeof(struct LargeInt));
	result->LInt = (char*)malloc(iterator_max + 1);
	int sumcheck;

	/* iterates through both LargeInts simultaneously, and
	   1. adds their 2 bytes + carry from last byte, if neither array is at their end yet
	   2. adds the carry and the byte from the longer one, if one array is at the end
	   3. appends the carry, or cuts the length, if both are at the end, depending on the carry*/
	for (unsigned int i = 0; i <= iterator_max; ++i)
	{
		//first case
		if (i < min(lia->size, lib->size))
		{
			sumcheck = lia->LInt[i] + lib->LInt[i] + carry;
			result->LInt[i] = (char)(sumcheck % 256);
			if (sumcheck > 255) {
				carry = 0x01;
			}
			else {
				carry = 0x00;
			}
		}
		//second case
		else if (i < iterator_max) {
			sumcheck = longerOne->LInt[i] + carry;
			result->LInt[i] = (char)(sumcheck % 256);
			if (sumcheck > 255) {
				carry = 0x01;
			}
			else {
				carry = 0x00;
			}
		}
		//third case
		else if (i == iterator_max) {
			if (carry == 0x0) {
				result->LInt = realloc(result->LInt, iterator_max);
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

/* recieves a LargeInt and returns a String, depicting the hexvalues of
   the array
   {&(0x1274),2} -> "0x1274"
*/
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

/* frees the array, and the LargeInt itself, as both have been malloc'd in their life cycle
*/
void destructor(struct LargeInt * li) {
	free(li->LInt);
	free(li);
}

void cutEnd(struct LargeInt * li) {
	for (int i = li->size - 1; i >= 0; i--) {
		if (li->LInt[i] != 0) {
			struct LargeInt * asstitsndick = li->LInt;
			if (i == li->size - 1)
			{
				return;
			}
			li->LInt = realloc(li->LInt, i + 1);
			li->size = i + 1;
			return;
		}
	}
}

struct LargeInt * bitshiftup(struct LargeInt * li, int amount) {
	if (amount <= 0) {
		assert(0);
	}
	struct LargeInt * result = (struct LargeInt *) malloc(sizeof(struct LargeInt));
	unsigned char* byteadjustedpointer;
	if (amount > 8) {
		result->LInt = calloc(li->size + amount / 8 + 1, 1);
		byteadjustedpointer = result->LInt + amount / 8;
		amount = amount % 8;
	}
	else {
		result->LInt = malloc(li->size + 1);
		byteadjustedpointer = result->LInt;
	}

	char carry = 0x0;
	short currbyte;
	for (unsigned int i = 0; i < li->size; ++i) {
		currbyte = (short)li->LInt[i];
		currbyte = (currbyte << amount);
		byteadjustedpointer[i] = currbyte | carry;
		carry = currbyte >> 8;
	}
	if (carry != 0x0) {
		byteadjustedpointer[li->size] = carry;
		result->size = li->size + byteadjustedpointer - result->LInt + 1;
	}
	else {
		result->size = li->size + byteadjustedpointer - result->LInt;
		result->LInt = realloc(result->LInt, result->size);
	}
	return result;
}


struct LargeInt * mult(struct LargeInt * lia, struct LargeInt * lib) {
	struct LargeInt * result = malloc(sizeof(struct LargeInt));
	result->LInt = calloc(lia->size + lib->size, 1);
	//TODO cut function at end
	result->size = lia->size + lib->size;

	int iterator_min = min(lia->size, lib->size);
	struct LargeInt * shorterOne = (lia->size < lib->size) ? lia : lib;
	struct LargeInt * longerOne = (lia->size < lib->size) ? lib : lia;

	unsigned char shiftcarry = 0x0;
	unsigned char addcarry = 0x0;
	unsigned short placeholder = 0x0;
	short addbyte = 0x0;
	short currbyte = 0x0;
	for (int i = 0; i < iterator_min; ++i) {
		//printf("\n\n\ni = %d\n",i);
		currbyte = (short)shorterOne->LInt[i];
		for (int bitit = 0; bitit < 8; bitit++) {
			//printf("   bitit = %d\n",bitit);
			short bit = (short)currbyte & 0x1;
			//printf("   bit = %d\n",bit);
			if (bit == 1) {
				//result add longerOne << bitit
				for (unsigned int j = 0; j < longerOne->size || addcarry != 0x0 || shiftcarry != 0x0; ++j) {
					//printf("      j = %d\n",j);
					if (j < longerOne->size) {
						addbyte = longerOne->LInt[j];
						//printf("            addbyte = %d\n",addbyte);
						addbyte = addbyte << bitit;
						//printf("            addbyte = %d\n",addbyte);

						//result[byteadjust+i] += (longerOne[j] << bitit) + addcarry + shiftcarry
						placeholder = result->LInt[i + j] + (addbyte & 0xff) + addcarry + shiftcarry;
						//printf("            placeholder = %d\n",placeholder);
						addcarry = placeholder >> 8;
						result->LInt[i + j] = placeholder % 0x100;
						shiftcarry = addbyte >> 8;
					}
					else {
						placeholder = result->LInt[i + j] + addcarry + shiftcarry;
						addcarry = placeholder >> 8;
						result->LInt[i + j] = placeholder % 0x100;
						shiftcarry = 0x0;

					}
				}
			}
			currbyte = currbyte >> 1;
			if (currbyte == 0)break;
		}
	}
	cutEnd(result);
	return result;
}

struct LargeInt * oneTenth(int length) {
	struct LargeInt * oneTenth = (struct LargeInt *)malloc(sizeof(struct LargeInt));
	oneTenth->LInt = malloc(length);
	oneTenth->size = length;
	for (int i = 0; i < length - 1; ++i) {
		oneTenth->LInt[i] = 0x99;
	}
	oneTenth->LInt[length - 1] = 0x19;
	oneTenth->LInt[0] = ((oneTenth->LInt[0] & 0xf0) | 0x0A);
	return oneTenth;
}

struct LargeInt * divideByTen(struct LargeInt * li) {
	struct LargeInt * oT = oneTenth(li->size + 1);
	struct LargeInt * ph = mult(li, oT);

	struct LargeInt * result = (struct LargeInt *)malloc(sizeof(struct LargeInt));
	result->LInt = malloc(ph->size - li->size);
	result->size = ph->size - li->size;

	for (int i = 0; i + li->size < ph->size; ++i) {
		result->LInt[i] = ph->LInt[i + oT->size];
	}


	cutEnd(result);
	destructor(oT);
	destructor(ph);
	return result;
}

/* TODO: recieves a LargeInt and returns a String, depicting the decimal value of the array
   {&(0x30),1} -> "48"
*/
short intFromHalfByte(char a) {
	return ((short)a) * 6 % 10;
}
char lowestdigitinDec(struct LargeInt * lint) {
	long long sum = 0;
	for (unsigned int i = 1; i < lint->size; ++i) {
		sum += intFromHalfByte(lint->LInt[i] & 0x0f) + intFromHalfByte(lint->LInt[i] >> 4 & 0x0f);
	}
	sum += (lint->LInt[0] & 0x0f) % 10 + intFromHalfByte(lint->LInt[0] >> 4 & 0x0f);
	return ((char)(sum % 10)) + '0';
}
char * LargeIntToString_Dec(struct LargeInt * lint)
{
	if (lint->size == 0) {
		return "0";
	}
	char * result = calloc(lint->size * 2, 1);
	result[0] = lowestdigitinDec(lint);
	struct LargeInt* iterator = divideByTen(lint);
	struct LargeInt* oldLI;
	int i;
	for (i = 1; iterator->size > 1 || (iterator->size == 1 && iterator->LInt[0] != 0x0); ++i) {
		oldLI = iterator;
		result[i] = lowestdigitinDec(iterator);
		iterator = divideByTen(iterator);
		destructor(oldLI);
	}
	char * actualresult = malloc(i + 1);
	for (int j = i - 1; j >= 0; --j) {
		actualresult[i - 1 - j] = result[j];
	}
	actualresult[i] = 0;
	free(result);
	destructor(iterator);
	return actualresult;
}
