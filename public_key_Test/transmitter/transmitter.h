#include <stdio.h>
#include <stdlib.h>

#include "../defines.h"

#ifdef TIME
#include "myTime.h"
#endif


#define AMOUNT_OF_MEMBERS (AMOUNT_OF_VARIABLES * (AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) / 6)
#define SIZE_OF_LONG_NUMBER ((SIZE_OF_MODULE + 3*SIZE_OF_VARIABLE + 7) / 8)
#define LENGTH_OF_ENCODED_NUMBER (SIZE_OF_LONG_NUMBER + BYTES_FOR_AMOUNT_OF_MEMBER)
#define MAX_TERMS_IN_CUBE ((AMOUNT_OF_VAR_IN_LINE_FIRST + 1) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 2) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 3) / 6)
#define MAX_VARS_IN_CUBE (((MAX_TERMS_IN_CUBE / 8 * sizeof(uint)) + 1) * 3 * SIZE_OF_AVAR)
#define MAX_TERMS_IN_KEY (AMOUNT_OF_VAR_IN_LINE_SECOND * MAX_TERMS_IN_CUBE)
#define MAX_VARS_IN_KEY (((MAX_TERMS_IN_KEY / 8 * sizeof(uint)) + 1) * 3 * SIZE_OF_AVAR)

#define PATH_TO_PUBLIC_KEY "channel/public_key"
#define PATH_TO_THE_FIRST_FLAG "channel/flag1"
#define PATH_TO_THE_SECOND_FLAG "channel/flag2"
#define PATH_TO_MESSAGE "channel/message.txt"

typedef unsigned long long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;

typedef struct FullCubePolynomial {
	ulong factor[MAX_TERMS_IN_KEY];
	uint vars[MAX_VARS_IN_KEY];
} FullCubePolynomial;

void getPublicKey(FullCubePolynomial *publicKey);
void createMessage(ulong *message);
void coding(ulong *message, FullCubePolynomial *publicKey, uchar *encodedMessage);
uint returnEncodedMessage(uchar *encodedMessage);

ulong getRandom(ulong module);
void factor(ulong *publicKey);

//	TO OPTIMIZE THESE FUNCTIONS
void longAdd(uchar *a, uchar *b, uchar *result);
void longAddEncoded(uchar *a, uchar *b, uchar *result);
void longMult(uchar *a, uchar *b, uchar *result);
void longMultToChar(uchar *a, uchar b, uchar *result);
void longDeg(ulong a, uchar b, uchar *result);
#ifdef PRINT
void longPrintNumber(uchar *number, int N);
#endif
void longToZero(uchar *number);
void longToZeroEncoded(uchar *number);
void longGetNumber(ulong number, uchar *longNumber);

void printFullCubePolynomials(FullCubePolynomial *cubePolynomials);
uint getFromVar(uint var, uint pos);

void writeVar_test(uint*, uint, uint);
void print_bin(uint, uint);
uint getFromVar_test(uint*, uint);
ulong get3Vars_test (uint*, uint);
