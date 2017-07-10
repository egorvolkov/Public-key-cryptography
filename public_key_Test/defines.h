/**
 * TIME - if this constant is defined then time will be measured and printed
 * PRINT - if this constant is defined then all information will be printed to console and to a file "output.txt"
 */
//#define TIME
#define PRINT

/**
 * AMOUNT_OF_LOOP_ITERATIONS - the program will starts a loop with this parameter. Must be more then 0
 * PAUSE - amount of loop iterations before a pause. Must be more then 0
 * 		Also you can set this parameter like an argument of execution the program. Parameter has more priority.
 */
#define AMOUNT_OF_LOOP_ITERATIONS 10000
#define PAUSE 10

/**
 * AMOUNT_OF_VARIABLES - amount of variables; from 0 to infinity (in a theory)
 * 		But if you want to increase size of coding message we recommend to increase SIZE_OF_VARIABLE
 * 		because increasing AMOUNT_OF_VARIABLES can cause memory error. Also it will be much faster.
 * 		For instanse our computer don't execute the program with AMOUNT_OF_VARIABLES more then 28.
 * SIZE_OF_VARIABLE - amount of bits on a variable; from 3 to 62
 * SIZE_OF_MODULE - amount of bits on a module; from 4 to 63
 * 		SIZE_OF_MODULE must be more then SIZE_OF_VARIABLE
 * MAS_SIZE - amount of multipliers of which the module consist; We recommend the next meanings:
 * 		SIZE_OF_MODULE < 21			then MAS_SIZE is 1
 * 		21 <= SIZE_OF_MODULE < 29	then MAS_SIZE is 2
 * 		29 <= SIZE_OF_MODULE < 47	then MAS_SIZE is 3
 * 		47 <= SIZE_OF_MODULE < 56	then MAS_SIZE is 4
 * 		56 <= SIZE_OF_MODULE < 64	then MAS_SIZE is 5
 *   	You can use any meanings you want but our algorithm of module generation isn't perfect.
 * 		Some meanings can cause looping.
 */
#define AMOUNT_OF_VARIABLES 2
#define AMOUNT_OF_VAR_IN_LINE 2
#define SIZE_OF_VARIABLE 7
#define SIZE_OF_MODULE (SIZE_OF_VARIABLE + 1)
#define MAS_SIZE 1

/**
 * BYTES_FOR_AMOUNT_OF_MEMBER - this constant is for encoding a long number.
 * 		AMOUNT_OF_VARIABLES < 11		then BYTES_FOR_AMOUNT_OF_MEMBER is 1 or 2;
 * 		11 <= AMOUNT_OF_VARIABLES < 73	then BYTES_FOR_AMOUNT_OF_MEMBER is 2;
 * 		You don't need more meanings.
 */
#define BYTES_FOR_AMOUNT_OF_MEMBER 2
