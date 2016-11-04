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
	LargeInt * lia = NEW_LargeInt_from_str("0xffff", 0);
	LargeInt * lib = NEW_LargeInt_from_str("0xff0123", 0);
	LargeInt * lic = add(lia,lib);

	LargeInt * lie = NEW_LargeInt_from_str("0x0111111111111111", 0);
	LargeInt * lif = NEW_LargeInt_from_str("0x0111111111111111", 0);
	LargeInt * lig = mult(lie,lif); 
	LargeInt * lih = pow(lia,lia);
//	LargeInt * lii = divideByTen(lih);
//	LargeInt * lij = divideByTen(lii);
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
	str = LargeIntToString_Hex(lih);
	printf("pow = %s\n", str);
	free(str);
/*	str = LargeIntToString_Hex(lii);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Hex(lij);
	printf("%s\n", str);
	free(str);
*/	str = LargeIntToString_Dec(lia);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Dec(lif);
	printf("%s\n", str);
	free(str);
	str = LargeIntToString_Dec(lig);
	printf("%s\n", str);
	free(str);

	LargeInt * lik = NEW_LargeInt_from_str("0x0A", 0);
	LargeInt * lil = NEW_LargeInt_from_str("0x0A", 0);
	LargeInt * lim; 
/*
	for(int i=0; i<100;++i){
		lim = lil;
		lil = mult(lik,lil);
		str = LargeIntToString_Dec(lil);
		printf("%s\n", str);
		free(str);
		str = LargeIntToString_Hex(lil);
		printf("%s\n", str);
		free(str);
		destructor(lim);
		
	}


	unsigned long long pot = 1;
	double flut = 1;
	for (int i = 0; i < 16; i++)
	{
		printf("%llu %lf\n", pot, flut);
		pot *= 16;
		flut += 1.205;
	}*/

	destructor(lia);
	destructor(lib);
	destructor(lic);
	destructor(lid);
	destructor(lie);
	destructor(lif);
	destructor(lig);
	destructor(lih);
//	destructor(lij);
	destructor(lik);
	destructor(lil);
	/*destructor(lia);
	destructor(lib);
	destructor(lic);
	destructor(lid);*/
	PAUSE
	return EXIT_SUCCESS;
}
