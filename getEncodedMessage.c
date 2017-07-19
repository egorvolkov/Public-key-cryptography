#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

#ifdef PRINT
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
#ifdef PRINT
	fprintf(output, "Encoded message:\n"); printf("Encoded message:\n");
#endif
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		encodedOrRealMessage[i] = 0;
		for (int j = 0; j < LENGTH_OF_ENCODED_NUMBER; j++) {
			fread(&bufChar, 1, 1, fin);
#ifdef PRINT
			for (int k = 0; k < 8; k++) {
				fprintf(output, "%d", (bufChar >> (7 - k)) & 1); printf("%d", (bufChar >> (7 - k)) & 1);
			}
			fprintf(output, " "); printf(" ");
#endif
			buf = bufChar;
			encodedOrRealMessage[i] = modularAdd(modularMult(encodedOrRealMessage[i], (ulong)256), buf);
		}
#ifdef PRINT
		fprintf(output, "\n"); printf("\n");
#endif
	}
	fclose(fin);
}
