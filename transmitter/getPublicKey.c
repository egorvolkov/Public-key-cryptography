#include "transmitter.h"

extern const uchar size;

void getPublicKey(ulong *publicKey) {
	FILE* fin = NULL;

	while (1) {
		fin = fopen(PATH_TO_THE_FIRST_FLAG, "r");
		if (fin)
			break;
	}
	fclose(fin);
	remove(PATH_TO_THE_FIRST_FLAG);

	fin = fopen(PATH_TO_PUBLIC_KEY, "rb");
	if (!fin) {
		printf("Can't open the file for reading\n");
		return;
	}

	uint amount = AMOUNT_OF_MEMBERS;
	uint bufSize = size*amount*MAX_SIZE_OF_MODULE / 8 + ((size*amount*MAX_SIZE_OF_MODULE % 8) ? 1 : 0);
	char buf;

	publicKey[0] = 0;
	uint count = 0, index = 0, bit_count = 0;
	for (int i = 0; i < bufSize; i++) {
		fread(&buf, 1, 1, fin);
		for (int k = 0; k < 8; k++) {
			publicKey[count*amount + index] = (publicKey[count*amount + index] << 1) | ((buf >> (7 - k)) & 1);
			bit_count++;
			if (bit_count == MAX_SIZE_OF_MODULE) {
				bit_count = 0;
				index++;
				if (index == amount) {
					index = 0;
					count++;
					if (count == size) {
						break;
					}
				}
				publicKey[count*amount + index] = 0;
			}
		}
		if (count == size) {
			break;
		}
	}
	fclose(fin);

	factor(publicKey);	//	Раставляем коэфициенты в конечном многочлене. Нужно для подставления значений.
}