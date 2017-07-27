#include "receiver.h"

/**
 *	Main block
 */
const uint size = AMOUNT_OF_VARIABLES;
struct Module moduleStruct = {1, MAS_SIZE};

int main(int argc, char* argv[]) {
	int amountOfLoopIterations = AMOUNT_OF_LOOP_ITERATIONS;
	int pause = PAUSE;
	uint bytes;
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

	clock_t timer[3];
    ulong realtime[3];

	FILE *file = NULL;
	struct Matrices matrices;
	CubePolynomial publicKey[size];
	ulong encodedOrRealMessage[size];

	for (int loop = 0; loop < amountOfLoopIterations; loop++) {

        startTime(timer, realtime);
		generateSecretKey(&matrices); //////////////////////////////////////////////////////
        endTime(timer, realtime);

		printSecretKey(matrices);

        startTime(timer + 1, realtime + 1);
        computePublicKey(matrices.firstMatrix, matrices.secondMatrix, matrices.secondInverseMatrix, publicKey, matrices.constants, matrices.constants3);
        endTime(timer + 1, realtime + 1);

		bytes = returnPublicKey(publicKey);

		printPublicKey(publicKey, bytes);
		
		transmitterConnection();

		getEncodedMessage(encodedOrRealMessage);

		printEncodedMessage(encodedOrRealMessage);

        startTime(timer + 2, realtime + 2);
        decoding(matrices.firstInverseMatrix, matrices.secondInverseMatrix, encodedOrRealMessage, matrices.constants, matrices.constants3);
        endTime(timer + 2, realtime + 2);

		printDecodedMessage(encodedOrRealMessage);
        printTimes(timer, realtime);

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

		printf("\n____________________________________\n");

		if ((loop + 1) % pause == 0) {
			printf("PAUSE (Press Enter to continue)\n");
			getchar();
		}
	}
	return 0;
}