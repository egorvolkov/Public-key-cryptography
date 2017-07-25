#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3) {
	ulong cube[size];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "Cube: "); printf("Cube: ");
#endif

	for (int i = 0; i < size; i++) {
		// Добавляем вектор кубических констант к многочлену
		encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i],constants3[i]);
	}

	for (int i = 0; i < size; i++) {
		cube[i] = 0;
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена
		for (int j = 0; j < size; j++) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * size + j], encodedOrRealMessage[j]));
		}

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
		for (int j = 0; j < size; j++) {
			encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i], modularMult(firstInverseMatrix[i * size + j], cube[j]));
		}
	}
}

void newDecoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *funcMatrix, ulong *encodedOrRealMessage, ulong *constants, ulong *constants3) {
	ulong cube[size];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
#endif
	for (int i = 0; i < size; i++) {
		// Добавляем вектор кубических констант к многочлену
		encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i],constants3[i]);
	}

#ifdef PRINT
	for (int i = 0; i < size; i++) {
		printf("%llu ",encodedOrRealMessage[i]);
	}
	printf("\n");
	fprintf(output, "Cube: "); printf("Cube: ");
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
		fprintf(output, "%llu ", cube[i]);
		printf("%llu ", cube[i]);
#endif
	/**/}

	// Вычисляем и убираем дополнительные функции
	for (int i = size - 1; i>=0; i--) {
		for (int j = i + 1; j < size; j++) {
			cube[i] = modularSub(cube[i],modularDeg(cube[j], 2));
		}
		//	Вычисляем кубический корень. Нашли результат начального многочлена
		cube[i] = modularDeg(cube[i], inverseDegree);
	}

	for (int i = 0; i < size; i++) {/**/
		//  Вычитаем столбец констант
		cube[i] = modularAdd(cube[i], modularInverseAdd(constants[i]));
	}
#ifdef PRINT
	fprintf(output, "\nCube-root: "); printf("\nCube-root: ");
	for (int i = 0; i < size; i++) {
		fprintf(output, "%llu ", cube[i]);
		printf("%llu ", cube[i]);
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