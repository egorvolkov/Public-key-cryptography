#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

void generateSecretKey(struct Matrices *matrices, struct NewMatrices *newMatrices) {
	generateModule();
	generateFirstMatrices_rare(newMatrices->firstMatrix, newMatrices->firstInverseMatrix);
	generateSecondMatrices_rare(newMatrices->secondMatrix, newMatrices->secondInverseMatrix);
	generateConstants(newMatrices->constants);
	// for (ulong i = 0; i < AMOUNT_OF_VARIABLES; i++){
	// 	newMatrices->constants[i] = matrices->constants[i];
	// }
	// ulong i, j, countFirst = 0, countInvFirst = 0, countSecond = 0, countInvSecond = 0;
	// for (i = 0; i < AMOUNT_OF_VARIABLES; i++){
	// 	for (j = 0; j < AMOUNT_OF_VARIABLES; j++){
	// 		if (matrices->firstMatrix[AMOUNT_OF_VARIABLES * i + j] != 0){
	// 			newMatrices->firstMatrix[countFirst] = j;
	// 			newMatrices->firstMatrix[countFirst + 1] = matrices->firstMatrix[AMOUNT_OF_VARIABLES * i + j];
	// 			countFirst += 2;
	// 		}
	// 		if (matrices->firstInverseMatrix[AMOUNT_OF_VARIABLES * i + j] != 0){
	// 			newMatrices->firstInverseMatrix[countInvFirst] = j;
	// 			newMatrices->firstInverseMatrix[countInvFirst + 1] = matrices->firstInverseMatrix[AMOUNT_OF_VARIABLES * i + j];
	// 			countInvFirst += 2;
	// 		}
	// 		if (matrices->secondMatrix[AMOUNT_OF_VARIABLES * i + j] != 0){
	// 			newMatrices->secondMatrix[countSecond] = j;
	// 			newMatrices->secondMatrix[countSecond + 1] = matrices->secondMatrix[AMOUNT_OF_VARIABLES * i + j];
	// 			countSecond += 2;
	// 		}
	// 		if (matrices->secondInverseMatrix[AMOUNT_OF_VARIABLES * i + j] != 0){
	// 			newMatrices->secondInverseMatrix[countInvSecond] = j;
	// 			newMatrices->secondInverseMatrix[countInvSecond + 1] = matrices->secondInverseMatrix[AMOUNT_OF_VARIABLES * i + j];
	// 			countInvSecond += 2;
	// 		}
	// 	}
	// }
}

void generateModule() {

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
	} while (pow(getModules(MODULES - MODULES / 10), moduleStruct.masSize - 1) < (double)((ulong)1 << SIZE_OF_VARIABLE) / k[0] || k[0] >= ((ulong)1 << SIZE_OF_VARIABLE));
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
			} while (inArray(k, j, k[j]));
			moduleStruct.module *= k[j];
			moduleStruct.partsOfModule[j] = k[j];
		}
	} while (moduleStruct.module < ((ulong)1 << SIZE_OF_VARIABLE) || moduleStruct.module >= ((ulong)1 << SIZE_OF_MODULE));

	computePartsOfModule();
}

void generateConstants(ulong *constants){
	for (uint i = 0; i < size; i++) {
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

void computePartsOfModule() {
    for (int i = 0; i < moduleStruct.masSize; i++) {
        moduleStruct.partsOfModule[i + moduleStruct.masSize * 1] = moduleStruct.module / moduleStruct.partsOfModule[i];
        moduleStruct.partsOfModule[i + moduleStruct.masSize * 2] = modularInverseMultUniver(moduleStruct.partsOfModule[i + moduleStruct.masSize * 1], moduleStruct.partsOfModule[i]);
    }
}

// void generateFirstMatrices(ulong *firstMatrix, ulong *firstInverseMatrix, ulong lines) {
// 	ulong LUmatrices[lines * lines];

// 	ulong h = 1;
// 	//ulong det = 1;

// 	while (h) {
// 		h = 0; //det = 1;
// 		getRandTriangleMatrix(LUmatrices, 0, lines);
// 		getRandTriangleMatrix(LUmatrices, 1, lines);
// 		modularTriangleMatrixMult(LUmatrices, firstMatrix, lines);
// 		for (int i = 0; i < lines; i++) {
// 			for (int j = 0; j < lines; j++) {
// 				if (!cube(firstMatrix[i * lines + j])) {
// 					h++;
// 				}
// 			}
// 		}
// 		if (h) {
// 			continue;
// 		}
// 	}
// 	computeInverseMatrix(LUmatrices, firstInverseMatrix,lines);
// }

void generateSecondMatrices(ulong *secondMatrix, ulong *secondInverseMatrix, ulong lines) {
	ulong LUmatrices[lines * lines];

	getNewRandTriangleMatrix(LUmatrices, 2, lines);

	modularTriangleMatrixMult(LUmatrices, secondMatrix, lines);

	computeInverseMatrix(LUmatrices, secondInverseMatrix, lines);
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

void getRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines) {
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

/*
 * Функция для генерации рандомных треугольных матриц. Параметры:
 * matrix - матрица для записи треугольно матрицы;
 * lines - размер матрицы;
 * dir - флаг:
 *      = 0 - генерация нижнетреугольный матрицы
 *      = 1 - генерация верхнетреугольной матрицы
 *      иные значения - генерация обеих треугольных матриц с записью в одну квадратную
 */
void getNewRandTriangleMatrix(ulong *matrix, uchar dir, ulong lines) {
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
	generateSecondMatrices(B, inv_B, N);
	}
	while (tenzorMult(A, B, firstMatrix, N, 1));
	tenzorMult(A, B, firstMatrix, N, 1);
	for (ulong i = 0; i < K; i++) {
		A[i] = modularDiv(1, A[i]);
	}
	tenzorMult(A, inv_B, firstInverseMatrix, N, 0);
	shake(firstMatrix, firstInverseMatrix, size, 2 * AMOUNT_OF_VAR_IN_LINE_FIRST);
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
	shake(secondMatrix, secondInverseMatrix, size, 2 * AMOUNT_OF_VAR_IN_LINE_SECOND);
}

uchar tenzorMult(ulong *A, ulong *B, ulong *result, ulong N, uchar check) {
	int cur = 0;
	if (check){
		for (ulong i = 0; i < size; i++) {
			cur = 0;
			for (ulong j = 0; j < size; j++) {
				if (i / N == j / N) {
					result[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + cur] = j;
					result[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + cur + 1] = modularMult(A[i / N] , B[(i % N)*N + (j % N)]);
					cur += 2;
				}
				if ((i / N == j / N) && !cube(result[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + cur + 1])){
					return 1;
				}
			}
		}
		return 0;
	}
	else {
		for (ulong i = 0; i < size; i++) {
			cur = 0;
			for (ulong j = 0; j < size; j++) {
				if (i / N == j / N) {
					result[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + cur] = j;
					result[i * 2 * AMOUNT_OF_VAR_IN_LINE_FIRST + cur + 1] = modularMult(A[i / N] , B[(i % N)*N + (j % N)]);
					cur += 2;
				}
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
		swap(&matrix[i * columns], &matrix[temp[i] * columns], columns);
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < columns; k += 2) {
				if (invert_matrix[j*columns + k] == i) {
					invert_matrix[j*columns + k] = temp[i];
				} else {
					if (invert_matrix[j*columns + k] == temp[i]) {
						invert_matrix[j*columns + k] = i;
					}
				}
			}
		}
		// for (ulong j = 0; j < size; j++) {
		// 	ulong k = invert_matrix[j * size + i];
		// 	invert_matrix[j * size + i] = invert_matrix[j * size + temp[i]];
		// 	invert_matrix[j * size + temp[i]] = k;
		// }
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
		swap(&invert_matrix[i * columns], &invert_matrix[temp[i] * columns], columns);
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < columns; k += 2) {
				if (matrix[j*columns + k] == i) {
					matrix[j*columns + k] = temp[i];
				} else {
					if (matrix[j*columns + k] == temp[i]) {
						matrix[j*columns + k] = i;
					}
				}
			}
		}
		// for (ulong j = 0; j < size; j++) {
		// 	ulong k = matrix[j * size + i];
		// 	matrix[j * size + i] = matrix[j * size + temp[i]];
		// 	matrix[j * size + temp[i]] = k;
		// }
	}
}
