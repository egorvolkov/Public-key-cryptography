#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

uint bitsForVar;

#ifdef PRINT
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, FullCubePolynomial *publicKey, ulong *constants) {
	bitsForVar = bitsForVariable();
	CubePolynomial bufferMatrix[size];
	cubeOfPolynomials(firstMatrix, bufferMatrix, constants);
#ifdef PRINT
	printf("Constants:\n");
	for(uint i = 0; i < size; i++){
		printf("%u ", constants[i]);
	}
	printf("\n\n");
	fprintf(output, "Cube polynomials\n"); printf("Cube polynomials\n");
	fPrintCubePolynomials(bufferMatrix); printCubePolynomials(bufferMatrix);
	fprintf(output, "\n"); printf("\n");
#endif
	multToSecondMatrix(secondMatrix, bufferMatrix, publicKey);
}


void cubeOfPolynomials(ulong *matrix, CubePolynomial bufferMatrix[], ulong *constants) {
	CubePolynomial *buf;
	ulong polinom[size + 1];
	for (uint i = 0; i < size; i++) {
		for (uint j = 0; j < size; j++){
			polinom[j] = matrix[j + i*size];
		}
		polinom[size] = constants[i];
		buf = &(bufferMatrix[i]);
		polynomialCube(polinom, buf);
	}
}

void polynomialCube(ulong *polyn, CubePolynomial *bufferMatrix) {
	uint cur = 0;
	ulong bufferFactor;

	for (uint i = 0; i < size; i++) {
		bufferFactor = modularDeg(polyn[i], 3);
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		bufferMatrix->vars[cur] = 0;
		for (uint j = 0; j < 3; j++) {
			writeToVar(bufferMatrix->vars + cur, i + 1, j);
		}
		cur++;
	}

	for (uint i = 0; i < size; i++) {
		for (uint j = 0; j < size; j++) {
			if (i == j)
				continue;
			bufferFactor = modularMult(3, modularMult(modularDeg(polyn[i], 2), polyn[j]));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			bufferMatrix->vars[cur] = 0;
			for (uint k = 0; k < 2; k++) {
				writeToVar(bufferMatrix->vars + cur, i + 1, k);
			}
			writeToVar(bufferMatrix->vars + cur, j + 1, 2);
			cur++;
		}
	}

	for (uint i = 0; i < size; i++) {
		for (uint j = i + 1; j < size; j++) {
			for (uint k = j + 1; k < size; k++) {
				bufferFactor = modularMult(6, modularMult(modularMult(polyn[i], polyn[j]), polyn[k]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				bufferMatrix->vars[cur] = 0;
				writeToVar(bufferMatrix->vars + cur, i + 1, 0);
				writeToVar(bufferMatrix->vars + cur, j + 1, 1);
				writeToVar(bufferMatrix->vars + cur, k + 1, 2);
				cur++;
			}
		}
	}

	for (uint i = 0; i < size; i++) {
		for(uint j = i; j < size; j++) {
			bufferFactor = modularMult(i==j?3:6, modularMult(polyn[size], modularMult(polyn[i], polyn[j])));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			bufferMatrix->vars[cur] = 0;
			writeToVar(bufferMatrix->vars + cur, i + 1, 0);
			writeToVar(bufferMatrix->vars + cur, j + 1, 1);
			cur++;
		}
	}

	for(uint i = 0; i < size; i++) {
		bufferFactor = modularMult(3, modularMult(modularDeg(polyn[size], 2), polyn[i]));
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		bufferMatrix->vars[cur] = 0;
		writeToVar(bufferMatrix->vars + cur, i + 1, 0);
		cur++;
	}

	for (cur; cur < MAX_TERMS_IN_CUBE; cur++) {
		bufferMatrix->factor[cur] = 0;
		bufferMatrix->vars[cur] = 0;
	}
}

void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey) {
	ulong cur;
	for (uint i = 0; i < size; i++) {
		cur = 0;
		for (uint j = 0; j < size; j++) {
			for (uint k = 0; k < MAX_TERMS_IN_CUBE; k++) {
				if (bufferMatrix[j].factor[k] == 0) {
					break;
				}
				uint p;
				for (p = 0; p < cur; p++) {
					if (bufferMatrix[j].vars[k] == publicKey[i].vars[p]) {
						publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]));
						break;
					}
				}
				if (p >= cur) {
					publicKey[i].factor[cur] = modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]);
					if (publicKey[i].factor[cur] == 0) {
						break;
					}
					publicKey[i].vars[cur] = bufferMatrix[j].vars[k];
					cur++;
				}
			}
		}
		for (cur; cur < MAX_TERMS_IN_KEY; cur++) {
			publicKey[i].factor[cur] = 0;
			publicKey[i].vars[cur] = 0;
		}
	}
}

void writeToVar(uint *var, uint number, uint pos) {
	for (uint i = 0; i < bitsForVar; i++) {
		*var &= (~(1 << (31 - i - bitsForVar * pos)));
	}
	*var |= number << (32 - bitsForVar * pos - bitsForVar);
}

uint getFromVar(uint var, uint pos) {
	uint g = (1 << bitsForVar) - 1;
	return (var >> (32 - bitsForVar - bitsForVar * pos)) & g;
}

uint bitsForVariable() {
	uint i = 0;
	uint vars = AMOUNT_OF_VARIABLES;
	while (vars > 0) {
		vars >>= 1;
		i++;
	}
	return i;
}
