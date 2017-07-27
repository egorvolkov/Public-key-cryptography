#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

uint returnPublicKey(CubePolynomial *publicKey) {
	FILE* fout;
	uint bytes = 0;

	fout = fopen(PATH_TO_PUBLIC_KEY, "wb");
	if (!fout) {
		printf("Can't open the file for return\n");
		return 1;
	}

	uint amount;
	for (int i = 0; i < size; i++) {
		amount = 0;
		for (int j = MAX_TERMS_IN_KEY - 1; j >= 0; j--) {
			if (publicKey[i].factor[j] != 0) {
				break;
			}
			amount++;
		}
		amount = MAX_TERMS_IN_KEY - amount;
		fwrite(&amount, 1, 4, fout);
		bytes += 4;
		fwrite(publicKey[i].factor, 8, amount, fout);
		bytes += 8 * (amount);

		amount = 0;
		for (int j = MAX_VARS_IN_KEY - 1; j >= 0; j--) {
			if (publicKey[i].vars[j] != 0) {
				break;
			}
			amount++;
		}
		amount = MAX_VARS_IN_KEY - amount;
		fwrite(&amount, 1, 4, fout);
		bytes += 4;
		fwrite(publicKey[i].vars, 4, amount, fout);
		bytes += 4 * (amount);
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