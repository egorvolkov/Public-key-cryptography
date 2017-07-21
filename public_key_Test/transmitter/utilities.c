#include "transmitter.h"
#include <sys/time.h>
#include <time.h>

extern const uint size;

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

// void factor(ulong *publicKey) {
// 	uint amount = AMOUNT_OF_MEMBERS, cur = 0;

// 	for (int i = 0; i < size; i++) {
// 		for (int j = 0; j < amount; j++) {
// 			publicKey[(i + size)*amount + j] = 0;
// 		}
// 	}

// 	for (int i = 0; i < size; i++) {
// 		for (int j = 0; j < size; j++) {
// 			if (j == i) {
// 				publicKey[(size + j)*amount+cur] = 3;
// 			}
// 		}
// 		cur++;
// 	}

// 	for (int i = 0; i < size; i++) {
// 		for (int j = 0; j < size; j++) {
// 			if (j == i)
// 				continue;
// 			publicKey[(size + i)*amount+cur] = 2;
// 			publicKey[(size + j)*amount+cur] = 1;
// 			cur++;
// 		}
// 	}

// 	for (int i = 0; i < size; i++) {
// 		for (int j = i + 1; j < size; j++) {
// 			for (int k = j + 1; k < size; k++) {
// 				publicKey[(size + i)*amount+cur] = 1;
// 				publicKey[(size + j)*amount+cur] = 1;
// 				publicKey[(size + k)*amount+cur] = 1;
// 				cur++;
// 			}
// 		}
// 	}
// }
