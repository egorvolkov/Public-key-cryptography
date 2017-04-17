#include <stdio.h>

#include "myTime.h"

#include "../defines.h"

typedef unsigned long long ulong;
typedef unsigned long uint;
typedef unsigned char uchar;

void getPublicKey(ulong *publicKey);
void createMessage(ulong *message);
void coding(ulong *message, ulong *publicKey, uchar *encodedMessage);
uint returnEncodedMessage(uchar *encodedMessage);

ulong getRandom(ulong module);
void factor(ulong *publicKey);

//	TO OPTIMIZE THESE FUNCTIONS
void longAdd(uchar *a, uchar *b, uchar *result);
void longAddEncoded(uchar *a, uchar *b, uchar *result);
void longMult(uchar *a, uchar *b, uchar *result);
void longMultToChar(uchar *a, uchar b, uchar *result);
void longDeg(ulong a, uchar b, uchar *result);
#ifdef DEBUG
void longPrintNumber(uchar *number, int N);
#endif
void longToZero(uchar *number);
void longToZeroEncoded(uchar *number);
void longGetNumber(ulong number, uchar *longNumber);