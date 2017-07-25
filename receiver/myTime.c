#include "myTime.h"


/*double getTime() {
	//#ifdef TIME
	struct timespec t;
	clock_gettime (CLOCK_MONOTONIC, &t);
	return (double)t.tv_sec * 1000 + (double)t.tv_nsec / 1000000;
	//#endif
	//return 0;
}*/

double timeRDTSC() {
	//#ifdef TIME
	// union ticks {
	// 	unsigned long long tx;
	// 	struct dblword { long tl,th; } dw; // little endian
	// } t;
	// asm("rdtsc\n": "=a"(t.dw.tl),"=d"(t.dw.th));
	// return (double)t.tx;
	//#endif
	//return 0;
} // for x86 only!