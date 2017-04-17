#include "receiver.h"

/**
 *	Main block
 */
const uchar size = AMOUNT_OF_VARIABLES;
struct Module moduleStruct = {1, MAS_SIZE};

#ifdef DEBUG
FILE *output;
#endif

int main(int argc, char* argv[]) {

	int pause = PAUSE;
	if (argc > 1) {
		pause = 0;
		int i = 0;
		while (argv[1][i] != '\0') {
			pause = pause*10 + argv[1][i] - '0';
			i++;
		}
	}


	struct Matrices matrices;
	ulong publicKey[size * AMOUNT_OF_MEMBERS];
	ulong encodedOrRealMessage[size];
#ifdef DEBUG
	double tStart, tEnd, timeOfSecretKey = 0, timeOfPublicKey = 0, timeOfGettingMessage = 0, timeOfDecoding = 0;
	FILE *file = NULL;
	ulong buf;
	uint count = 0;
	while (1) {
		output = fopen(PATH_TO_OUTPUT, "w");
		count++;
		moduleStruct.module = 1;
		tStart = getTime();
#endif
		generateSecretKey(&matrices);
#ifdef DEBUG
		printf("SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);
		fprintf(output, "SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);
		printf("Module: %llu\nParts of module: ", moduleStruct.module);
		fprintf(output, "Module: %llu\nParts of module: ", moduleStruct.module);
		for (int i = 0; i < moduleStruct.masSize; i++) {
			printf("%llu ", moduleStruct.partsOfModule[i]);
			fprintf(output, "%llu ", moduleStruct.partsOfModule[i]);
		}
		fprintf(output, "\n");
		printf("\n");
		// fprintf(output, "Matrix A\n");
		// fPrintMatrix(matrices.firstMatrix, size, size);
		// fprintf(output, "\n");
		// fprintf(output, "Matrix A^(-1)\n");
		// fPrintMatrix(matrices.firstInverseMatrix, size, size);
		// fprintf(output, "\n");
		// fprintf(output, "Matrix B\n");
		// fPrintMatrix(matrices.secondMatrix, size, size);
		// fprintf(output, "\n");
		// fprintf(output, "Matrix B^(-1)\n");
		// fPrintMatrix(matrices.secondInverseMatrix, size, size);
		// fprintf(output, "\n");
		tEnd = getTime();
		printf("Time of generation a secret key: %f ms\n", tEnd - tStart);
		timeOfSecretKey += tEnd - tStart;

		printf("module: %llu\n", moduleStruct.module);
		tStart = getTime();
#endif
		computePublicKey(matrices.firstMatrix, matrices.secondMatrix, publicKey);
#ifdef DEBUG
		// fprintf(output, "Public key\n");
		// fPrintMatrix(publicKey, size, AMOUNT_OF_MEMBERS);
		// fprintf(output, "\n");
		// printf("\n");
		// printMatrix(publicKey, size, AMOUNT_OF_MEMBERS);
		// printf("\n");
		tEnd = getTime();
		printf("Time of computing a public key: %f ms\n", tEnd - tStart);
		timeOfPublicKey += tEnd - tStart;

		printf("Bytes written: %d\n", returnPublicKey(publicKey));
#endif
#ifndef DEBUG
		returnPublicKey(publicKey);
#endif
		transmitterConnection();
#ifdef DEBUG
		tStart = getTime();
#endif
		getEncodedMessage(encodedOrRealMessage);
#ifdef DEBUG
		tEnd = getTime();
		printf("Time of getting message: %f ms\n", tEnd - tStart);
		timeOfGettingMessage += tEnd - tStart;

		printf("Encoded: ");
		fprintf(output, "Encoded message by module: ");
		for (int i = 0; i < size; i++) {
			printf("%llu ", encodedOrRealMessage[i]);
			fprintf(output, "%llu ", encodedOrRealMessage[i]);
		}
		printf("\n");
		fprintf(output, "\n");

		tStart = getTime();
#endif
		decoding(matrices.firstInverseMatrix, matrices.secondInverseMatrix, encodedOrRealMessage);
#ifdef DEBUG
		tEnd = getTime();
		printf("Time of decoding: %f ms\n", tEnd - tStart);
		timeOfDecoding += tEnd - tStart;

		printf("Message: ");
		fprintf(output, "Message: ");
		for (int i = 0; i < size; i++) {
			printf("%llu ", encodedOrRealMessage[i]);
			fprintf(output, "%llu ", encodedOrRealMessage[i]);
		}

		printf("Middle time\n\
			Time of generation a secret key: %f ms\n\
			Time of computing a public key: %f ms\n\
			Time of getting message: %f ms\n\
			Time of decoding: %f ms\n\
			", timeOfSecretKey / count, timeOfPublicKey / count, timeOfGettingMessage / count, timeOfDecoding / count);

		fclose(output);

		file = fopen(PATH_TO_MESSAGE, "r");
		for (int i = 0; i < size; i++) {
			fscanf(file, "%llu", &buf);
			if (buf != encodedOrRealMessage[i]) {
				printf("ERROR!!!\n");
				getchar();
			}
		}
		fclose(file);

		printf("\n____________________________________\n");
		if (count % pause == 0) {
			printf("PAUSE\n");
			getchar();
		}
		//getchar();
	}
#endif
	return 0;
}

#ifdef DEBUG
void printMatrix(ulong *matrix, uchar size1, uchar size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			printf("%llu ", matrix[i * size2 + j]);
		}
		printf("\n");
	}
}
void fPrintMatrix(ulong *matrix, uchar size1, uchar size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			fprintf(output, "%llu ", matrix[i * size2 + j]);
		}
		fprintf(output, "\n");
	}
}
#endif
