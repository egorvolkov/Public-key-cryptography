#include "receiver.h"

ulong iters = 0;
float mtime[6] = {0,0,0,0,0,0};

#ifdef PRINT
extern FILE *output;
#endif

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

void printTimes(clock_t *times, ulong *realtime) {
#ifdef TIME
    iters++;
    printf("CPU time for secret key generation: %.fms\n", ((float)(times[0]*1000))/CLOCKS_PER_SEC);
    printf("CPU time for public key generation: %.fms\n", ((float)(times[1]*1000))/CLOCKS_PER_SEC);
    printf("CPU time for decoding: %.fms\n", ((float)(times[2]*1000))/CLOCKS_PER_SEC);
#ifdef __WIN32
	printf("Real time for secret key generation: %fms\n",(float)(realtime[0]*1000)/winfq.QuadPart);
	printf("Real time for public key generation: %fms\n", (float)(realtime[1]*1000)/winfq.QuadPart);
	printf("Real time for decoding: %fms\n", (float)(realtime[2]*1000)/winfq.QuadPart);
#endif
	printf("\n");
	for (int i = 0; i < 3; i++) {
		mtime[2*i] += ((float)(times[i]*1000))/CLOCKS_PER_SEC;
#ifdef __WIN32
		mtime[2*i+1] += (float)(realtime[i]*1000)/winfq.QuadPart;
#endif
	}
	printf("Average CPU time for secret key generation: %.fms\n", mtime[0]/iters);
    printf("Average CPU time for public key generation: %.fms\n", mtime[2]/iters);
    printf("Average CPU time for decoding: %.fms\n", mtime[4]/iters);

#ifdef __WIN32
	printf("Average real time for secret key generation: %fms\n", mtime[1]/iters);
	printf("Average real time for public key generation: %fms\n", mtime[3]/iters);
	printf("Average real time for decoding: %fms\n", mtime[5]/iters);
#endif

#ifdef PRINT
	fprintf(output, "CPU time for secret key generation: %.fms\n", ((float)(times[0]*1000))/CLOCKS_PER_SEC);
	fprintf(output, "CPU time for public key generation: %.fms\n", ((float)(times[1]*1000))/CLOCKS_PER_SEC);
    fprintf(output, "CPU time for decoding: %.fms\n", ((float)(times[2]*1000))/CLOCKS_PER_SEC);
	fprintf(output, "Average CPU time for secret key generation: %.fms\n", mtime[0]/iters);
	fprintf(output, "Average CPU time for public key generation: %.fms\n", mtime[2]/iters);
	fprintf(output, "Average CPU time for decoding: %.fms\n", mtime[4]/iters);
#ifdef __WIN32
    fprintf(output, "Real time for secret key generation: %fms\n", (float)(realtime[0]*1000)/winfq.QuadPart);
    fprintf(output, "Real time for public key generation: %fms\n", (float)(realtime[1]*1000)/winfq.QuadPart);
    fprintf(output, "Real time for decoding: %fms\n", (float)(realtime[2]*1000)/winfq.QuadPart);
    fprintf(output, "Average real time for secret key generation: %fms\n", mtime[1]/iters);
    fprintf(output, "Average real time for public key generation: %fms\n", mtime[3]/iters);
    fprintf(output, "Average real time for decoding: %fms\n", mtime[5]/iters);
#endif
#endif
#endif
}