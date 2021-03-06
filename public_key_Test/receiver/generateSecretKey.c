#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

void generateSecretKey(struct Matrices *matrices) {
	generateModule();
	generateFirstMatrices_rare(matrices->firstMatrix, matrices->firstInverseMatrix);
	generateSecondMatrices_rare(matrices->secondMatrix, matrices->secondInverseMatrix);
	generateConstants(matrices->constants);
}

void generateModule() {
	//printf("%d %u %lf\n", MODULES - MODULES / 10, getModules(MODULES - MODULES / 10), (pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize)));
	// if ((pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize) < (double)((ulong)1 << SIZE_OF_VARIABLE))/* && (getModules(MODULES - MODULES / 10) < 6200)*/) {
	// 	printf("Too little masSize. Press Enter to exit.\n");
	// 	getchar();
	// 	exit(1);
	// }

	// const int masSize = moduleStruct.masSize;
	// ulong min[masSize];
	// ulong max[masSize];

	// min[masSize - 1] = (ulong)1 << SIZE_OF_VARIABLE;
	// max[masSize - 1] = (ulong)1 << SIZE_OF_MODULE;

	// ulong modules = 0;
	// ulong a = 0;
	// ulong b = 0;

	// uint buf;
	// FILE *fin = fopen(PATH_TO_MODULES, "rb");
	// for (int i = masSize - 2; i >= 0; i--) {
	// 	fseek(fin, 0, SEEK_SET);
	// 	modules = 0;
	// 	do {
	// 		fread(&buf, 4, 1, fin);
	// 		if (buf > max[i + 1]) {
	// 			break;
	// 		}
	// 		modules++;
	// 	} while (modules < MODULES);
	// 	a = getModules(modules/10);
	// 	b = getModules(modules - modules/10);
	// 	min[i] = min[i + 1] / b;
	// 	max[i] = max[i + 1] / a;
	// 	printf("%llu %llu\n", min[i], max[i]);
	// }
	// modules = 0;
	// do {
	// 	fread(&buf, 4, 1, fin);
	// 	if (buf > max[masSize - 1]) {
	// 		break;
	// 	}
	// 	modules++;
	// } while (modules < MODULES);
	// fclose(fin);

	// moduleStruct.module = 1;
	// for (int i = 0; i < masSize; i++) {
	// 	do {
	// 		moduleStruct.partsOfModule[i] = getModules(getRandom(modules));
	// 	} while (moduleStruct.module * moduleStruct.partsOfModule[i] >= max[i] || moduleStruct.module * moduleStruct.partsOfModule[i] < min[i] || inArray(moduleStruct.partsOfModule, i, moduleStruct.partsOfModule[i]));
	// 	printf("%llu ", moduleStruct.partsOfModule[i]);
	// 	moduleStruct.module *= moduleStruct.partsOfModule[i];
	// }
	// //printf("\n");

	// computePartsOfModule();






	if (pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize) < (double)((ulong)1 << SIZE_OF_VARIABLE)) {
		printf("Too little masSize. Press Enter to exit.\n");
		getchar();
		exit(1);
	}

	ulong min = (ulong)1 << SIZE_OF_VARIABLE;
	ulong max = (ulong)1 << SIZE_OF_MODULE;
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

    	moduleStruct.inverseDegree[0] = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
    	moduleStruct.inverseDegree[1] = modularInverseMultUniver(5, euler(moduleStruct.module)); //  Считаем степень, которая будет соответствовать корнюы 5 степени из числа
    	
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
	} while (((ulong)1 << SIZE_OF_MODULE) > a);
	p1 = i - 2;

	do {
		k[0] = getModules(getRandom(p1));
	} while (/*((k[0] - 1) % 5 == 0) || */pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize - 1) < (double)((ulong)1 << SIZE_OF_VARIABLE) / k[0] || k[0] >= ((ulong)1 << SIZE_OF_VARIABLE));
	moduleStruct.module *= k[0];
	moduleStruct.partsOfModule[0] = k[0];

	min = (ulong)ceil(pow((double)((ulong)1 << SIZE_OF_VARIABLE) / k[0], (double)1 / (moduleStruct.masSize - 1)));
	max = (ulong)floor(pow((double)((ulong)1 << SIZE_OF_MODULE) / k[0], (double)1 / (moduleStruct.masSize - 1)));

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
			} while (/*((k[j] - 1) % 5 == 0) || */inArray(k, j, k[j]));
			moduleStruct.module *= k[j];
			moduleStruct.partsOfModule[j] = k[j];
		}
	} while (moduleStruct.module < ((ulong)1 << SIZE_OF_VARIABLE) || moduleStruct.module >= ((ulong)1 << SIZE_OF_MODULE));

    moduleStruct.inverseDegree[0] = modularInverseMultUniver(3, euler(moduleStruct.module));	//	Считаем степень, которая будет соответствовать кубическому корню из числа
    moduleStruct.inverseDegree[1] = modularInverseMultUniver(5, euler(moduleStruct.module)); //  Считаем степень, которая будет соответствовать корнюы 5 степени из числа

	computePartsOfModule();
}

void generateConstants(ulong *constants){
	for (uint i = 0; i < size; i++) {
		constants[i] = getRandom(moduleStruct.module);
	}
}

void computePartsOfModule() {
    for (int i = 0; i < moduleStruct.masSize; i++) {
        moduleStruct.partsOfModule[i + moduleStruct.masSize * 1] = moduleStruct.module / moduleStruct.partsOfModule[i];
        moduleStruct.partsOfModule[i + moduleStruct.masSize * 2] = modularInverseMultUniver(moduleStruct.partsOfModule[i + moduleStruct.masSize * 1], moduleStruct.partsOfModule[i]);
    }
}

void generateSimpleMatrix(ulong *matrix, ulong *inverseMatrix, ulong lines) {
	ulong LUmatrices[lines * lines];

	getRandTriangleMatrix(LUmatrices, 2, lines);

	modularTriangleMatrixMult(LUmatrices, matrix, lines);

	computeInverseMatrix(LUmatrices, inverseMatrix, lines);
}

void computeInverseMatrix(ulong *LUmatrices, ulong *inverseMatrix, ulong lines) {
	ulong sum;
	for (int i = lines - 1; i >= 0; i--) {
		for (int j = lines - 1; j >= 0; j--) {
			sum = 0;
			if (i == j) {
				for (int k = j + 1; k < lines; k++) {
					sum = modularAdd(sum, modularMult(LUmatrices[i * lines + k], inverseMatrix[k * lines + i]));
				}
				inverseMatrix[i * lines + i] = modularDiv(modularSub(1, sum), LUmatrices[i * lines + i]);
			}
			else if (i < j) {
				for (int k = i + 1; k < lines; k++) {
					sum = modularAdd(sum, modularMult(LUmatrices[i * lines + k], inverseMatrix[k * lines + j]));
				}
				inverseMatrix[i * lines + j] = modularDiv(modularSub(0, sum), LUmatrices[i * lines + i]);
			}
			else {
				for (int k = j + 1; k < lines; k++) {
					if (k == j) {
						sum = modularAdd(sum, inverseMatrix[i * lines + k]);
					} else {
						sum = modularAdd(sum, modularMult(LUmatrices[k * lines + j], inverseMatrix[i * lines + k]));
					}
				}
				inverseMatrix[i * lines + j] = modularSub(0, sum);
			}
		}
	}
}

/*
 * Функция для генерации рандомных треугольных матриц. Параметры:
 * matrix - матрица для записи треугольно матрицы;
 * lines - размер матрицы;
 * dir - флаг:
 *      = 0 - генерация нижнетреугольный матрицы
 *      = 1 - генерация верхнетреугольной матрицы
 *      иные значения - генерация обеих треугольных матриц с записью в одну квадратную
 */
void getRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines) {
    // Locale variables declaration
    ulong temp;
    ulong mult = 1;
    ulong preMult = 0;

    switch (dir){
        case 0:
            for (int i = 0; i < lines; i++) {
                for (int j = 0; j < i; j++) {
                    matrix[i * lines + j] = getRandom(moduleStruct.module);
                }
                for (int j = i + 1; j < lines; j++) {
                    matrix[i * lines + j] = 0;
                }
            }
            for (int i = 0; i < lines; i++) {
                matrix[i * lines + i] = 1;
            }
            break;
        case 1:
            for (int i = 0; i < lines; i++) {
                for (int j = 0; j < i; j++) {
                    matrix[i * lines + j] = 0;
                }
                for (int j = i + 1; j < lines; j++) {
                    matrix[i * lines + j] = getRandom(moduleStruct.module);
                }
            }
            for (int i = 0; i < lines; i++) {
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
            break;
        default:
            for (int i = 0; i < lines; i++) {
                for (int j = 0; j < lines; j++) {
                    matrix[i * lines + j] = getRandom(moduleStruct.module);
                }
            }
            for (int i = 0; i < lines; i++) {
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
            break;
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
		generateSimpleMatrix(B, inv_B, N);
	}
	while (tenzorMult(A, B, firstMatrix, N, 1, 1, 1));
	for (ulong i = 0; i < K; i++) {
		A[i] = modularDiv(1, A[i]);
	}
	tenzorMult(A, inv_B, firstInverseMatrix, N, 1, 0, 0);
	shake(firstMatrix, firstInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
}

uchar tenzorMult(ulong *A, ulong *B, ulong *result, ulong N, uchar numberOfMatrix, uchar checkCube, uchar checkZero) {
    int cur = 0;
    ulong rare;
    if (numberOfMatrix == 1) {
    	rare = AMOUNT_OF_VAR_IN_LINE_FIRST;
    } else {
    	rare = AMOUNT_OF_VAR_IN_LINE_SECOND;
    }
    for (ulong i = 0; i < size; i++) {
        cur = 0;
        for (ulong j = 0; j < size; j++) {
            if (i / N == j / N) {
                result[i * 2 * rare + cur] = j;
                result[i * 2 * rare + cur + 1] = modularMult(A[i / N] , B[(i % N)*N + (j % N)]);
                if ((!cube(result[i * 2 * rare + cur + 1]) && checkCube) || ((result[i * 2 * rare + cur + 1] == 0) && checkZero)){
                    return 1;
                }
                cur += 2;
            }
        }
    }
    return 0;
}

void generateSecondMatrices_rare(ulong *secondMatrix, ulong *secondInverseMatrix) {
    const ulong N = AMOUNT_OF_VAR_IN_LINE_SECOND, K = size / N;
    ulong A[K], B[N * N], inv_B[N * N];
    do
    {
        for (ulong i = 0; i < K; i++) {
            A[i] = getRandom(moduleStruct.module - 1) + 1;
            while (gcd(A[i], moduleStruct.module) != 1) {
                A[i] = getRandom(moduleStruct.module - 1) + 1;
            }
        }
		generateSimpleMatrix(B,inv_B,N);
    }
    while (tenzorMult(A, B, secondMatrix, N, 2, 0, 1));
    for (ulong i = 0; i < K; i++) {
        A[i] = modularDiv(1, A[i]);
    }
    tenzorMult(A, inv_B, secondInverseMatrix, N, 2, 0, 0);
    shake(secondMatrix, secondInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_SECOND);
}

void swap(ulong *line1, ulong *line2, ulong length) {
	ulong temp;
	for (ulong i = 0; i < length; i++) {
		temp = line1[i];
		line1[i] = line2[i];
		line2[i] = temp;
	}
}

void shake(ulong *matrix, ulong *invert_matrix, ulong lines, ulong columns, ulong rare) {
	// ulong temp[(columns < lines ? lines : columns)];
	// ulong i;
	// for (i = 0; i < lines; i++) {
	// 	temp[i] = i;
	// }
	// for (i = 0; i < lines - 1; i++) {
	// 	ulong num = getRandom(lines - i) + i;
	// 	ulong k = temp[i];
	// 	temp[i] = temp[num];
	// 	temp[num] = k;
	// }
	// for (i = 0; i < lines - 1; i++) {
	// 	swap(&matrix[i * columns], &matrix[temp[i] * columns], columns);
	// 	for (int j = 0; j < size; j++) {
	// 		for (int k = 0; k < columns; k += 2) {
	// 			if (invert_matrix[j*columns + k] == i) {
	// 				invert_matrix[j*columns + k] = temp[i];
	// 			} else {
	// 				if (invert_matrix[j*columns + k] == temp[i]) {
	// 					invert_matrix[j*columns + k] = i;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// for (i = 0; i < lines; i++) {
	// 	temp[i] = i;
	// }
	// for (i = 0; i < lines - 1; i++) {
	// 	ulong num = getRandom(lines - i) + i;
	// 	ulong k = temp[i];
	// 	temp[i] = temp[num];
	// 	temp[num] = k;
	// }
	// for (i = 0; i < lines - 1; i++) {
	// 	swap(&invert_matrix[i * columns], &invert_matrix[temp[i] * columns], columns);
	// 	for (int j = 0; j < size; j++) {
	// 		for (int k = 0; k < columns; k += 2) {
	// 			if (matrix[j*columns + k] == i) {
	// 				matrix[j*columns + k] = temp[i];
	// 			} else {
	// 				if (matrix[j*columns + k] == temp[i]) {
	// 					matrix[j*columns + k] = i;
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	ulong randIndex;
	for (int i = 0; i < lines; i++) {
		do {
			randIndex = getRandom(lines);
		} while (randIndex == i);
		swap(matrix + i*2*rare, matrix + randIndex*2*rare, 2*rare);
		for (int j = 0; j < lines; j++) {
			for (int k = 0; k < 2*rare; k+=2) {
				if (invert_matrix[j*2*rare + k] == i) {
					invert_matrix[j*2*rare + k] = randIndex;
				} else {
					if (invert_matrix[j*2*rare + k] == randIndex) {
						invert_matrix[j*2*rare + k] = i;
					}
				}
			}
		}
	}

	for (int i = 0; i < columns; i++) {
		do {
			randIndex = getRandom(columns);
		} while (randIndex == i);
		swap(invert_matrix + i*2*rare, invert_matrix + randIndex*2*rare, 2*rare);
		for (int j = 0; j < lines; j++) {
			for (int k = 0; k < 2*rare; k+=2) {
				if (matrix[j*2*rare + k] == i) {
					matrix[j*2*rare + k] = randIndex;
				} else {
					if (matrix[j*2*rare + k] == randIndex) {
						matrix[j*2*rare + k] = i;
					}
				}
			}
		}
	}
}
