#include "receiver.h"
#include <time.h>
#include <sys/time.h>
extern const uint size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, FullCubePolynomial *publicKey, ulong *constants, ulong det, ulong *answer) {
	mult_A_on_all_vectors_B(firstMatrix, answer);
	CubePolynomial bufferMatrix[AMOUNT_OF_POLYNOMS];
	cubeOfPolynomials(answer, bufferMatrix, constants);
#ifdef PRINT
	printf("Constants:\n");
	for(uint i = 0; i < AMOUNT_OF_POLYNOMS; i++){
		printf("%llu ", constants[i]);
	}
	printf("\n\n");
	fprintf(output, "Cube polynomials\n"); printf("Cube polynomials\n");
	fPrintCubePolynomials(bufferMatrix); printCubePolynomials(bufferMatrix);
	fprintf(output, "\n"); printf("\n");
#endif
	multToSecondMatrix(secondMatrix, bufferMatrix, publicKey);
	
}

void mult_A_on_all_vectors_B(ulong *matrix_A, ulong *answer){
	for(uint i = 0; i < AMOUNT_OF_VARIABLES; i++){
		mult_A_na_vector_B(matrix_A, (&answer[i]));
	}
}

void mult_A_na_vector_B(ulong *matrix_A, ulong *vector){
	ulong result[AMOUNT_OF_POLYNOMS] = {0};
	for(uint j = 0; j < AMOUNT_OF_POLYNOMS; j++){
		for(uint h = 0; h < AMOUNT_OF_POLYNOMS; h++){
			result[j] = modularAdd(result[j], modularMult(matrix_A[h + j * AMOUNT_OF_POLYNOMS], vector[h * AMOUNT_OF_VARIABLES]));
		}
	}
	for(uint i = 0; i < AMOUNT_OF_POLYNOMS; i++){
		vector[i * AMOUNT_OF_VARIABLES] = result[i];
	}
}

void mult_A_na_vector_B_xd(ulong *matrix_A, ulong *vector){
	ulong result[AMOUNT_OF_POLYNOMS] = {0};
	for(uint j = 0; j < AMOUNT_OF_POLYNOMS; j++){
		for(uint h = 0; h < AMOUNT_OF_POLYNOMS; h++){
			result[j] = modularAdd(result[j], modularMult(matrix_A[h + j * AMOUNT_OF_POLYNOMS], vector[h]));
		}
	}
	for(uint i = 0; i < AMOUNT_OF_POLYNOMS; i++){
		vector[i] = result[i];
	}
}

void cubeOfPolynomials(ulong *matrix, CubePolynomial bufferMatrix[], ulong *constants) {
	CubePolynomial *buf;
	ulong polinom[AMOUNT_OF_VARIABLES + 1];
	for (uint i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		for (uint j = 0; j < AMOUNT_OF_VARIABLES; j++){
			polinom[j] = matrix[j + i * AMOUNT_OF_VARIABLES];
		}
		polinom[AMOUNT_OF_VARIABLES] = constants[i];
		buf = &(bufferMatrix[i]);
		polynomialCube(polinom, buf);
	}
}

void polynomialCube(ulong *polyn, CubePolynomial *bufferMatrix) {
	uint cur = 0;
	ulong bufferFactor;

	for (uint i = 0; i < MAX_VARS_IN_CUBE; i++) {
		bufferMatrix->vars[i] = 0;
	}

	for (uint i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		bufferFactor = modularDeg(polyn[i], 3);
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		//bufferMatrix->vars[cur] = 0;
		for (uint j = 0; j < 3; j++) {
			writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + j);
		}
		cur++;
	}

	for (uint i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		for (uint j = 0; j < AMOUNT_OF_VARIABLES; j++) {
			if (i == j)
				continue;
			bufferFactor = modularMult(3, modularMult(modularDeg(polyn[i], 2), polyn[j]));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			//bufferMatrix->vars[cur] = 0;
			for (uint k = 0; k < 2; k++) {
				writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + k);
			}
			writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 2);
			cur++;
		}
	}

	for (uint i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		for (uint j = i + 1; j < AMOUNT_OF_VARIABLES; j++) {
			for (uint k = j + 1; k < AMOUNT_OF_VARIABLES; k++) {
				bufferFactor = modularMult(6, modularMult(modularMult(polyn[i], polyn[j]), polyn[k]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				//bufferMatrix->vars[cur] = 0;
				writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
				writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 1);
				writeVar_test(bufferMatrix->vars, k + 1, (3 * cur) + 2);
				cur++;
			}
		}
	}

	for (uint i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		for(uint j = i; j < AMOUNT_OF_VARIABLES; j++) {
			bufferFactor = modularMult(i==j?3:6, modularMult(polyn[AMOUNT_OF_VARIABLES], modularMult(polyn[i], polyn[j])));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			//bufferMatrix->vars[cur] = 0;
			writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
			writeVar_test(bufferMatrix->vars, j + 1, (3 * cur) + 1);
			cur++;
		}
	}

	for(uint i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		bufferFactor = modularMult(3, modularMult(modularDeg(polyn[AMOUNT_OF_VARIABLES], 2), polyn[i]));
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		//bufferMatrix->vars[cur] = 0;
		writeVar_test(bufferMatrix->vars, i + 1, (3 * cur) + 0);
		cur++;
	}

	for (cur; cur < MAX_TERMS_IN_CUBE; cur++) {
		bufferMatrix->factor[cur] = 0;
		//bufferMatrix->vars[cur] = 0;
	}
}

void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey) {
	ulong cur;
	for(uint i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		for (uint j = 0; j < MAX_VARS_IN_KEY; j++) {
			publicKey[i].vars[j] = 0;
		}
	}
	for (uint i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		cur = 0;
		for (uint j = 0; j < AMOUNT_OF_POLYNOMS; j++) {
			for (uint k = 0; k < MAX_TERMS_IN_CUBE; k++) {
				if (bufferMatrix[j].factor[k] == 0) {
					break;
				}
				uint p;
				for (p = 0; p < cur; p++) {
					if (get3Vars_test(bufferMatrix[j].vars, k) == get3Vars_test(publicKey[i].vars, p)) {
						publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(matrix[i*AMOUNT_OF_POLYNOMS + j], bufferMatrix[j].factor[k]));
						break;
					}
				}
				if (p >= cur) {
					publicKey[i].factor[cur] = modularMult(matrix[i*AMOUNT_OF_POLYNOMS + j], bufferMatrix[j].factor[k]);
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
		for (cur; cur < MAX_TERMS_IN_KEY; cur++) {
			publicKey[i].factor[cur] = 0;
		}
	}
}

void solve(ulong *vector, ulong N, ulong answer, ulong *result){
	if (N == 1){
		result[0] = modularMult(modularInverseMult(modularInverseAdd(vector[0])),answer);
		return;
	}
  srand(time(NULL));
  ulong temp = answer;
  ulong module = vector[0];
  for (ulong i = 2; i < N; i++){
    result[i] = rand() % (moduleStruct.module - 1) + 1;
    temp = modularAddUniver(temp, modularMultUniver(modularInverseAddUniver(vector[i],module), result[i], module), module);
  }
  result[1] = modularMultUniver(temp, modularInverseMultUniver(vector[1],module), module) % moduleStruct.module;
  if (result[1] == 0){
    result[1] = module;
  }
  temp = modularInverseAdd(answer);
  for (int i = 1; i < N; i++){
		temp = modularAdd(temp, modularMult(result[i], vector[i]));
  }
  result[0] = modularDiv(temp,vector[0]);
}

/*coef_vector - коэффициенты уравнения, answer_vector - правые части, root_matrix - выходная матрица столбцов*/
void full_gcd(ulong* coef_vector, ulong* answer_vector, ulong* root_matrix){
	ulong k = 0;
	ulong	root_vector[LENGTH_OF_SECRET_VECTOR];
	for (ulong i = 0; i < AMOUNT_OF_VARIABLES*NUMBER_OF_RADIX; ++i)
	{
		/* code */
		solve(coef_vector, LENGTH_OF_SECRET_VECTOR, answer_vector[i], root_vector);
		for (ulong j = 0; j < LENGTH_OF_SECRET_VECTOR; ++j)
		{
			root_matrix[(i/AMOUNT_OF_VARIABLES)*LENGTH_OF_SECRET_VECTOR*AMOUNT_OF_VARIABLES+j*AMOUNT_OF_VARIABLES + i%AMOUNT_OF_VARIABLES] = root_vector[j];
		}
	}
}

void writeVar(uint *var, uint number, uint pos) {
	for (uint i = 0; i < SIZE_OF_AVAR; i++) {
		*var &= (~(1 << (31 - i - SIZE_OF_AVAR * pos)));
	}
	*var |= number << (32 - SIZE_OF_AVAR * pos - SIZE_OF_AVAR);
}

uint getFromVar(uint var, uint pos) {
	uint g = (1 << SIZE_OF_AVAR) - 1;
	return (var >> (32 - SIZE_OF_AVAR - SIZE_OF_AVAR * pos)) & g;
}
