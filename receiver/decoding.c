#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef DEBUG
extern FILE *output;
#endif

void decoding(ulong *firstInverseMatrix, ulong *secondInverseMatrix, ulong *encodedOrRealMessage) {
	ulong cube[size];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef DEBUG
	printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "InversedDegree: %llu\n", inverseDegree);

	printf("Cube: ");
	fprintf(output, "Cube: ");
#endif
	for (int i = 0; i < size; i++) {
		cube[i] = 0;
		//	Умножаем обратную вторую матрицу на результат каждого многочлена. Получаем куб результата первого многочлена
		for (int j = 0; j < size; j++) {
			cube[i] = modularAdd(cube[i], modularMult(secondInverseMatrix[i * size + j], encodedOrRealMessage[j]));
		}
#ifdef DEBUG
		printf("%llu (", cube[i]);
		fprintf(output, "%llu (", cube[i]);
#endif
		//	Вычисляем кубический корень. Нашли результат начального многочлена
		cube[i] = modularDeg(cube[i], inverseDegree);
#ifdef DEBUG
		printf("%llu) ", cube[i]);
		fprintf(output, "%llu) ", cube[i]);
#endif
	}
#ifdef DEBUG
	printf("\n");
	fprintf(output, "\n");
#endif
	//	Умножаем обратную первую матрицу на начальный. Получаем те значения, который пользователь подставлял в многочлен
	for (int i = 0; i < size; i++) {
		encodedOrRealMessage[i] = 0;
		for (int j = 0; j < size; j++) {
			encodedOrRealMessage[i] = modularAdd(encodedOrRealMessage[i], modularMult(firstInverseMatrix[i * size + j], cube[j]));
		}
	}
}