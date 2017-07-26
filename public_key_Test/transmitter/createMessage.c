#include "transmitter.h"

extern const uint size;

void createMessage(ulong *message) {
	FILE *fout = fopen(PATH_TO_MESSAGE, "w");
	for (int i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		message[i] = getRandom(2 * 3 * 5 * 7 * 11 * 13 * 17 * 19);
		fprintf(fout, "%llu ", message[i]);
	}
	fclose(fout);
}
