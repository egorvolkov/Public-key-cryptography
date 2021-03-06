#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../defines.h"


#define AMOUNT_OF_MEMBERS (AMOUNT_OF_VARIABLES * (AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) / 6)
#define SIZE_OF_LONG_NUMBER ((SIZE_OF_MODULE + 5*SIZE_OF_VARIABLE + 7) / 8)
#define LENGTH_OF_ENCODED_NUMBER (SIZE_OF_LONG_NUMBER + BYTES_FOR_AMOUNT_OF_MEMBER)
#define MAX_TERMS_IN_POLY (AMOUNT_OF_VARIABLES * (AMOUNT_OF_VAR_IN_LINE_FIRST + 1) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 2) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 3) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 4) * (AMOUNT_OF_VAR_IN_LINE_FIRST + 5)/ 120)
//#define MAX_TERMS_IN_POLY ((AMOUNT_OF_VARIABLES + 1) * (AMOUNT_OF_VARIABLES + 2) * (AMOUNT_OF_VARIABLES + 3) * (AMOUNT_OF_VARIABLES + 4) * (AMOUNT_OF_VARIABLES + 5) / 120)
#define MAX_VARS_IN_POLY (((MAX_TERMS_IN_POLY / (sizeof(uint) * 8)) + 1) * 5 * SIZE_OF_AVAR)
#define MAX_TERMS_IN_KEY (MAX_TERMS_IN_POLY)
#define MAX_VARS_IN_KEY (((MAX_TERMS_IN_KEY / (sizeof(uint) * 8)) + 1) * 5 * SIZE_OF_AVAR)

#define PATH_TO_PUBLIC_KEY "channel/public_key"
#define PATH_TO_THE_FIRST_FLAG "channel/flag1"
#define PATH_TO_THE_SECOND_FLAG "channel/flag2"
#define PATH_TO_MESSAGE "channel/message.txt"

typedef unsigned long long ulong;
typedef unsigned long uint;
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

/**
 *  This block for time calculations
 */

void startTime(clock_t *time, ulong *realtime);
void endTime(clock_t *time, ulong *realtime);
void printTimes(clock_t times, ulong realtime);

//	TO OPTIMIZE THESE FUNCTIONS
void longAdd(uchar *a, uchar *b, uchar *result);
void longAddEncoded(uchar *a, uchar *b, uchar *result);
void longMult(uchar *a, uchar *b, uchar *result);
void longMultToChar(uchar *a, uchar b, uchar *result);
void longDeg(ulong a, uchar b, uchar *result);
void longPrintNumber(uchar *number, int N);
void longGetNumber(ulong number, uchar *longNumber);

void printFullCubePolynomials(FullCubePolynomial *cubePolynomials);
uint getFromVar(uint var, uint pos);

void writeVar_test(uint*, uint, uint);
void print_bin(uint, uint);
uint getFromVar_test(uint*, uint);
ulong get5Vars(uint *, uint);
unsigned char bytesNumber(ulong num);