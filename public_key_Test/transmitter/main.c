#include "transmitter.h"

#ifdef DEBUG
void printMatrix(ulong *matrix, uchar size1, uchar size2);
#endif
/**
 *	Main block
 */
const uint size = AMOUNT_OF_VARIABLES;

int main(int argc, char* argv[]) {
	FullCubePolynomial publicKey[size];	//	Correct Factor! It's ineffective memory using
	uchar encodedMessage[size * LENGTH_OF_ENCODED_NUMBER];
	ulong message[size];
	clock_t timer;
	ulong realtime;
	uint count = 0;
	while (1) {
		count++;
		getPublicKey(publicKey);

		createMessage(message);
//#ifdef PRINT
		// printf("\n");
		//printFullCubePolynomials(publicKey);
		// printf("\n");
		printf("Message: ");
		for (int i = 0; i < size; i++) {
			printf("%llu ", message[i]);
		}
		printf("\n");
//#endif
		startTime(&timer, &realtime);
		coding(message, publicKey, encodedMessage);
		endTime(&timer, &realtime);
#ifdef DEBUG
		printf("bytes: %d\n", returnEncodedMessage(encodedMessage));
#endif
#ifndef DEBUG
		returnEncodedMessage(encodedMessage);
#endif
        printTimes(timer,realtime);
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
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
			if (cubePolynomials[i].factor[j] == 0) {
				break;
			}
			if (j != 0) {
				printf(" + ");
			}
			printf("%llu", cubePolynomials[i].factor[j]);
			for (int k = 0; k < 5; k++) {
				printf("*x%u", getFromVar(cubePolynomials[i].vars[j], k));
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