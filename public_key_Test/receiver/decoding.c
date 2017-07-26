#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3) {
	ulong cube[size];
	ulong inverseDegree[2];
    inverseDegree[0] = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
    inverseDegree[1] = modularInverseMultUniver(5, euler(moduleStruct.module)); //  Считаем степень, которая будет соответствовать корнюы 5 степени из числа
#ifdef PRINT
	printf("inverseDegree: %llu %llu\n\n", inverseDegree[0], inverseDegree[1]);
	fprintf(output, "InversedDegree: %llu %llu\n\n", inverseDegree[0], inverseDegree[1]);
#endif
	for (int i = 0; i < size; i++) {
		// Добавляем вектор кубических констант к многочлену
		encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i],constants3[i]);
	}

#ifdef PRINT
	// for (int i = 0; i < size; i++) {
	// 	printf("%llu ", encodedOrRealMessage[i]);
	// }
	// printf("\n\n");
	printf("After mult on B^(-1): ");
	fprintf(output, "After mult on B^(-1): "); 
#endif
	for (int i = 0; i < size; i++) {
		cube[i] = 0;
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена без констант
		for (int j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_SECOND;) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j + 1],
													  encodedOrRealMessage[secondInverseMatrix[
															  i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j]]));
			j += 2;
		}
#ifdef PRINT
		printf("%llu ", cube[i]);
		fprintf(output, "%llu ", cube[i]);
#endif
	/*secondInverseMatrix[pow * 2 + 1] % 6)*/}
    uint pow = ((uint)(AMOUNT_OF_VARIABLES - 1) * (AMOUNT_OF_VARIABLES) / 2 - 1) % (size * AMOUNT_OF_VAR_IN_LINE_SECOND);
    uint invpow = ((uint) AMOUNT_OF_VARIABLES - 1) % (size * AMOUNT_OF_VAR_IN_LINE_SECOND);
	// Вычисляем и убираем дополнительные функции
	for (int i = size - 1; i>=0; i--) {
		for (int j = size - 1; j > i; j--) {
			cube[i] = modularSub(cube[i],modularDeg(cube[j], secondInverseMatrix[pow * 2 + 1] % 6));
			if (pow == 0) {
				pow = size * AMOUNT_OF_VAR_IN_LINE_SECOND;
			}
            pow--;
		}
		//	Вычисляем кубический корень. Нашли результат начального многочлена
		cube[i] = modularDeg(cube[i], inverseDegree[secondInverseMatrix[invpow * 2 + 1] % 2]);
        if (invpow == 0) {
            invpow = size * AMOUNT_OF_VAR_IN_LINE_SECOND;
        }
        invpow--;
	}

	for (int i = 0; i < size; i++) {/**/
		//  Вычитаем столбец констант
		cube[i] = modularAdd(cube[i], modularInverseAdd(constants[i]));
	}
#ifdef PRINT
	printf("\n\nCube-root: ");
	fprintf(output, "\n\nCube-root: "); 
	for (int i = 0; i < size; i++) {
		printf("%llu ", cube[i]);
		fprintf(output, "%llu ", cube[i]);
	}
	printf("\n\n");
	fprintf(output, "\n\n"); 
#endif
	//	Умножаем обратную первую матрицу на начальный. Получаем те значения, который пользователь подставлял в многочлен
	for (int i = 0; i < size; i++) {
		encodedOrRealMessage[i] = 0;
		for (int j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_FIRST;) {
			encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i], modularMult(firstInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j + 1], cube[firstInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j]]));
		    j += 2;
        }
	}
}