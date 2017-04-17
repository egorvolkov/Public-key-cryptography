#include "transmitter.h"

extern const uchar size;

void createMessage(ulong *message) {
#ifdef DEBUG
	FILE *fout = fopen(PATH_TO_MESSAGE, "w");
#endif
	for (int i = 0; i < size; i++) {
		message[i] = getRandom((ulong)1 << SIZE_OF_VARIABLE);
#ifdef DEBUG
		fprintf(fout, "%llu ", message[i]);
#endif
	}
#ifdef DEBUG
	fclose(fout);
#endif
}