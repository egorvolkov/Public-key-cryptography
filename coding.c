#include "transmitter.h"

extern const uint size;

void coding(ulong *message, FullCubePolynomial *publicKey, uchar *encodedMessage) {
	uint amount = AMOUNT_OF_MEMBERS, var;
	uchar bufMult[SIZE_OF_LONG_NUMBER];
	uchar bufferPublicKey[SIZE_OF_LONG_NUMBER];

	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		longToZeroEncoded(encodedMessage + i * LENGTH_OF_ENCODED_NUMBER);
		int a = MAX_TERMS_IN_KEY;
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
			//if (publicKey[i].factor[j] == 0) {
			//	continue;
			//}
			longGetNumber(publicKey[i].factor[j], bufferPublicKey);
			for (uint h = 0; h < 3; h++) {
				var = getFromVar_test(publicKey[i].vars, (3 * j) + h);
				if (var == 0) {
					continue;
				}
				//longDeg(message[var - 1], publicKey[(size + h) * amount + j], bufMult);
				longGetNumber(message[var - 1], bufMult);
				longMult(bufferPublicKey, bufMult, bufferPublicKey);
			}
			longAddEncoded(bufferPublicKey, encodedMessage + i * LENGTH_OF_ENCODED_NUMBER, encodedMessage + i * LENGTH_OF_ENCODED_NUMBER);
		}
	}
}
