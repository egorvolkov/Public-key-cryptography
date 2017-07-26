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

#ifdef TIME
	double tStart, tEnd, timeOfSecretKey = 0, timeOfPublicKey = 0, timeOfDecoding = 0;
	double tStartRDTSC, tEndRDTSC, timeOfSecretKeyRDTSC = 0, timeOfPublicKeyRDTSC = 0, timeOfDecodingRDTSC = 0;
#endif

	FILE *file = NULL;
	struct Matrices matrices;
	CubePolynomial publicKey[size];
	ulong encodedOrRealMessage[size];

	for (int loop = 0; loop < amountOfLoopIterations; loop++) {
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
		generateSecretKey(&matrices); //////////////////////////////////////////////////////
#ifdef TIME
		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of generation a secret key: %f ms\n", tEnd - tStart);
		timeOfSecretKey += tEnd - tStart;
		timeOfSecretKeyRDTSC += tEndRDTSC - tStartRDTSC;
#endif
		printSecretKey(matrices);
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
        computePublicKey(matrices.firstMatrix, matrices.secondMatrix, matrices.secondInverseMatrix, publicKey, matrices.constants, matrices.constants3);
#ifdef TIME
		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of computing a public key: %f ms\n", tEnd - tStart);
		timeOfPublicKey += tEnd - tStart;
		timeOfPublicKeyRDTSC += tEndRDTSC - tStartRDTSC;
#endif

		bytes = returnPublicKey(publicKey);

		printPublicKey(publicKey, bytes);
		
		transmitterConnection();

		getEncodedMessage(encodedOrRealMessage);

		printEncodedMessage(encodedOrRealMessage);
#ifdef TIME
		tStart = getTime();
		tStartRDTSC = timeRDTSC();
#endif
        decoding(matrices.firstInverseMatrix, matrices.secondInverseMatrix, encodedOrRealMessage, matrices.constants, matrices.constants3);
#ifdef TIME
		tEnd = getTime();
		tEndRDTSC = timeRDTSC();
		printf("Time of decoding: %f ms\n", tEnd - tStart);
		timeOfDecoding += tEnd - tStart;
		timeOfDecodingRDTSC += tEndRDTSC - tStartRDTSC;
#endif		
		printDecodedMessage(encodedOrRealMessage);
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

		printf("\n____________________________________\n");

		if ((loop + 1) % pause == 0) {
			printf("PAUSE (Press Enter to continue)\n");
			getchar();
		}
	}
	return 0;
}