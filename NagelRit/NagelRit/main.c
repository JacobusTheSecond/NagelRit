#include <stdio.h>
#include "LargeInt.h"
#include <stdlib.h>

typedef struct LargeInt LargeInt;

int main()
{
	LargeInt * li = NEW_LargeInt_from_str("0x0102030405060708091011121314151617181920", 0);
	printf("%s\n", li->LInt);

	
	system("pause");
	return EXIT_SUCCESS;
}