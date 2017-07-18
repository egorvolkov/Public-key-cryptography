#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants) {
	ulong cube[size];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "Cube: "); printf("Cube: ");
#endif
	for (int i = 0; i < size; i++) {
		cube[i] = 0;
        printf("!!!!!!!!!!!!\n");
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена без констант
		for (int j = 0; j < size; j++) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * size + j], encodedOrRealMessage[j]));
            printf("%llu += %llu x %llu\n", cube[i], secondInverseMatrix[i * size + j], encodedOrRealMessage[j]);
		}
		// Добавляем вектор констант в кубе к многочлену
		cube[i] = modularAdd(cube[i],modularDeg(constants[i],3));
#ifdef PRINT
		//fprintf(output, "%llu ", cube[i]); printf("%llu ", cube[i]);
#endif
		//	Вычисляем кубический корень. Нашли результат начального многочлена
		cube[i] = modularDeg(cube[i], inverseDegree);
		//  Вычитаем столбец констант
		cube[i] = modularAdd(cube[i], modularInverseAdd(constants[i]));
	}
#ifdef PRINT
	fprintf(output, "\nCube-root: "); printf("\nCube-root: ");
	for (int i = 0; i < size; i++) {
		fprintf(output, "%llu ", cube[i]);
	}
	fprintf(output, "\n"); printf("\n");
#endif
	//	Умножаем обратную первую матрицу на начальный. Получаем те значения, который пользователь подставлял в многочлен
	for (int i = 0; i < size; i++) {
		encodedOrRealMessage[i] = 0;
		for (int j = 0; j < size; j++) {
			encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i], modularMult(firstInverseMatrix[i * size + j], cube[j]));
		}
	}
}

void newDecoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants) {
	ulong cube[size];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "Cube: "); printf("Cube: ");
#endif
	for (int i = 0; i < size; i++) {
		cube[i] = 0;
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена без констант
		printf("!!!!!!!!!!!!");
        for (int j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_SECOND;) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j + 1], encodedOrRealMessage[secondInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j]]));
            printf("%llu += %llu x %llu\n", cube[i], secondInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j + 1], encodedOrRealMessage[secondInverseMatrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j]]);
            j += 2;
        }
		// Добавляем вектор констант в кубе к многочлену
        //printf("** %llu\n", cube[i]);
		cube[i] = modularAdd(cube[i],modularDeg(constants[i],3));
#ifdef PRINT
		fprintf(output, "%llu ", cube[i]); printf("%llu ", cube[i]);
#endif
		//	Вычисляем кубический корень. Нашли результат начального многочлена
		cube[i] = modularDeg(cube[i], inverseDegree);
		//  Вычитаем столбец констант
		cube[i] = modularAdd(cube[i], modularInverseAdd(constants[i]));
	}
#ifdef PRINT
	fprintf(output, "\nCube-root: "); printf("\nCube-root: ");
	for (int i = 0; i < size; i++) {
		fprintf(output, "%llu ", cube[i]);
	}
	fprintf(output, "\n"); printf("\n");
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