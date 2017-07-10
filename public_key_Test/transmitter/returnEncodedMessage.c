#include "transmitter.h"

extern const uint size;

uint returnEncodedMessage(uchar *encodedMessage) {
	FILE* fout;
	uint bytes = 0;

	fout = fopen(PATH_TO_PUBLIC_KEY, "wb");
	if (!fout) {
		printf("Can't open the file for return\n");
		return 1;
	}

	fwrite(encodedMessage, 1, size*LENGTH_OF_ENCODED_NUMBER, fout);
	bytes += size*LENGTH_OF_ENCODED_NUMBER;

	fclose(fout);

	remove(PATH_TO_THE_FIRST_FLAG);

	fout = fopen(PATH_TO_THE_SECOND_FLAG, "wb");
	if (!fout) {
		printf("Can't open the file for setting a flag\n");
		return 1;
	}
	fclose(fout);

	return bytes;
}