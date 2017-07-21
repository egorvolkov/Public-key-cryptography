#include "transmitter.h"

#ifdef DEBUG
void printMatrix(ulong *matrix, uchar size1, uchar size2);
#endif
/**
 *	Main block
 */
const uint size = AMOUNT_OF_VARIABLES;

int main(int argc, char* argv[]) {
	FullCubePolynomial publicKey[AMOUNT_OF_VARIABLES];	//	Correct Factor! It's ineffective memory using
	uchar encodedMessage[AMOUNT_OF_VARIABLES * LENGTH_OF_ENCODED_NUMBER];
	ulong message[AMOUNT_OF_VARIABLES];
#ifdef TIME
	clock_t tStart, tEnd, middleTimeOfCoding = 0;
	ulong msec = 0;
	//double tStartRDTSC, tEndRDTSC, middleTimeOfCodingRDTSC = 0;
#endif
	uint count = 0;
	while (1) {
		count++;
		getPublicKey(publicKey);

		createMessage(message);
#ifdef PRINT
		printf("\n");
		printFullCubePolynomials(publicKey);
		printf("\n");
		printf("Message: ");
		for (int i = 0; i < AMOUNT_OF_VARIABLES; i++) {
			printf("%llu ", message[i]);
		}
		printf("\n");
#endif
#ifdef TIME
		tStart = clock();
		//tStartRDTSC = timeRDTSC();
#endif
		coding(message, publicKey, encodedMessage);
#ifdef TIME
		msec = (clock() - tStart) * 1000 / CLOCKS_PER_SEC;
		//tEndRDTSC = timeRDTSC();
		printf("Time of coding: %llu seconds %llu milliseconds\n", msec/1000, msec%1000);
		middleTimeOfCoding += clock() - tStart;
		//middleTimeOfCodingRDTSC += tEndRDTSC - tStartRDTSC;
#endif
#ifdef DEBUG
		printf("bytes: %d\n", returnEncodedMessage(encodedMessage));
#endif
#ifndef DEBUG
		returnEncodedMessage(encodedMessage);
#endif
#ifdef TIME
		printf("Middle time:\n\
			Time of coding:  %lu seconds %lu milliseconds\n", middleTimeOfCoding / count * 1000 / CLOCKS_PER_SEC / 1000, (middleTimeOfCoding / count * 1000 / CLOCKS_PER_SEC) % 1000);//middleTimeOfCodingRDTSC / count);
#endif
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
#endif


uint getFromVar(uint var, uint pos) {
	uint g = (1 << SIZE_OF_AVAR) - 1;
	return (var >> (32 - SIZE_OF_AVAR - SIZE_OF_AVAR * pos)) & g;
}
