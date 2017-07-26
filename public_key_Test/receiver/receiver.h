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
#define MAX_TERMS_IN_POLY (AMOUNT_OF_VARIABLES * (AMOUNT_OF_VAR_IN_LINE_FIRST + 1) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 2) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 3) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 4) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 5) / 120)
//#define MAX_TERMS_IN_POLY ((AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) * (AMOUNT_OF_VARIABLES + 3) * (AMOUNT_OF_VARIABLES + 4) * (AMOUNT_OF_VARIABLES + 5) / 120)
#define MAX_VARS_IN_POLY (((MAX_TERMS_IN_POLY / (sizeof(uint) * 8)) + 1) * 5 * SIZE_OF_AVAR)
#define MAX_TERMS_IN_KEY (MAX_TERMS_IN_POLY)
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

typedef struct Module {
	ulong module;
	const uchar masSize;
	ulong partsOfModule[MAS_SIZE * 3];
} Module;

typedef struct Matrices {
	ulong firstMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST];
	ulong firstInverseMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST];
	ulong secondMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND];
	ulong secondInverseMatrix[AMOUNT_OF_VARIABLES * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND];
	ulong constants[AMOUNT_OF_VARIABLES];
	ulong constants3[AMOUNT_OF_VARIABLES];
} Matrices;

typedef struct CubePolynomial {
	ulong factor[MAX_TERMS_IN_POLY];
	uint vars[MAX_VARS_IN_POLY];
} CubePolynomial;

// typedef struct FullCubePolynomial {
// 	ulong factor[MAX_TERMS_IN_KEY];
// 	uint vars[MAX_VARS_IN_KEY];
// } FullCubePolynomial;

void generateSecretKey(struct Matrices *matrices);
void generateModule();
void computePartsOfModule();

void generateSimpleMatrix(ulong *matrix, ulong *inverseMatrix, ulong lines);
void computeInverseMatrix(ulong *matrices, ulong *inverseMatrix, ulong lines);
void getRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines);

void generateFirstMatrices_rare(ulong *firstMatrix, ulong *firstInverseMatrix);
void generateSecondMatrices_rare(ulong *secondMatrix, ulong *secondInverseMatrix);
uchar tenzorMult(ulong *A, ulong *B, ulong *result, ulong N, uchar numberOfMatrix, uchar check, uchar checkZero);
void swap(ulong *line1, ulong *line2, ulong length);
void shake(ulong *matrix, ulong *invert_matrix, ulong lines, ulong columns, ulong rare);

void generateConstants(ulong *constants);


void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *inverseSecondMatrix, CubePolynomial *newPublicKey, ulong *constants, ulong *constants3);
void cubeOfPolynomials(ulong *matrix, CubePolynomial newBufferMatrix[], ulong *constants, ulong *inverseSecondMatrix);
void polynomialDeg(ulong *polyn, CubePolynomial *bufferMatrix, ulong deg);
void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, CubePolynomial *publicKey, ulong *constants3);
void addFunctions(ulong *matrix, ulong *constants, CubePolynomial *bufferMatrix, ulong *funcMatrix);

uint returnPublicKey(CubePolynomial *publicKey);
void transmitterConnection();
void getEncodedMessage(ulong *encodedOrRealMessage);

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3);

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

uint getFromVar(uint var, uint pos);

void printSecretKey(Matrices matrices);
void printCubePolynomials(CubePolynomial *cubePolynomials);
void printPolynomialsAfterAddFunctions(CubePolynomial *polynomials);
void printPublicKey(CubePolynomial *publicKey, uint bytes);
void printEncodedMessage(ulong *encodedMessage);
void printDecodedMessage(ulong *decodedMessage);
void printMatrix(ulong *matrix, uint size1, uint size2);
void fprintMatrix(ulong *matrix, uint size1, uint size2);
void printPolynomials(CubePolynomial *cubePolynomials);
void fprintPolynomials(CubePolynomial *cubePolynomials);
void printPolynomial(CubePolynomial cubePolynomials);
void fprintPolynomial(CubePolynomial cubePolynomials);
void printRareMatrix(ulong* matrix, int size1, int size2, int amount);
void fprintRareMatrix(ulong* matrix, int size1, int size2, int amount);

void writeVar_test(uint*, uint, uint);
void print_bin(uint, uint);
uint getFromVar_test(uint*, uint);
ulong get5Vars(uint *, uint);
void write5Vars(uint *, uint *, uint, uint);
