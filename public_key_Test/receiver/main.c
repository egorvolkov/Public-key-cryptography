#include "receiver.h"

/**
 *	Main block
 */
const uint size = AMOUNT_OF_VARIABLES;
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
	double tStart, tEnd, timeOfSecretKey = 0, timeOfPublicKey = 0, timeOfDecoding = 0;
	double tStartRDTSC, tEndRDTSC, timeOfSecretKeyRDTSC = 0, timeOfPublicKeyRDTSC = 0, timeOfDecodingRDTSC = 0;
#endif

	FILE *file = NULL;
	struct NewMatrices newMatrices;
	FullCubePolynomial publicKey[size];
	ulong encodedOrRealMessage[size];

	for (int loop = 0; loop < amountOfLoopIterations; loop++) {
#ifdef PRINT
		output = fopen(PATH_TO_OUTPUT, "w");
#endif
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
		generateSecretKey(&newMatrices); //////////////////////////////////////////////////////
#ifdef TIME

		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of generation a secret key: %f ms\n", tEnd - tStart);
		timeOfSecretKey += tEnd - tStart;
		timeOfSecretKeyRDTSC += tEndRDTSC - tStartRDTSC;
#endif
#ifdef PRINT
		fprintf(output, "SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE); printf("SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);

		fprintf(output, "Module: %llu\nParts of module: ", moduleStruct.module); printf("Module: %llu\nParts of module: ", moduleStruct.module);

		for (int i = 0; i < moduleStruct.masSize; i++) {
			fprintf(output, "%llu ", moduleStruct.partsOfModule[i]); printf("%llu ", moduleStruct.partsOfModule[i]);
		}
		fprintf(output, "\n"); printf("\n");

        printf("New Matrix A\n");
        printNewMatrix(newMatrices.firstMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
        printf("\n");

        printf("New Matrix A^(-1)\n");
        printNewMatrix(newMatrices.firstInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
        printf("\n");

        printf("New Matrix B\n");
        printNewMatrix(newMatrices.secondMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
        printf("\n");

        printf("New Matrix B^(-1)\n");
        printNewMatrix(newMatrices.secondInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
        printf("\n");

#endif
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
        computeNewPublicKey(newMatrices.firstMatrix, newMatrices.secondMatrix, publicKey, newMatrices.constants, newMatrices.constants3);
#ifdef TIME
		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of computing a public key: %f ms\n", tEnd - tStart);
		timeOfPublicKey += tEnd - tStart;
		timeOfPublicKeyRDTSC += tEndRDTSC - tStartRDTSC;
#endif
#ifdef PRINT
		fprintf(output, "Public key\n"); printf("Public key\n");
        fPrintFullCubePolynomials(publicKey); printFullCubePolynomials(publicKey);
        fprintf(output, "\n"); printf("\n");

        printf("Bytes written: %d\n", returnPublicKey(publicKey));
#endif
#ifndef PRINT
		returnPublicKey(publicKey); ///////////////////////////////////////
#endif
		transmitterConnection();

		getEncodedMessage(encodedOrRealMessage);
#ifdef PRINT
		fprintf(output, "Encoded message by module: "); printf("Encoded message by module: ");
		for (int i = 0; i < size; i++) {
			fprintf(output, "%llu ", encodedOrRealMessage[i]); printf("%llu ", encodedOrRealMessage[i]);
		}
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
        newDecoding(newMatrices.firstInverseMatrix, newMatrices.secondInverseMatrix, encodedOrRealMessage, newMatrices.constants, newMatrices.constants3);
#ifdef TIME
		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of decoding: %f ms\n", tEnd - tStart);
		timeOfDecoding += tEnd - tStart;
		timeOfDecodingRDTSC += tEndRDTSC - tStartRDTSC;
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
			Time of generation a secret key: %f ms; %f Hz\n\
			Time of computing a public key: %f ms; %f Hz\n\
			Time of decoding: %f ms; %f Hz\n",\
		       loop + 1,\
		       timeOfSecretKey / (loop + 1), timeOfSecretKeyRDTSC / (loop + 1),\
		       timeOfPublicKey / (loop + 1), timeOfPublicKeyRDTSC / (loop + 1),\
		       timeOfDecoding / (loop + 1), timeOfDecodingRDTSC / (loop + 1));
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
#endif
		printf("\n____________________________________\n");
		if ((loop + 1) % pause == 0) {
			printf("PAUSE (Press Enter to continue)\n");
			getchar();
		}
	}
	return 0;
}

#ifdef PRINT
void printMatrix(ulong *matrix, uint size1, uint size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			printf("%llu ", matrix[i * size2 + j]);
		}
		printf("\n");
	}
}
void fPrintMatrix(ulong *matrix, uint size1, uint size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			fprintf(output, "%llu ", matrix[i * size2 + j]);
		}
		fprintf(output, "\n");
	}
}

void printCubePolynomials(CubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < MAX_TERMS_IN_CUBE; j++) {
			if (cubePolynomials[i].factor[j] == 0) {
				break;
			}
			if (j != 0) {
				printf(" + ");
			}
			printf("%llu", cubePolynomials[i].factor[j]);
			for (int k = 0; k < 3; k++) {
				printf("*x%u", getFromVar_test(cubePolynomials[i].vars, (3 * j) + k));
			}
		}
		printf("\n");
	}
}
void fPrintCubePolynomials(CubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < MAX_TERMS_IN_CUBE; j++) {
			if (cubePolynomials[i].factor[j] == 0) {
				break;
			}
			if (j != 0) {
				fprintf(output, " + ");
			}
			fprintf(output, "%llu", cubePolynomials[i].factor[j]);
			for (int k = 0; k < 3; k++) {
				fprintf(output, "*x%lu", getFromVar_test(cubePolynomials[i].vars, (3 * j) + k));
			}
		}
		fprintf(output, "\n");
	}
}

void printFullCubePolynomials(FullCubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
			if (cubePolynomials[i].factor[j] == 0) {
				break;
			}
			if (j != 0) {
				printf(" + ");
			}
			printf("%llu", cubePolynomials[i].factor[j]);
			for (int k = 0; k < 3; k++) {
				printf("*x%lu", getFromVar_test(cubePolynomials[i].vars, (3 * j) + k));
			}
		}
		printf("\n");
	}
}
void fPrintFullCubePolynomials(FullCubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
			if (cubePolynomials[i].factor[j] == 0) {
				break;
			}
			if (j != 0) {
				fprintf(output, " + ");
			}
			fprintf(output, "%llu", cubePolynomials[i].factor[j]);
			for (int k = 0; k < 3; k++) {
				fprintf(output, "*x%u", getFromVar_test(cubePolynomials[i].vars, (3 * j) + k));
			}
		}
		fprintf(output, "\n");
	}
}

void printNewMatrix(ulong* matrix, int size1, int size2, int amount) {
	int was_printed;
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			was_printed = 0;
			for (int k = 0; k < amount; k++) {
				if (matrix[i*2*amount+2*k] == j) {
					printf("%u ", matrix[i*2*amount+2*k + 1]);
					was_printed = 1;
					break;
				}
			}
			if (!was_printed) {
				printf("0 ");
			}
		}
		printf("\n");
	}
}

#endif
