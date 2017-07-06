#include "transmitter.h"

extern const uchar size;

void coding(ulong *message, ulong *publicKey, uchar *encodedMessage) {
	uint amount = AMOUNT_OF_MEMBERS;
	uchar bufferDeg[SIZE_OF_LONG_NUMBER];
	uchar bufferPublicKey[SIZE_OF_LONG_NUMBER];

	for (int i = 0; i < size; i++) {
		longToZeroEncoded(encodedMessage + i * LENGTH_OF_ENCODED_NUMBER);
		for (int j = 0; j < amount; j++) {
			longGetNumber(publicKey[i * amount + j], bufferPublicKey);
			for (int h = 0; h < size; h++) {
				if (publicKey[(size + h) * amount + j] == 0) {
					continue;
				}
				longDeg(message[h], publicKey[(size + h) * amount + j], bufferDeg);
				longMult(bufferPublicKey, bufferDeg, bufferPublicKey);
			}
			longAddEncoded(bufferPublicKey, encodedMessage + i * LENGTH_OF_ENCODED_NUMBER, encodedMessage + i * LENGTH_OF_ENCODED_NUMBER);
		}
	}
}