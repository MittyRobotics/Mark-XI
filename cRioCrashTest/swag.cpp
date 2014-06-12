#include "swag.h"

swag::swag():
    test(6.9)
{
   printf("Inside swag.cpp\n");
}

swag::~swag()
{
}

void swag::swagOut()
{
	printf("%f", this->test);

   	printf("swagging out\n");
}

swag* swag::getPointer()
{
    return 0;
}
