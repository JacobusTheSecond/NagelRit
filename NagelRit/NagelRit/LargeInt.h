#pragma once

struct LargeInt {
	unsigned char * LInt;
	unsigned int size;
};

struct LargeInt * NEW_LargeInt_from_str(const char * string, int size);
char * LargeIntToString(struct LargeInt * LInt);