#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

uint bitsForVar;

#ifdef PRINT
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, FullCubePolynomial *publicKey, ulong *constants) {
	bitsForVar = bitsForVariable();
	CubePolynomial bufferMatrix[MAX_TERMS];
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
	//for(uint i = 0; i < size; i++) bufferMatrix->vars[i] = 0;

	for (uint i = 0; i < size; i++) {
		bufferFactor = modularDeg(polyn[i], 3);
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		bufferMatrix->vars[cur] = 0;
		for (uint j = 0; j < 3; j++) {
			writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + j);
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
				writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + k);
			}
			writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 2);
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
				writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
				writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 1);
				writeVar_test(bufferMatrix->vars, k + 1, (3 * cur) + 2);
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
			writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
			writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 1);
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
		writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
		cur++;
	}

	for (cur; cur < MAX_TERMS/AMOUNT_OF_VARIABLES; cur++) {
		bufferMatrix->factor[cur] = 0;
		bufferMatrix->vars[cur] = 0;
	}
}

void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey) {
	ulong cur;
	for (uint i = 0; i < size; i++) {
		cur = 0;
		for (uint j = 0; j < size; j++) {
			for (uint k = 0; k < MAX_TERMS/AMOUNT_OF_VARIABLES; k++) {
				if (bufferMatrix[j].factor[k] == 0) {
					break;
				}
				uint p;
				for (p = 0; p < cur; p++) {
					if (get3Vars_test(bufferMatrix[j].vars, k) == get3Vars_test(publicKey[i].vars, p)) {
						publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]));
						break;
					}
				}
				if (p >= cur) {
					publicKey[i].factor[cur] = modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]);
					if(publicKey[i].factor[cur] == 0) {
						break;
					}
					write3Vars_test(bufferMatrix[j].vars, publicKey[i].vars, k, cur);
					//print_bin(get3Vars_test(bufferMatrix[j].vars, k), 9);
					//printf("(%llu); ", bufferMatrix[j].factor[k]);
					//print_bin(get3Vars_test(publicKey[i].vars, cur), 9);
					//printf("(%llu);\n", publicKey[i].factor[cur]);
					cur++;
				}
			}
		}
		for (cur; cur < MAX_TERMS; cur++) {
			publicKey[i].factor[cur] = 0;
			publicKey[i].vars[cur] = 0;
		}
	}
}

void writeVar(uint *var, uint number, uint pos) {
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
