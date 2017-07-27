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
	printf("Encoded message:\n");
	fprintf(output, "Encoded message:\n"); 
#endif
	int amount;
	for (int i = 0; i < size; i++) {
		encodedOrRealMessage[i] = 0;
		fread(&amount, 1, 4, fin);
		for (int j = 0; j < amount; j++) {
			fread(&bufChar, 1, 1, fin);
#ifdef PRINT
			for (int k = 0; k < 8; k++) {
				printf("%d", (bufChar >> (7 - k)) & 1);
				fprintf(output, "%d", (bufChar >> (7 - k)) & 1); 
			}
			printf(" ");
			fprintf(output, " "); 
#endif
			buf = bufChar;
			encodedOrRealMessage[i] = modularAdd(modularMult(encodedOrRealMessage[i], (ulong)256), buf);
		}
#ifdef PRINT
		printf("\n");
		fprintf(output, "\n"); 
#endif
	}
#ifdef PRINT
	printf("\n");
	fprintf(output, "\n"); 
#endif
	fclose(fin);
}