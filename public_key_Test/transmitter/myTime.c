#include "transmitter.h"

ulong iters = 0;
float mtime[2] = {0,0};

#ifdef __WIN32
#include <windows.h>
//winonly code
int wininit = 0;
LARGE_INTEGER winfq;
LARGE_INTEGER winclk;
#endif

void startTime(clock_t *timer, ulong *realtime) {
#ifdef TIME
    *timer = clock();
#ifdef __WIN32
	if (wininit == 0){
		QueryPerformanceFrequency(&winfq);
	}
	QueryPerformanceCounter(&winclk);
	*realtime = (ulong)winclk.QuadPart;
#endif
#endif
}

void endTime(clock_t *timer, ulong *realtime) {
#ifdef TIME
	*timer = clock() - (*timer);
#ifdef __WIN32
	QueryPerformanceCounter(&winclk);
	*realtime = (ulong) winclk.QuadPart - (*realtime);
#endif
#endif
}

void printTimes(clock_t times, ulong realtime) {
#ifdef TIME
    iters++;
    printf("CPU time for coding: %.fms\n", ((float)(times*1000))/CLOCKS_PER_SEC);

#ifdef __WIN32
    printf("Real time for coding: %fms\n", (float)(realtime*1000)/winfq.QuadPart);
#endif

    printf("\n");
    mtime[0] += ((float)(times*1000))/CLOCKS_PER_SEC;
    printf("Average CPU time for coding: %.fms\n", mtime[0]/iters);

#ifdef __WIN32
    mtime[1] += (float)(realtime*1000)/winfq.QuadPart;
    printf("Average real time for coding: %fms\n", mtime[1]/iters);
#endif

    printf("\n\n");
#endif
}