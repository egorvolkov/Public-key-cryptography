#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedMessage, ulong *realMessage, ulong *constants, ulong *secretVector) {
	ulong cube[AMOUNT_OF_POLYNOMS];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "Cube: "); printf("Cube: ");
#endif
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		cube[i] = 0;
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена без констант
		for (int j = 0; j < AMOUNT_OF_POLYNOMS; j++) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * AMOUNT_OF_POLYNOMS + j], encodedMessage[j]));
		}
		// Добавляем вектор констант в кубе к многочлену
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
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		fprintf(output, "%llu ", cube[i]);
	}
	fprintf(output, "\n"); printf("\n");
#endif
	//	Умножаем обратную первую матрицу на начальный. Получаем те значения, который пользователь подставлял в многочлен

	/* СКАЛЯРНОЕ ПРОИЗВЕДЕНИЕ*/
	ulong result = 0;
	scalar(cube,secretVector,&result,AMOUNT_OF_POLYNOMS);
	translateFromDecimal(result, RADIX, realMessage);
	// for (int i = 0; i < size; i++) {
	// 	encodedMessage[i] = 0;
	// 	for (int j = 0; j < size; j++) {
	// 		encodedMessage[i] = modularAdd(encodedMessage[i], modularMult(firstInverseMatrix[i * size + j], cube[j]));
	// 	}
	// }
}
void scalar(ulong *a, ulong *b, ulong *result, ulong N){
  *result = 0;
  for (int i = 1; i < N; i++){
    *result = modularAdd(*result, modularMult(a[i], b[i]));
  }
	*result = modularAdd(*result, modularInverseAdd(modularMult(a[0],b[0])));
}
void translateFromDecimal(ulong decimal, ulong system, ulong *array) {
    ulong i = 0;
    while (i < AMOUNT_OF_VARIABLES) {
        array[i++] = decimal % system;
        decimal /= system;
    }
}
