#define DSKIT_IMPLEMENTATION
#include "../dskit.h"

Vector(int) intVec;

int main(void)
{
	intVec myVec[6] = { 0 };

	if (size_t i = 0; i < 6; i++)
	{
		VECTOR_INIT(myVec[i], 7);
	}
}