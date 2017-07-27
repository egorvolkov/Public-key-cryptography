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

	int amount;
	int flag = 0;
	for (int i = 0; i < size; i++) {
		amount = 0;
		flag = 0;
		for (int j = 0; j < LENGTH_OF_ENCODED_NUMBER; j++) {
			for (int k = 0; k < 8; k++) {
				if ((encodedMessage[LENGTH_OF_ENCODED_NUMBER*i + j] >> (7 - k)) & 1 == 1) {
					flag = 1;
					break;
				}
			}
			if (flag) {
				break;
			}
			amount++;
		}
		amount = LENGTH_OF_ENCODED_NUMBER - amount;
		fwrite(&amount, 1, 4, fout);
		bytes += 4;
		for (int j = LENGTH_OF_ENCODED_NUMBER - amount; j < LENGTH_OF_ENCODED_NUMBER; j++) {
			fwrite(encodedMessage + i*LENGTH_OF_ENCODED_NUMBER + j, 1, 1, fout);
			bytes++;
		}
	}

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