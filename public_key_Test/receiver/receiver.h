#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../defines.h"

#ifdef TIME
#include "myTime.h" 
#endif


#define AMOUNT_OF_MEMBERS (AMOUNT_OF_VARIABLES * (AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) / 6)
#define SIZE_OF_LONG_NUMBER ((SIZE_OF_MODULE + 5*SIZE_OF_VARIABLE + 7) / 8)
#define LENGTH_OF_ENCODED_NUMBER (SIZE_OF_LONG_NUMBER + BYTES_FOR_AMOUNT_OF_MEMBER)
//#define MAX_TERMS_IN_POLY ((AMOUNT_OF_VAR_IN_LINE_FIRST + 1) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 2) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 3) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 4) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 5) / 120)
#define MAX_TERMS_IN_POLY ((AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) * (AMOUNT_OF_VARIABLES + 3) * (AMOUNT_OF_VARIABLES + 4) * (AMOUNT_OF_VARIABLES + 5) / 120)
#define MAX_VARS_IN_POLY (((MAX_TERMS_IN_POLY / (sizeof(uint) * 8)) + 1) * 5 * SIZE_OF_AVAR)
#define MAX_TERMS_IN_KEY (AMOUNT_OF_VAR_IN_LINE_SECOND * MAX_TERMS_IN_POLY)
#define MAX_VARS_IN_KEY (((MAX_TERMS_IN_KEY / (sizeof(uint) * 8)) + 1) * 5 * SIZE_OF_AVAR)

#define PATH_TO_PUBLIC_KEY "channel/public_key"
#define PATH_TO_THE_FIRST_FLAG "channel/flag1"
#define PATH_TO_THE_SECOND_FLAG "channel/flag2"
#define PATH_TO_MESSAGE "channel/message.txt"
#ifdef PRINT
#define PATH_TO_OUTPUT "output.txt"
#endif


#define PATH_TO_MODULES "modules.txt"
#define MODULES 4806

typedef unsigned long long ulong;
typedef unsigned long uint;
typedef unsigned char uchar;

struct Module {
	ulong module;
	const uchar masSize;
	ulong partsOfModule[MAS_SIZE * 3];
};

struct Matrices {
	ulong firstMatrix[AMOUNT_OF_VARIABLES * AMOUNT_OF_VARIABLES];
	ulong firstInverseMatrix[AMOUNT_OF_VARIABLES * AMOUNT_OF_VARIABLES];
	ulong secondMatrix[AMOUNT_OF_VARIABLES * AMOUNT_OF_VARIABLES];
	ulong secondInverseMatrix[AMOUNT_OF_VARIABLES * AMOUNT_OF_VARIABLES];
	ulong constants[AMOUNT_OF_VARIABLES];
	ulong constants3[AMOUNT_OF_VARIABLES];
};

struct NewMatrices {
	ulong firstMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST];
	ulong firstInverseMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST];
	ulong secondMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND];
	ulong secondInverseMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND];
	ulong constants[AMOUNT_OF_VARIABLES];
	ulong constants3[AMOUNT_OF_VARIABLES];
	ulong functionMatrix[AMOUNT_OF_VARIABLES * AMOUNT_OF_VARIABLES];
};

typedef struct CubePolynomial {
	ulong factor[MAX_TERMS_IN_POLY];
	uint vars[MAX_VARS_IN_POLY];
} CubePolynomial;

typedef struct FullCubePolynomial {
	ulong factor[MAX_TERMS_IN_KEY];
	uint vars[MAX_VARS_IN_KEY];
} FullCubePolynomial;

void generateSecretKey(struct NewMatrices *matrices);
void generateModule();
void computePartsOfModule();

void generateFirstMatrices(ulong *firstMatrix, ulong *firstInverseMatrix, ulong lines);
void generateSecondMatrices(ulong *secondMatrix, ulong *secondInverseMatrix, ulong lines);
void generateFunctionMatrix(ulong *matrix);
void computeInverseMatrix(ulong *matrices, ulong *inverseMatrix, ulong lines);
void getRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines);
void getNewRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines);

void generateFirstMatrices_rare(ulong *firstMatrix, ulong *firstInverseMatrix);
void generateSecondMatrices_rare(ulong *secondMatrix, ulong *secondInverseMatrix);
ulong determinant(ulong *matrix, ulong size);
void commonComputeInverseMatrix(ulong *inmatrix, ulong* outmatrix, ulong size, ulong det);
uchar tenzorMult(ulong *A, ulong *B, ulong *result, ulong N, uchar check);
void swap(ulong *line1, ulong *line2, ulong length);
void shake(ulong *matrix, ulong *invert_matrix, ulong lines, ulong columns);

void generateConstants(ulong *constants);


void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, FullCubePolynomial *publicKey, ulong *constants, ulong *constants3);
void computeNewPublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *funcMatrix, FullCubePolynomial *newPublicKey, ulong *constants, ulong *constants3);
void cubeOfNewPolynomials(ulong *matrix, CubePolynomial newBufferMatrix[], ulong *constants);
void cubeOfPolynomials(ulong *matrix, CubePolynomial bufferMatrix[], ulong *constants);
void polynomialCube(ulong *polyn, CubePolynomial *bufferMatrix);
void polynomialDeg(ulong *polyn, CubePolynomial *bufferMatrix, ulong deg);
void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey, ulong *constants3);
void newMultToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey, ulong *constants3);
void addFunctions(ulong *matrix, ulong *constants, CubePolynomial *bufferMatrix, ulong *funcMatrix);

uint returnPublicKey(FullCubePolynomial *publicKey);
void transmitterConnection();
void getEncodedMessage(ulong *encodedOrRealMessage);

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3);
void newDecoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *funcMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3);

uchar inArray(ulong *arr, uint length, ulong element);
uint getModules(uint index);
ulong getRandom(ulong module);
uchar cube(ulong x);
int canCreate(int n, ulong min, ulong max);

/**
 *	This block for operations with the common module
 */

ulong modularAdd(ulong a, ulong b);
ulong modularSub(ulong a, ulong b);
ulong modularDiv(ulong a, ulong b);
ulong modularMult(ulong a, ulong b);
ulong modularDeg(ulong a, ulong b);
ulong modularInverseAdd(ulong a);
ulong modularInverseMult(ulong a);
void modularTriangleMatrixMult(ulong *matrices, ulong *result, ulong lines);
void modularMatrixMult(ulong *mat1, ulong *mat2, ulong *result, ulong lines);

/**
 *	This block for operations with an any module
 */

ulong modularAddUniver(ulong a, ulong b, ulong module);
ulong modularSubUniver(ulong a, ulong b, ulong module);
ulong littleModularMultUniver(ulong a, ulong b, ulong module);
ulong modularMultUniver(ulong a, ulong b, ulong module);
ulong modularDegUniver(ulong a, ulong b, ulong module);
ulong modularInverseAddUniver(ulong a, ulong module);
ulong modularInverseMultUniver(ulong a, ulong module);

/**
 *	Other
 */

ulong euler(ulong n);
ulong gcd(ulong a, ulong b);

#ifdef PRINT
void printMatrix(ulong *matrix, uint size1, uint size2);
void fPrintMatrix(ulong *matrix, uint size1, uint size2);
#endif

void writeToVar(uint *var, uint number, uint pos);
uint getFromVar(uint var, uint pos);

void printCubePolynomials(CubePolynomial *cubePolynomials);
void fPrintCubePolynomials(CubePolynomial *cubePolynomials);
void printCubePolynomial(CubePolynomial cubePolynomials);
void printFullCubePolynomials(FullCubePolynomial *cubePolynomials);
void fPrintFullCubePolynomials(FullCubePolynomial *cubePolynomials);
void printNewMatrix(ulong* matrix, int size1, int size2, int amount);

void writeVar_test(uint*, uint, uint);
void print_bin(uint, uint);
uint getFromVar_test(uint*, uint);
ulong get5Vars(uint *, uint);
void write5Vars(uint *, uint *, uint, uint);
