#include "receiver.h"
#include <sys/time.h>

extern const uchar size;
extern struct Module moduleStruct;

uchar inArray(ulong *arr, uint length, ulong element) {
	for (int i = 0; i < length; i++) {
		if (arr[i] == element)
			return 1;
	}
	return 0;
}

uint getModules(uint index) {
	FILE *fin = fopen(PATH_TO_MODULES, "rb");

	uint a = 0;

	fseek(fin, index * 4, SEEK_SET);
	fread(&a, 4, 1, fin);
	fclose(fin);
	return a;
}

// Static global for current file seed variable for correct work
static ulong seed;

/*
*    Just prepare starting seed
*/
void prepare(ulong *preSeed)
{
	//seed = (ulong)preSeed;   // Seed initialization as a adress number in common scope with other generator functions
	//struct timespec t;
	//clock_gettime (CLOCK_MONOTONIC, &t);
	//seed = t.tv_nsec;
	srand(time(NULL));
	seed = rand();
}

ulong randMain()
{
	ulong next = seed;
	ulong result;

	next *= 1103515245;
	next += 12345;
	result = (next >> 16) % 2048; // next / 65536

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (next >> 16) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (next >> 16) % 1024;

	seed = next;

	return result;
}

/*
*    Wrapper function. Final for use
*/
ulong getRandom(ulong module)
{
	// Static variable for counting function calls
	static ulong quan = 0;

	// Is it first call of this fucntion? If so, then initialization our static seed
	if (!quan) {
		prepare(&module);
		quan++;
	}

	// Return our random number modulo 'module'
	return randMain() % module;
}

uchar cube(ulong x) {
	if (x > (1 << 22)) {
		return 1;
	}
	unsigned long long y = x * x;
	if (y > moduleStruct.module)
		return 1;
	y *= x;
	if (y > moduleStruct.module)
		return 1;
	return 0;
}
