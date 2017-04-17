/**
 * If this constant is defined then:
 * 		The program outputs part of an information to console and all information to the file "output.txt";
 *   	The program checks some errors of the algorithm;
 *   	Start an infinite loop with pauses;
 * Else:
 * 		The program works once.
 *
 * You can comment "//" this line to turn off debug-mode.
 */
#define DEBUG


/**
 * AMOUNT_OF_VARIABLES - amount of variables; from 0 to infinity
 * SIZE_OF_VARIABLE - amount of bits on a variable; from 3 to 62
 * SIZE_OF_MODULE - amount of bits on a module; from 4 to 63
 * 		SIZE_OF_MODULE must be more then SIZE_OF_VARIABLE
 * MAS_SIZE - amount of multipliers of which the module consist; We recommend the next meanings:
 * 		SIZE_OF_MODULE < 21			then MAS_SIZE is 1
 * 		21 <= SIZE_OF_MODULE < 33	then MAS_SIZE is 2
 * 		33 <= SIZE_OF_MODULE < 47	then MAS_SIZE is 3
 * 		47 <= SIZE_OF_MODULE < 56	then MAS_SIZE is 4
 * 		56 <= SIZE_OF_MODULE < 64	then MAS_SIZE is 5
 *   	You can use any meanings you want but our algorithm of module generation isn't perfect.
 * 		Some meanings can cause looping.
 */
#define AMOUNT_OF_VARIABLES 17
#define SIZE_OF_VARIABLE 62
#define SIZE_OF_MODULE (SIZE_OF_VARIABLE + 1)
#define MAS_SIZE 5

/**
 * BYTES_FOR_AMOUNT_OF_MEMBER - this constant is for encoding a long number.
 * 		AMOUNT_OF_VARIABLES < 11		then BYTES_FOR_AMOUNT_OF_MEMBER is 1;
 * 		11 <= AMOUNT_OF_VARIABLES < 73	then BYTES_FOR_AMOUNT_OF_MEMBER is 2;
 */
#define BYTES_FOR_AMOUNT_OF_MEMBER 2

/**
 * You can set the PAUSE - amount of loop iterations before pause.
 * Also you can set it like the first argument of the program.
 * Argument has more priority
 */
#define PAUSE 1