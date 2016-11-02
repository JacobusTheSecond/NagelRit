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

	LargeInt * lie = NEW_LargeInt_from_str("0x0111111111111111", 0);
	LargeInt * lif = NEW_LargeInt_from_str("0x0111111111111111", 0);
	LargeInt * lig = mult(lie,lif); 
	
	char * str = LargeIntToString_Hex(lia);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lib);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lic);
	printf("%s\n", str);
	free(str);

	LargeInt * lid = bitshiftup(lia,14);
	str = LargeIntToString_Hex(lid);
	printf("%s\n\n", str);
	free(str);	

	str = LargeIntToString_Hex(lie);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lif);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lig);
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

	destructor(lia);
	destructor(lib);
	destructor(lic);
	destructor(lid);
	PAUSE
	return EXIT_SUCCESS;
}
