#include "transmitter.h"

extern const uint size;
extern struct Module moduleStruct;

void moduleCoding(ulong *message, FullCubePolynomial *publicKey, ulong *encodedMessage) {
	uint amount = AMOUNT_OF_MEMBERS, var;
	ulong bufMult = 0;
	ulong bufferPublicKey = 0;

	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		encodedMessage[i] = 0;
		for (int j = 0; j < MAX_TERMS_IN_KEY; j++) {
			if (publicKey[i].factor[j] == 0) {
				continue;
			}
			bufferPublicKey = publicKey[i].factor[j];
			for (uint h = 0; h < 3; h++) {
				var = getFromVar_test(publicKey[i].vars, (3 * j) + h);
				if (var == 0) {
					continue;
				}
				bufferPublicKey = modularMult(bufferPublicKey, message[var - 1]);
			}
			encodedMessage[i] = modularAdd(bufferPublicKey, encodedMessage[i]);
		}
	}
}
