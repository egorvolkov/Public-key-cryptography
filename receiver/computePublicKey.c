#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef DEBUG
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *publicKey) {
	ulong bufferMatrix[size * AMOUNT_OF_MEMBERS];
	cubeOfPolynomials(firstMatrix, bufferMatrix);
#ifdef DEBUG
	// fprintf(output, "Cube polynomials\n");
	// fPrintMatrix(bufferMatrix, size, AMOUNT_OF_MEMBERS);
	// fprintf(output, "\n");
#endif
	multToSecondMatrix(secondMatrix, bufferMatrix, publicKey);
}


void cubeOfPolynomials(ulong *matrix, ulong *bufferMatrix) {
	for (int i = 0; i < size; i++) {
		polynomialCube(matrix + i * size, bufferMatrix + i * AMOUNT_OF_MEMBERS);
	}
}

void polynomialCube(ulong *polyn, ulong *bufferMatrix) {
	uint cur = 0;

	for (int i = 0; i < size; i++) {
		bufferMatrix[cur++] = modularDeg(polyn[i], 3);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j)
				continue;
			bufferMatrix[cur++] = modularMult(3, modularMult(modularDeg(polyn[i], 2), polyn[j]));
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			for (int k = j + 1; k < size; k++) {
				bufferMatrix[cur++] = modularMult(6, modularMult(modularMult(polyn[i], polyn[j]), polyn[k]));
			}
		}
	}
}

void multToSecondMatrix(ulong *matrix, ulong *bufferMatrix, ulong *publicKey) {
	uint k, amount = AMOUNT_OF_MEMBERS;

	for (int i = 0; i < size * amount; i++) {
		publicKey[i] = 0;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < amount; k++) {
				publicKey[i * amount + k] = modularAdd(publicKey[i * amount + k], modularMult(matrix[i * size + j], bufferMatrix[j * amount + k]));
			}
		}
	}
}