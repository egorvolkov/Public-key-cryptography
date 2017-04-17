#include "myTime.h"

double getTime() {
	struct timespec t;
	clock_gettime (CLOCK_MONOTONIC, &t);
	return (double)t.tv_sec * 1000 + (double)t.tv_nsec / 1000000;
}