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
	clock_t tStart, tEnd, timeOfSecretKey = 0, timeOfPublicKey = 0, timeOfDecoding = 0;
	ulong msec = 0;
	//double tStartRDTSC, tEndRDTSC, timeOfSecretKeyRDTSC = 0, timeOfPublicKeyRDTSC = 0, timeOfDecodingRDTSC = 0;
#endif

	FILE *file = NULL;
	struct Matrices matrices;
	FullCubePolynomial publicKey[AMOUNT_OF_POLYNOMS];
	ulong encodedMessage[AMOUNT_OF_POLYNOMS];
	ulong realMessage[AMOUNT_OF_VARIABLES] = {0};
	ulong radixes[NUMBER_OF_RADIX * 3] = {2, 3, 5, 7, 11, 13, 17, 19};
	ulong answers[AMOUNT_OF_VARIABLES * NUMBER_OF_RADIX];
	ulong secretVector[LENGTH_OF_SECRET_VECTOR];
	ulong transposition[AMOUNT_OF_VARIABLES];
	for (int loop = 0; loop < amountOfLoopIterations; loop++) {
		ulong radix = 1;
	generatePows(answers, radixes, AMOUNT_OF_VARIABLES - 1);
	for (int i = 0; i < NUMBER_OF_RADIX; i++){
		radix *= radixes[i];
	}
	computeRadixes(radixes,radix);
#ifdef PRINT
		output = fopen(PATH_TO_OUTPUT, "w");
#endif
		//moduleStruct.module = 1;
#ifdef TIME
		tStart = clock();
		//tStartRDTSC = timeRDTSC();
#endif
		generateSecretKey(&matrices, secretVector, answers, transposition, radix); //////////////////////////////////////////////////////
		printf("Module: %llu\n", moduleStruct.module);
#ifdef TIME
		msec = (clock() - tStart) * 1000 / CLOCKS_PER_SEC;
		//tEndRDTSC = timeRDTSC();
		printf("Time of generation a secret key: %llu seconds %llu milliseconds\n", msec/1000, msec%1000);
		timeOfSecretKey += clock() - tStart;
		//timeOfSecretKeyRDTSC += tEndRDTSC - tStartRDTSC;
#endif
#ifdef PRINT
		fprintf(output, "SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE); printf("SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);

		fprintf(output, "Module: %llu\nParts of module: ", moduleStruct.module); printf("Module: %llu\nParts of module: ", moduleStruct.module);

		for (int i = 0; i < moduleStruct.masSize; i++) {
			fprintf(output, "%llu ", moduleStruct.partsOfModule[i]); printf("%llu ", moduleStruct.partsOfModule[i]);
		}
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix A\n"); printf("Matrix A\n");
		fPrintMatrix(matrices.firstMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS); printMatrix(matrices.firstMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS);
		fprintf(output, "\n"); printf("\n");

		// fprintf(output, "Matrix A^(-1)\n"); printf("Matrix A^(-1)\n");
		// fPrintMatrix(matrices.firstInverseMatrix, size, size); printMatrix(matrices.firstInverseMatrix, size, size);
		// fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix B\n"); printf("Matrix B\n");
		fPrintMatrix(matrices.secondMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS); printMatrix(matrices.secondMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS);
		fprintf(output, "\n"); printf("\n");

		fprintf(output, "Matrix B^(-1)\n"); printf("Matrix B^(-1)\n");
		fPrintMatrix(matrices.secondInverseMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS); printMatrix(matrices.secondInverseMatrix, AMOUNT_OF_POLYNOMS, AMOUNT_OF_POLYNOMS);
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		tStart = clock();
		//tStartRDTSC = timeRDTSC();
#endif
		computePublicKey(matrices.firstMatrix, matrices.secondMatrix, publicKey, matrices.constants, matrices.firstMatrixDet, answers, transposition); ///////////////////////////////////////////
#ifdef TIME
		msec = (clock() - tStart) * 1000 / CLOCKS_PER_SEC;
		//tEndRDTSC = timeRDTSC();
		printf("Time of computing a public key:  %llu seconds %llu milliseconds\n", msec/1000, msec%1000);
		timeOfPublicKey += clock() - tStart;
		//timeOfPublicKeyRDTSC += tEndRDTSC - tStartRDTSC;
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

		FILE* fin;
		fin = fopen(PATH_TO_PUBLIC_KEY, "rb");
		if (!fin) {
			printf("Can't open the file for reading\n");
			return 0;
		}
		fread(encodedMessage, 8, AMOUNT_OF_POLYNOMS, fin);
		fclose(fin);
		//getEncodedMessage(encodedMessage);
#ifdef PRINT
		fprintf(output, "Encoded message by module: "); printf("Encoded message by module: ");
		for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
			fprintf(output, "%llu ", encodedMessage[i]); printf("%llu ", encodedMessage[i]);
		}
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		tStart = clock();
		//tStartRDTSC = timeRDTSC();
#endif
		decoding(matrices.firstInverseMatrix, matrices.secondInverseMatrix, encodedMessage,realMessage, matrices.constants, secretVector, radixes, transposition, radix, matrices.firstMatrixDet);
#ifdef TIME
		msec = (clock() - tStart) * 1000 / CLOCKS_PER_SEC;
		//tEndRDTSC = timeRDTSC();
		printf("Time of decoding: %llu seconds %llu milliseconds\n", msec/1000, msec%1000);
		timeOfDecoding += clock() - tStart;
		//timeOfDecodingRDTSC += tEndRDTSC - tStartRDTSC;
#endif
#ifdef PRINT
		fprintf(output, "Message: "); printf("Message: ");
		for (int i = 0; i < AMOUNT_OF_VARIABLES; i++) {
			fprintf(output, "%llu ", realMessage[i]); printf("%llu ", realMessage[i]);
		}
		fprintf(output, "\n"); printf("\n");
#endif
#ifdef TIME
		printf("\nMiddle time (iteration %u)\n\
			Time of generation a secret key: %lu seconds %lu milliseconds\n\
			Time of computing a public key: %lu seconds %lu milliseconds\n\
			Time of decoding: %lu seconds %lu milliseconds\n",\
		       loop + 1,\
		       timeOfSecretKey / (loop + 1) * 1000 / CLOCKS_PER_SEC / 1000, (timeOfSecretKey / (loop + 1) * 1000 / CLOCKS_PER_SEC) % 1000, \
		       timeOfPublicKey / (loop + 1) * 1000 / CLOCKS_PER_SEC /1000, (timeOfPublicKey / (loop + 1) * 1000 / CLOCKS_PER_SEC) % 1000, \
		       timeOfDecoding / (loop + 1) * 1000 / CLOCKS_PER_SEC / 1000, (timeOfDecoding / (loop + 1) * 1000 / CLOCKS_PER_SEC) % 1000);
#endif
		file = fopen(PATH_TO_MESSAGE, "r");
		for (int i = 0; i < AMOUNT_OF_VARIABLES; i++) {
			ulong buf;
			fscanf(file, "%llu", &buf);
			if (buf != realMessage[i]) {
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
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
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
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		for (int j = 0; j < MAX_TERMS_IN_CUBE; j++) {
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

void printFullCubePolynomials(FullCubePolynomial *cubePolynomials) {
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
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
void fPrintFullCubePolynomials(FullCubePolynomial *cubePolynomials) {
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
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
#endif
