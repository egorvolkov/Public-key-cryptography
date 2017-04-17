#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "myTime.h"

#include "../defines.h"

#define PATH_TO_MODULES "modules.txt"
#define MODULES 4806

typedef unsigned long long ulong;
typedef unsigned long uint;
typedef unsigned char uchar;

struct Module {
	ulong module;
	const uchar masSize;
	ulong partsOfModule[MAS_SIZE*3];
};

struct Matrices {
	ulong firstMatrix[SIZE * SIZE];
	ulong firstInverseMatrix[SIZE * SIZE];
	ulong secondMatrix[SIZE * SIZE];
	ulong secondInverseMatrix[SIZE * SIZE];
};

void generateSecretKey(struct Matrices *matrices);
void generateModule();
void computePartsOfModule();
void generateFirstMatrices(ulong *firstMatrix, ulong *firstInverseMatrix);
void generateSecondMatrices(ulong *secondMatrix, ulong *secondInverseMatrix);
void computeInverseMatrix(ulong *matrixDown, ulong *matrixUp, ulong *inverseMatrix);
void getRandTriangleMatrix(ulong *matrix, uchar dir);

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *publicKey);
void cubeOfPolynomials(ulong *matrix, ulong *bufferMatrix);
void polynomialCube(ulong *polyn, ulong *bufferPolyn);
void multToSecondMatrix(ulong *matrix, ulong *bufferMatrix, ulong *publicKey);

uint returnPublicKey(ulong *publicKey);
void transmitterConnection();
void getEncodedMessage(ulong *encodedOrRealMessage);

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage);

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
void modularMatrixMult(ulong *mat1, ulong *mat2, ulong *result);

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

#ifdef DEBUG
void printMatrix(ulong *matrix, uchar size1, uchar size2);
void fPrintMatrix(ulong *matrix, uchar size1, uchar size2);
#endif
