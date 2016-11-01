#ifdef _MSC_VER
#define PAUSE system("pause");
#else
#define PAUSE
#endif
#include <stdio.h>
#include "LargeInt.h"
#include <stdlib.h>

typedef struct LargeInt LargeInt;

int main()
{
	LargeInt * lia = NEW_LargeInt_from_str("0xffabcd", 0);
	LargeInt * lib = NEW_LargeInt_from_str("0xff0123", 0);
	LargeInt * lic = add(lia,lib);
	
	char * str = LargeIntToString_Hex(lia);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lib);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lic);
	printf("%s\n", str);
	free(str);

	unsigned long long pot = 1;
	double flut = 1;
	for (int i = 0; i < 16; i++)
	{
		printf("%llu %lf\n", pot, flut);
		pot *= 16;
		flut += 1.205;
	}

	PAUSE
	return EXIT_SUCCESS;
}
