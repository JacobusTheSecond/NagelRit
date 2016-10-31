#include <stdio.h>
#include "LargeInt.h"
#include <stdlib.h>

typedef struct LargeInt LargeInt;

int main()
{
	LargeInt * lia = NEW_LargeInt_from_str("0xff", 0);
	LargeInt * lib = NEW_LargeInt_from_str("0xff", 0);
	//printf("%s\n", li->LInt);
	LargeInt * lic = add(lia,lib);
	
	return EXIT_SUCCESS;
}
