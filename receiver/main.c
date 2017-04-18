#include "receiver.h"

/**
 *	Main block
 */
const uchar size = AMOUNT_OF_VARIABLES;
struct Module moduleStruct = {1, MAS_SIZE};

#ifdef PRINT
FILE *output;
#endif

int main(int argc, char* argv[]) {
	int amountOfLoopIterations = AMOUNT_OF_LOOP_ITERATIONS;
	int pause = PAUSE;
	if (argc > 1) {
		amountOfLoopIterations = 0;
		int i = 0;
		while (argv[1][i] != '\0') {
			amountOfLoopIterations = amountOfLoopIterations * 10 + argv[1][i] - '0';
			i++;
		}
		if (argc > 2) {
			pause = 0;
			int j = 0;
			while (argv[2][j] != '\0') {
				pause = pause * 10 + argv[2][j] - '0';
				j++;
			}
		}
	}

#ifdef TIME
	double tStart, tEnd, timeOfSecretKey = 0, timeOfPublicKey = 0, timeOfGettingMessage = 0, timeOfDecoding = 0;
#endif

	FILE *file = NULL;
	struct Matrices matrices;
	ulong publicKey[size * AMOUNT_OF_MEMBERS];
	ulong encodedOrRealMessage[size];

	for (int loop = 0; loop < amountOfLoopIterations; loop++) {
#ifdef PRINT
		output = fopen(PATH_TO_OUTPUT, "w");
#endif
		//moduleStruct.module = 1;
#ifdef TIME
		tStart = getTime();
#endif
		generateSecretKey(&matrices);
#ifdef TIME
		tEnd = getTime();
		printf("Time of generation a secret key: %f ms\n", tEnd - tStart);
		timeOfSecretKey += tEnd - tStart;
#endif
#ifdef PRINT
		fprintf(output, "SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE); printf("SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);

		fprintf(output, "Module: %llu\nParts of module: ", moduleStruct.module); printf("Module: %llu\nParts of module: ", moduleStruct.module);

		for (int i = 0; i < moduleStruct.masSize; i++) {
			fprintf(output, "%llu ", moduleStruct.partsOfModule[i]); printf("%llu ", moduleStruct.partsOfModule[i]);
		}
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix A\n"); printf("Matrix A\n");
		fPrintMatrix(matrices.firstMatrix, size, size); printMatrix(matrices.firstMatrix, size, size);
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix A^(-1)\n"); printf("Matrix A^(-1)\n");
		fPrintMatrix(matrices.firstInverseMatrix, size, size); printMatrix(matrices.firstInverseMatrix, size, size);
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix B\n"); printf("Matrix B\n");
		fPrintMatrix(matrices.secondMatrix, size, size); printMatrix(matrices.secondMatrix, size, size);
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix B^(-1)\n"); printf("Matrix B^(-1)\n");
		fPrintMatrix(matrices.secondInverseMatrix, size, size); printMatrix(matrices.secondInverseMatrix, size, size);
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		tStart = getTime();
#endif
		computePublicKey(matrices.firstMatrix, matrices.secondMatrix, publicKey);
#ifdef TIME
		tEnd = getTime();
		printf("Time of computing a public key: %f ms\n", tEnd - tStart);
		timeOfPublicKey += tEnd - tStart;
#endif
#ifdef PRINT
		fprintf(output, "Public key\n"); printf("Public key\n");
		fPrintMatrix(publicKey, size, AMOUNT_OF_MEMBERS); printMatrix(publicKey, size, AMOUNT_OF_MEMBERS);
		fprintf(output, "\n"); printf("\n");

		printf("Bytes written: %d\n", returnPublicKey(publicKey));
#endif
#ifndef PRINT
		returnPublicKey(publicKey);
#endif
		transmitterConnection();
#ifdef TIME
		tStart = getTime();
#endif
		getEncodedMessage(encodedOrRealMessage);
#ifdef TIME
		tEnd = getTime();
		printf("Time of getting message: %f ms\n", tEnd - tStart);
		timeOfGettingMessage += tEnd - tStart;
#endif
#ifdef PRINT
		fprintf(output, "Encoded message by module: "); printf("Encoded message by module: ");
		for (int i = 0; i < size; i++) {
			fprintf(output, "%llu ", encodedOrRealMessage[i]); printf("%llu ", encodedOrRealMessage[i]);
		}
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		tStart = getTime();
#endif
		decoding(matrices.firstInverseMatrix, matrices.secondInverseMatrix, encodedOrRealMessage);
#ifdef TIME
		tEnd = getTime();
		printf("Time of decoding: %f ms\n", tEnd - tStart);
		timeOfDecoding += tEnd - tStart;
#endif
#ifdef PRINT
		fprintf(output, "Message: "); printf("Message: ");
		for (int i = 0; i < size; i++) {
			fprintf(output, "%llu ", encodedOrRealMessage[i]); printf("%llu ", encodedOrRealMessage[i]);
		}
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		printf("\nMiddle time (iteration %u)\n\
			Time of generation a secret key: %f ms\n\
			Time of computing a public key: %f ms\n\
			Time of getting message: %f ms\n\
			Time of decoding: %f ms\n", loop + 1, timeOfSecretKey / (loop + 1), timeOfPublicKey / (loop + 1), timeOfGettingMessage / (loop + 1), timeOfDecoding / (loop + 1));
#endif
		file = fopen(PATH_TO_MESSAGE, "r");
		for (int i = 0; i < size; i++) {
			ulong buf;
			fscanf(file, "%llu", &buf);
			if (buf != encodedOrRealMessage[i]) {
				printf("ERROR!!! Maybe this is error of connection between transmitter and receiver.\n");
				getchar();
				break;
			}
		}
		fclose(file);
#ifdef PRINT
		fclose(output);
		printf("\n____________________________________\n");
#endif
		if ((loop + 1) % pause == 0) {
			printf("PAUSE (Press Enter to continue)\n");
			getchar();
		}
	}
	return 0;
}

#ifdef PRINT
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
