#include "receiver.h"
#include <sys/time.h>
#include <time.h>
extern const uint size;
extern struct Module moduleStruct;
void swap_ulong(ulong *a, ulong *b){
    ulong temp = *a;
    *a = *b;
    *b = temp;
}

void generateVector(ulong *array){
	srand(time(NULL));
	ulong max_amount = 4807;
	ulong selector[max_amount / 2];
	for(uint i = 0; i < max_amount / 2; i++){
		selector[i] = i;
	}
	uint random = 0;
	for(uint i = 0; i < LENGTH_OF_SECRET_VECTOR * 2; i++){
		random = rand() % (max_amount / 2 - i) + i;
    if(gcd(getModules(random), moduleStruct.module) != 1){
      i--;
      continue;
    }
		swap_ulong(&selector[i], &selector[random]);
	}
	for(uint i = 0; i < LENGTH_OF_SECRET_VECTOR * 2; i++){
    array[i / 2] = getModules(selector[i]) * getModules(selector[i + 1]);
	}
	uint min = 0;
	for(uint i = 0; i < LENGTH_OF_SECRET_VECTOR; i++){
		if(array[min] > array[i]){
			min = i;
		}
	}
	swap_ulong(&array[0], &array[min]);
}

void matricesTransposition(ulong *matrices, ulong *transposition){
  for(ulong i = 0 ; i < AMOUNT_OF_VARIABLES; i++){
    transposition[i] = i;
  }
  ulong random = 0;
  for(ulong i = 0; i < AMOUNT_OF_VARIABLES; i++){
    random = getRandom(AMOUNT_OF_VARIABLES - i) + i;
    swap_ulong(&transposition[i], &transposition[random]);
  }
  for(ulong i = 0 ; i < AMOUNT_OF_VARIABLES; i++){
    printf("%llu ", transposition[i]);
  }
  printf("\n");
  for(ulong i = 0 ; i < AMOUNT_OF_VARIABLES; i++){
    	printf("%llu ", matrices[i]);
  	}
  	printf("\n");
  for(ulong i = 0; i < AMOUNT_OF_VARIABLES; i++){
	
    for(ulong j = 0; j < AMOUNT_OF_POLYNOMS; j++){
      ulong tmp = matrices[i + j * AMOUNT_OF_VARIABLES];
      matrices[i + j * AMOUNT_OF_VARIABLES] = matrices[transposition[i] + j * AMOUNT_OF_VARIABLES];
      matrices[transposition[i] + j * AMOUNT_OF_VARIABLES] = tmp;
    }
	for(ulong i = 0 ; i < AMOUNT_OF_VARIABLES; i++){
    	printf("%llu ", matrices[i]);
  	}
  	printf("\n");
  }
}


void generatePows(ulong *result, ulong *radixes, ulong lastPow) {
    //Заносит в массив с указателем array
    //степени числа radix, степень соответствует индексу
    //надо проверить чтобы длина массива была не меньше lastPow
    ulong i = 0;
    for (int j = 0;j < NUMBER_OF_RADIX; j++){
      result[i] = 1;
      i++;
      for (int k = 1; k<=lastPow; k++) {
          result[i] = result[i-1]*radixes[j];
          i++;
      }
    }
}

void generateSecretKey(struct Matrices *matrices, ulong *secretVector, ulong *answers, ulong *transposition, ulong radix) {
    ulong minModule = answers[AMOUNT_OF_VARIABLES * NUMBER_OF_RADIX - 1] * (radix-1) - 1;
	generateModule(minModule);
  //generateVector(secretVector);
  //printf("secretVector : ");
  //for (int i = 0; i < LENGTH_OF_SECRET_VECTOR; i++){
  //  printf("%llu ", secretVector[i]);
  //}
  //printf("\n");
	generateFirstMatrices(matrices->firstMatrix, matrices->firstInverseMatrix, AMOUNT_OF_POLYNOMS, &matrices->firstMatrixDet);
	//matricesTransposition(matrices->firstMatrix, transposition);
	generateSecondMatrices(matrices->secondMatrix, matrices->secondInverseMatrix, AMOUNT_OF_POLYNOMS);
	generateConstants(matrices->constants);
	//for(uint i = 0; i < AMOUNT_OF_POLYNOMS; i++)
		//matrices->constants[i] = 0;
}

void generateModule(ulong minModule) {
	int bit = 0;
	 while (minModule >>= 1){
		bit++;
	 }
	ulong min = (ulong)1 << (bit + 1);
	ulong max = (ulong)1 << (bit + 2);

	ulong k[moduleStruct.masSize];
	ulong i, a = 0, p1, p2;

	if (moduleStruct.masSize == 1) {
		FILE *fin = fopen(PATH_TO_MODULES, "rb");
		i = 0;
		do {
			fread(&a, 4, 1, fin);
			i++;
		} while (min > a);
		p1 = i - 1;

		do {
			fread(&a, 4, 1, fin);
			i++;
			if (i == MODULES) {
				break;
			}
		} while (max > a);
		p2 = i - 2;

		moduleStruct.module = getModules(getRandom(p2 - p1 + 1) + p1);
		moduleStruct.partsOfModule[0] = moduleStruct.module;
		computePartsOfModule();
		fclose(fin);
		return;
	}

	moduleStruct.module = 1;

	i = 0;

	FILE *fin = fopen(PATH_TO_MODULES, "rb");

	do {
		fread(&a, 4, 1, fin);
		i++;
		if (i == MODULES) {
			break;
		}
	} while (((ulong)1 << (bit+2)) > a);
	p1 = i - 2;

	do {
		k[0] = getModules(getRandom(p1));
	} while (pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize - 1) < (double)((ulong)1 << (bit+1)) / k[0] || k[0] >= ((ulong)1 << (bit+1)));
	moduleStruct.module *= k[0];
	moduleStruct.partsOfModule[0] = k[0];

	min = (ulong)ceil(pow((double)((ulong)1 << (bit+1)) / k[0], (double)1 / (moduleStruct.masSize - 1)));
	max = (ulong)floor(pow((double)((ulong)1 << (bit+2)) / k[0], (double)1 / (moduleStruct.masSize - 1)));

	i = 0;
	fseek(fin, 0, SEEK_SET);
	do {
		fread(&a, 4, 1, fin);
		i++;
	} while (min > a);
	p1 = i - 1;

	do {
		fread(&a, 4, 1, fin);
		i++;
		if (i == MODULES) {
			break;
		}
	} while (max > a);
	p2 = i - 2;

	fclose(fin);

	if (p2 - p1 < moduleStruct.masSize - 1) {
		if (p1 < 10) {
			p1 = 0;
		} else {
			p1 -= 10;
		}
		p2 += 10;
		if (p2 >= MODULES) {
			p2 = MODULES - 1;
		}
	}

	do {
		moduleStruct.module = k[0];
		for (int j = 1; j < moduleStruct.masSize; j++) {
			do {
				k[j] = getModules(getRandom(p2 - p1 + 1) + p1);
			} while (inArray(k, j, k[j]));
			moduleStruct.module *= k[j];
			moduleStruct.partsOfModule[j] = k[j];
		}
	} while (moduleStruct.module < ((ulong)1 << (bit+1)) || moduleStruct.module >= ((ulong)1 << (bit+2)));

	computePartsOfModule();
}

void generateConstants(ulong *constants){
	for (uint i = 0; i < AMOUNT_OF_POLYNOMS; i++) {
		constants[i] = getRandom(moduleStruct.module);
	}
}

// int canCreate(int n, ulong min, ulong max) {
// 	int downLine = MODULES/3;
// 	int upLine = MODULES - MODULES/3;

// 	int downMult = 1;

// 	int upMult = 1;

// 	for (int i = 0; i < n; i++) {
// 		downMult = getModules(downLine - i);
// 		upMult = getModules(upLine + i);
// 	}

// 	if (upMult >= min && downMult <= upLine) {
// 		return 1;
// 	}
// 	return 0;
// }
void computeRadixes(ulong *radixes, ulong radix) {
	for (int i = 0; i < NUMBER_OF_RADIX; i++) {
		radixes[i + NUMBER_OF_RADIX * 1] = radix / radixes[i];
	  radixes[i + NUMBER_OF_RADIX * 2] = modularInverseMultUniver(radixes[i + NUMBER_OF_RADIX * 1], radixes[i]);
	}
}
void computePartsOfModule() {
	for (int i = 0; i < moduleStruct.masSize; i++) {
		moduleStruct.partsOfModule[i + moduleStruct.masSize * 1] = moduleStruct.module / moduleStruct.partsOfModule[i];
		moduleStruct.partsOfModule[i + moduleStruct.masSize * 2] = modularInverseMultUniver(moduleStruct.partsOfModule[i + moduleStruct.masSize * 1], moduleStruct.partsOfModule[i]);
	}
}

void generateFirstMatrices(ulong *firstMatrix, ulong *firstInverseMatrix, ulong lines, ulong *det) {
	ulong matrixUp[lines * lines];
	ulong matrixDown[lines * lines];

	ulong h = 1;
	*det = 1;

	while (h) {
		h = 0; //det = 1;
		getRandTriangleMatrix(matrixDown, 0, lines);
		getRandTriangleMatrix(matrixUp, 1, lines);
		modularMatrixMult(matrixDown, matrixUp, firstMatrix, lines);
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < lines; j++) {
				if (!cube(firstMatrix[i * lines + j])) {
					h++;
				}
			}
		}
		if (h) {
			continue;
		}
	}
	for(uint i = 0; i < lines; i++){
		*det = modularMult(*det, matrixUp[i + i * lines]);
		printf("%llu ", *det);
	}
	computeInverseMatrix(matrixDown, matrixUp, firstInverseMatrix,lines);
}

void generateSecondMatrices(ulong *secondMatrix, ulong *secondInverseMatrix, ulong lines) {
	ulong matrixUp[lines * lines];
	ulong matrixDown[lines * lines];

	getRandTriangleMatrix(matrixDown, 0, lines);
	getRandTriangleMatrix(matrixUp, 1, lines);
	modularMatrixMult(matrixDown, matrixUp, secondMatrix, lines);

	computeInverseMatrix(matrixDown, matrixUp, secondInverseMatrix, lines);
}

void computeInverseMatrix(ulong *matrixDown, ulong *matrixUp, ulong *inverseMatrix, ulong lines) {
	ulong sum;
	for (int i = lines - 1; i >= 0; i--) {
		for (int j = lines - 1; j >= 0; j--) {
			sum = 0;
			if (i == j) {
				for (int k = j + 1; k < lines; k++) {
					sum = modularAdd(sum, modularMult(matrixUp[i * lines + k], inverseMatrix[k * lines + i]));
				}
				inverseMatrix[i * lines + i] = modularDiv(modularSub(1, sum), matrixUp[i * lines + i]);
			}
			else if (i < j) {
				for (int k = i + 1; k < lines; k++) {
					sum = modularAdd(sum, modularMult(matrixUp[i * lines + k], inverseMatrix[k * lines + j]));
				}
				inverseMatrix[i * lines + j] = modularDiv(modularSub(0, sum), matrixUp[i * lines + i]);
			}
			else {
				for (int k = j + 1; k < lines; k++) {
					sum = modularAdd(sum, modularMult(matrixDown[k * lines + j], inverseMatrix[i * lines + k]));
				}
				inverseMatrix[i * lines + j] = modularSub(0, sum);
			}
		}
	}
}

void getRandTriangleMatrix(ulong *matrix, uchar dir,ulong lines) {
	// Locale variables declaration
	ulong temp;
	ulong mult = 1;
	ulong preMult = 0;

	// Fill TOP triangle (some trash)
	if (dir)
	{
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < i; j++) {
				matrix[i * lines + j] = 0;
			}
			for (int j = i + 1; j < lines; j++) {
				matrix[i * lines + j] = getRandom(moduleStruct.module);
			}
		}
	}
	// Fill DOWN triangle (some trash)
	else {
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < i; j++) {
				matrix[i * lines + j] = getRandom(moduleStruct.module);
			}
			for (int j = i + 1; j < lines; j++) {
				matrix[i * lines + j] = 0;
			}
		}
	}

	// Fill diagonal right numbers
	for (int i = 0; i < lines; i++) {
		if (!dir) {
			matrix[i * lines + i] = 1;
		}
		else {
			temp = getRandom(moduleStruct.module - 1) + 1;    // exclude zero
			preMult = modularMult(mult, temp);

			// Choose right number
			while (gcd(preMult, moduleStruct.module) != 1) {
				temp = (temp + 1) % moduleStruct.module;
				preMult = modularMult(mult, temp);
			}

			mult = preMult;
			matrix[i * lines + i] = temp;
		}
	}
	return;
}

void generateFirstMatrices_rare(ulong *firstMatrix, ulong *firstInverseMatrix) {
	const ulong N = AMOUNT_OF_VAR_IN_LINE_FIRST, K = size / N;
	ulong A[K], B[N * N], inv_B[N * N];
	do
	{
		for (ulong i = 0; i < K; i++) {
			A[i] = getRandom(moduleStruct.module - 1) + 1;
			while (gcd(A[i], moduleStruct.module) != 1) {
				A[i] = getRandom(moduleStruct.module - 1) + 1;
			}
		}
		generateSecondMatrices(B,inv_B,N);
	}
	while (tenzorMult(A, B, firstMatrix, N, 1));
	for (ulong i = 0; i < K; i++) {
		A[i] = modularDiv(1, A[i]);
	}
	tenzorMult(A, inv_B, firstInverseMatrix, N, 0);
	shake(firstMatrix, firstInverseMatrix, size, size);
}

void generateSecondMatrices_rare(ulong *secondMatrix, ulong *secondInverseMatrix) {
	const ulong N = AMOUNT_OF_VAR_IN_LINE_SECOND, K = size / N;
	ulong A[K], B[N * N], inv_B[N * N];
	for (ulong i = 0; i < K; i++) {
		A[i] = getRandom(moduleStruct.module - 1) + 1;
		while (gcd(A[i], moduleStruct.module) != 1) {
			A[i] = getRandom(moduleStruct.module - 1) + 1;
		}
	}
	generateSecondMatrices(B,inv_B,N);
	tenzorMult(A, B, secondMatrix, N, 0);
	for (ulong i = 0; i < K; i++) {
		A[i] = modularDiv(1, A[i]);
	}
	tenzorMult(A, inv_B, secondInverseMatrix, N, 0);
	shake(secondMatrix, secondInverseMatrix, size, size);
}

uchar tenzorMult(ulong *A, ulong *B, ulong *result, ulong N, uchar check) {
	if (check){
		for (ulong i = 0; i < size; i++) {
			for ( ulong j = 0; j < size; j++) {
				result[i * size + j] = (i / N == j / N) ? modularMult(A[i / N] , B[(i % N)*N + (j % N)]) : 0;
				if (!cube(result[i * size + j]) && (i / N == j / N)){
					return 1;
				}
			}
		}
		return 0;
	}
	else{
		for (ulong i = 0; i < size; i++) {
			for ( ulong j = 0; j < size; j++) {
				result[i * size + j] = (i / N == j / N) ? modularMult(A[i / N] , B[(i % N)*N + (j % N)]) : 0;
			}
		}
	}
}

void swap(ulong *line1, ulong *line2, ulong length) {
	ulong temp;
	for (ulong i = 0; i < length; i++) {
		temp = line1[i];
		line1[i] = line2[i];
		line2[i] = temp;
	}
}

void shake(ulong *matrix, ulong *invert_matrix, ulong lines, ulong columns) {
	ulong temp[(columns < lines ? lines : columns)];
	ulong i;
	for (i = 0; i < lines; i++) {
		temp[i] = i;
	}
	for (i = 0; i < lines - 1; i++) {
		ulong num = getRandom(lines - i) + i;
		ulong k = temp[i];
		temp[i] = temp[num];
		temp[num] = k;
	}
	for (i = 0; i < lines - 1; i++) {
		swap(&matrix[i * size], &matrix[temp[i] * size], size);
		for (ulong j = 0; j < size; j++) {
			ulong k = invert_matrix[j * size + i];
			invert_matrix[j * size + i] = invert_matrix[j * size + temp[i]];
			invert_matrix[j * size + temp[i]] = k;
		}
	}
	for (i = 0; i < columns; i++) {
		temp[i] = i;
	}
	for (i = 0; i < columns - 1; i++) {
		ulong num = getRandom(columns - i) + i;
		ulong k = temp[i];
		temp[i] = temp[num];
		temp[num] = k;
	}
	for (i = 0; i < columns - 1; i++) {
		swap(&invert_matrix[i * size], &invert_matrix[temp[i] * size], size);
		for (ulong j = 0; j < size; j++) {
			ulong k = matrix[j * size + i];
			matrix[j * size + i] = matrix[j * size + temp[i]];
			matrix[j * size + temp[i]] = k;
		}
	}
}
