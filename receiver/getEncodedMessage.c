#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef DEBUG
extern FILE *output;
#endif

void getEncodedMessage(ulong *encodedOrRealMessage) {	// TO IMPROVE THE ALGORITHM
	FILE *fin;

	fin = fopen(PATH_TO_PUBLIC_KEY, "rb");
	if (!fin) {
		printf("Can't open the file for reading\n");
		return;
	}

	uchar bufChar = 0;
	ulong buf = 0;
#ifdef DEBUG
	fprintf(output, "Encoded message\n");
#endif
	for (int i = 0; i < size; i++) {
		encodedOrRealMessage[i] = 0;
		for (int j = 0; j < LENGTH_OF_ENCODED_NUMBER; j++) {
			fread(&bufChar, 1, 1, fin);
#ifdef DEBUG
			for (int k = 0; k < 8; k++) {
				fprintf(output, "%d", (bufChar >> (7 - k)) & 1);
			}
			fprintf(output, " ");
#endif
			buf = bufChar;
			encodedOrRealMessage[i] = modularAdd(modularMult(encodedOrRealMessage[i], (ulong)256), buf);
		}
#ifdef DEBUG
		fprintf(output, "\n");
#endif
	}
	fclose(fin);
}