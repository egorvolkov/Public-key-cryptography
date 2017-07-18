#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

uint returnPublicKey(FullCubePolynomial *publicKey) {
	FILE* fout;
	uint bytes = 0;
	uint amount = AMOUNT_OF_MEMBERS;

	fout = fopen(PATH_TO_PUBLIC_KEY, "wb");
	if (!fout) {
		printf("Can't open the file for return\n");
		return 1;
	}

	for (int i = 0; i < size; i++) {
		fwrite(publicKey[i].factor, 8, MAX_TERMS_IN_KEY, fout);
		fwrite(publicKey[i].vars, 4, MAX_VARS_IN_KEY, fout);
	}

	// uint bufSize = (size * amount * SIZE_OF_MODULE + 7) / 8;
	// char buf = 0;
	// uint count = 0, bitCount = 0;
	// for (int i = 0; i < size; i++) {
	// 	for (int j = 0; j < amount; j++) {
	// 		for (int k = 0; k < SIZE_OF_MODULE; k++) {
	// 			buf = (buf << 1) | ((publicKey[i * amount + j] >> (SIZE_OF_MODULE - 1 - k)) & 1);
	// 			bitCount++;
	// 			if (bitCount == 8) {
	// 				fwrite(&buf, 1, 1, fout);
	// 				bytes++;
	// 				buf = 0;
	// 				bitCount = 0;
	// 				count++;
	// 			}
	// 		}
	// 	}
	// }
	// if (bitCount != 0) {
	// 	buf <<= (8 - bitCount);
	// 	fwrite(&buf, 1, 1, fout);
	// 	bytes++;
	// }

	fclose(fout);

	return bytes;
}
