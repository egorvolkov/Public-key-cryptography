#include "transmitter.h"

extern const uchar size;

void createMessage(ulong *message) {
	FILE *fout = fopen(PATH_TO_MESSAGE, "w");
	for (int i = 0; i < size; i++) {
		message[i] = getRandom((ulong)1 << SIZE_OF_VARIABLE);
		fprintf(fout, "%llu ", message[i]);
	}
	fclose(fout);
}