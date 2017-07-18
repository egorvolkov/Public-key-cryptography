#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void matricesDeTransposition(ulong *matrices, ulong *transposition){
  for(int i = AMOUNT_OF_VARIABLES - 1; i >= 0; i--){
    for(ulong j = 0; j < 1; j++){
      ulong tmp = matrices[i + j * AMOUNT_OF_VARIABLES];
      matrices[i + j * AMOUNT_OF_VARIABLES] = matrices[transposition[i] + j * AMOUNT_OF_VARIABLES];
      matrices[transposition[i] + j * AMOUNT_OF_VARIABLES] = tmp;
    }
  }
}
void decoding(ulong *secondInverseMatrix, ulong *encodedMessage, ulong *realMessage, ulong *constants, ulong *secretVector, ulong *radixes, ulong *transposition, ulong radix) {
	ulong cube[AMOUNT_OF_POLYNOMS];
	ulong inverseDegree = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
#ifdef PRINT
	fprintf(output, "InversedDegree: %llu\n", inverseDegree); printf("inverseDegree: %llu\n", inverseDegree);
	fprintf(output, "Cube: "); printf("Cube: ");
#endif
	for (int i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		cube[i] = 0;
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
		printf("%llu ", cube[i]);
	}
	fprintf(output, "\n"); printf("\n");
#endif
	//	Умножаем обратную первую матрицу на начальный. Получаем те значения, который пользователь подставлял в многочлен

	/* СКАЛЯРНОЕ ПРОИЗВЕДЕНИЕ*/
	ulong pre_result[NUMBER_OF_RADIX];
	ulong result;
	scalar(cube,secretVector,pre_result,LENGTH_OF_SECRET_VECTOR);//получаем числа из систем счисления, после чего пользуемся китайской теоремой об остатках.
  printf("pre_result: ");
  for (int i = 0; i < NUMBER_OF_RADIX; i++) {
    printf("%llu ", pre_result[i]);
  }
  fprintf(output, "\n"); printf("\n");

	translateFromDecimal(pre_result, radixes, realMessage, radixes, radix, transposition);
  matricesDeTransposition(realMessage, transposition);
	//matricesDeTransposition(realMessage, transposition);
	// for (int i = 0; i < size; i++) {
	// 	encodedMessage[i] = 0;
	// 	for (int j = 0; j < size; j++) {
	// 		encodedMessage[i] = modularAdd(encodedMessage[i], modularMult(firstInverseMatrix[i * size + j], cube[j]));
	// 	}
	// }
}
void chyna(ulong *pre_result, ulong *radixes, ulong radix, ulong *result){
	*result = 0;
	for (int i = 0; i < NUMBER_OF_RADIX; i++)
		*result = modularAddUniver(*result,modularMultUniver(modularMultUniver(pre_result[i],radixes[i + NUMBER_OF_RADIX],radix), radixes[i+ 2 * NUMBER_OF_RADIX],radix),radix);
}
void scalar(ulong *a, ulong *b, ulong *result, ulong N){
	for (int j = 0; j < NUMBER_OF_RADIX; j++){
		result[j] = 0;
  	for (int i = 1; i < N; i++){
	    result[j] = modularAdd(result[j], modularMult(a[i + j*LENGTH_OF_SECRET_VECTOR], b[i]));
	  }
		result[j] = modularAdd(result[j], modularInverseAdd(modularMult(a[j*LENGTH_OF_SECRET_VECTOR],b[0])));
	}
}
void translateFromDecimal(ulong* decimals, ulong *systems, ulong *result, ulong *radixes, ulong radix, ulong *transposition) {
	ulong temp[NUMBER_OF_RADIX];
    for(int k = 0; k < AMOUNT_OF_VARIABLES; k++){
		for (int i = 0; i < NUMBER_OF_RADIX; i++){
        	temp[i] = decimals[i] % systems[i];
		}
		chyna(temp, radixes, radix, &result[k]);
		for (int i = 0; i < NUMBER_OF_RADIX; i++){
					decimals[i] -= result[k];
        	decimals[i] /= systems[i];
    	}
	}
}
