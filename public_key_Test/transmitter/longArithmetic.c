#include "transmitter.h"

extern const uint size;

void longAdd(uchar *a, uchar *b, uchar *result) {
	int overflow = 0;
	int bufA, bufB;
	for (int i = SIZE_OF_LONG_NUMBER - 1; i >= 0; i--) {
		bufA = a[i];
		bufB = b[i];
		result[i] = (bufA + bufB + overflow) & 255;
		overflow = (bufA + bufB + overflow) / 256;
	}
}

void longAddEncoded(uchar *a, uchar *b, uchar *result) {
	int overflow = 0;
	int bufA, bufB;
	for (int i = SIZE_OF_LONG_NUMBER - 1, j = LENGTH_OF_ENCODED_NUMBER - 1; j >= 0; i--, j--) {
		bufA = a[i];
		bufB = b[j];
		if (i < 0) {
			if (overflow == 0) {
				break;
			}
			result[j] = (bufB + overflow) & 255;
			overflow = (bufB + overflow) / 256;
			continue;
		}
		result[j] = (bufA + bufB + overflow) & 255;
		overflow = (bufA + bufB + overflow) / 256;
	}
}

void longMult(uchar *a, uchar *b, uchar *result) {
	uchar bufResult[SIZE_OF_LONG_NUMBER];
	uchar bufAdd[SIZE_OF_LONG_NUMBER];
	//longToZero(bufResult);
	memset(bufResult, 0, SIZE_OF_LONG_NUMBER);
	for (int i = SIZE_OF_LONG_NUMBER - 1; i >= 0; i--) {
		if (b[i] == 0) {
			continue;
		}
		for (int j = 0; j < i + 1; j++) {
			bufAdd[j] = a[SIZE_OF_LONG_NUMBER - 1 - i + j];
		}
		for (int j = i + 1; j < SIZE_OF_LONG_NUMBER; j++) {
			bufAdd[j] = 0;
		}
		longMultToChar(bufAdd, b[i], bufAdd);
		longAdd(bufResult, bufAdd, bufResult);
	}
	memmove(result, bufResult, SIZE_OF_LONG_NUMBER);
	// for (int i = 0; i < SIZE_OF_LONG_NUMBER; i++) {
	// 	result[i] = bufResult[i];
	// }
}

void longMultToChar(uchar *a, uchar b, uchar *result) {
	uchar bufResult[SIZE_OF_LONG_NUMBER];
	uchar overflow = 0;
	for (int i = SIZE_OF_LONG_NUMBER - 1; i >= 0; i--) {
		bufResult[i] = (a[i] * b + overflow) & 255;
		overflow = (a[i] * b + overflow) / 256;
	}
	memmove(result, bufResult, SIZE_OF_LONG_NUMBER);
	// for (int i = 0; i < SIZE_OF_LONG_NUMBER; i++) {
	// 	result[i] = bufResult[i];
	// }
}

void longDeg(ulong a, uchar b, uchar *result) {
	if (b == 0) {
		//longToZero(result);
		memset(result, 0, SIZE_OF_LONG_NUMBER);
		result[SIZE_OF_LONG_NUMBER - 1] = 1;
		return;
	}
	uchar bufA[SIZE_OF_LONG_NUMBER];
	longGetNumber(a, result);
	longGetNumber(a, bufA);
	for (int i = 1; i < b; i++) {
		longMult(result, bufA, result);
	}
}

void longPrintNumber(uchar *number, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%d", (number[i] >> (7 - j)) & 1);
		}
		printf(" ");
	}
	printf("\n");
}

void longGetNumber(ulong number, uchar *longNumber) {
	// for (int i = 0; i < SIZE_OF_LONG_NUMBER; i++) {
	// 	longNumber[i] = 0;
	// }
	memset(longNumber, 0, SIZE_OF_LONG_NUMBER);
	int i = SIZE_OF_LONG_NUMBER - 1;
	while (number != 0) {
		longNumber[i--] = number & 255;
		number >>= 8;
	}
}