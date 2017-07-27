#include "transmitter.h"

ulong iters = 0;
float mtime[2] = {0,0};

//winonly code
int wininit = 0;
LARGE_INTEGER winfq;
LARGE_INTEGER winclk;

void startTime(clock_t *timer, ulong *realtime) {
#ifdef TIME
    *timer = clock();
	if (wininit == 0){
		QueryPerformanceFrequency(&winfq);
	}
	QueryPerformanceCounter(&winclk);
	*realtime = (ulong)winclk.QuadPart;
#endif
}

void endTime(clock_t *timer, ulong *realtime) {
#ifdef TIME
	*timer = clock() - (*timer);
	QueryPerformanceCounter(&winclk);
	*realtime = (ulong) winclk.QuadPart - (*realtime);
#endif
}

void printTimes(clock_t times, ulong realtime) {
#ifdef TIME
    iters++;
    printf("CPU time for coding: %.fms\n", ((float)(times*1000))/CLOCKS_PER_SEC);
	printf("Real time for coding: %fms\n", (float)(realtime*1000)/winfq.QuadPart);
    printf("\n");
    mtime[0] += ((float)(times*1000))/CLOCKS_PER_SEC;
    mtime[1] += (float)(realtime*1000)/winfq.QuadPart;
    printf("Average CPU time for coding: %.fms\n", mtime[0]/iters);
    printf("Average real time for coding: %fms\n", mtime[1]/iters);
    printf("\n\n");
#endif
}