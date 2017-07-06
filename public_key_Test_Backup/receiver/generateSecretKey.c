#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

void generateSecretKey(struct Matrices *matrices) {
	generateModule();
	generateFirstMatrices_rare(matrices->firstMatrix, matrices->firstInverseMatrix);
	generateSecondMatrices(matrices->secondMatrix, matrices->secondInverseMatrix);
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

void generateFirstMatrices(ulong *firstMatrix, ulong *firstInverseMatrix) {
	ulong matrixUp[size * size];
	ulong matrixDown[size * size];

	ulong h = 1;
	//ulong det = 1;

	while (h) {
		h = 0; //det = 1;
		getRandTriangleMatrix(matrixDown, 0);
		getRandTriangleMatrix(matrixUp, 1);
		modularMatrixMult(matrixDown, matrixUp, firstMatrix);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (!cube(firstMatrix[i * size + j])) {
					h++;
				}
			}
		}
		if (h) {
			continue;
		}
		// for (int i = 0; i < size; i++) {
		// 	det = modularMult(det, matrixUp[i * size + i]);
		// }
		// if ((det == 0) || (gcd(det, moduleStruct.module) != 1)) {
		// 	h++;
		// }
	}

	computeInverseMatrix(matrixDown, matrixUp, firstInverseMatrix);
}

void generateSecondMatrices(ulong *secondMatrix, ulong *secondInverseMatrix) {
	ulong matrixUp[size * size];
	ulong matrixDown[size * size];

	ulong h = 1;
	//ulong det = 1;

	while (h) {
		h = 0;// det = 1;
		getRandTriangleMatrix(matrixDown, 0);
		getRandTriangleMatrix(matrixUp, 1);
		modularMatrixMult(matrixDown, matrixUp, secondMatrix);
		// for (i = 0; i < size; i++) {
		// 	det = modularMult(det, matrixUp[i * size + i]);
		// }
		// if ((det == 0) || (gcd(det, moduleStruct.module) != 1)) {
		// 	h++;
		// }
	}

	computeInverseMatrix(matrixDown, matrixUp, secondInverseMatrix);
}

void computeInverseMatrix(ulong *matrixDown, ulong *matrixUp, ulong *inverseMatrix) {
	ulong sum;
	for (int i = size - 1; i >= 0; i--) {
		for (int j = size - 1; j >= 0; j--) {
			sum = 0;
			if (i == j) {
				for (int k = j + 1; k < size; k++) {
					sum = modularAdd(sum, modularMult(matrixUp[i * size + k], inverseMatrix[k * size + i]));
				}
				inverseMatrix[i * size + i] = modularDiv(modularSub(1, sum), matrixUp[i * size + i]);
			}
			else if (i < j) {
				for (int k = i + 1; k < size; k++) {
					sum = modularAdd(sum, modularMult(matrixUp[i * size + k], inverseMatrix[k * size + j]));
				}
				inverseMatrix[i * size + j] = modularDiv(modularSub(0, sum), matrixUp[i * size + i]);
			}
			else {
				for (int k = j + 1; k < size; k++) {
					sum = modularAdd(sum, modularMult(matrixDown[k * size + j], inverseMatrix[i * size + k]));
				}
				inverseMatrix[i * size + j] = modularSub(0, sum);
			}
		}
	}
}

void getRandTriangleMatrix(ulong *matrix, uchar dir) {
	// Locale variables declaration
	ulong temp;
	ulong mult = 1;
	ulong preMult = 0;

	// Fill TOP triangle (some trash)
	if (dir)
	{
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < i; j++) {
				matrix[i * size + j] = 0;
			}
			for (int j = i + 1; j < size; j++) {
				matrix[i * size + j] = getRandom(moduleStruct.module);
			}
		}
	}
	// Fill DOWN triangle (some trash)
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < i; j++) {
				matrix[i * size + j] = getRandom(moduleStruct.module);
			}
			for (int j = i + 1; j < size; j++) {
				matrix[i * size + j] = 0;
			}
		}
	}

	// Fill diagonal right numbers
	for (int i = 0; i < size; i++) {
		if (!dir) {
			matrix[i * size + i] = 1;
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
			matrix[i * size + i] = temp;
		}
	}
	return;
}

void generateFirstMatrices_rare(ulong *firstMatrix, ulong *firstInverseMatrix) {
	const ulong N = AMOUNT_OF_VAR_IN_LINE, K = size / N;
	ulong A[K], B[N * N], inv_B[N * N];
	ulong det = 1, temp_det = 0;
	for (ulong i = 0; i < K; i++) {
		A[i] = getRandom(moduleStruct.module - 1) + 1;
		while (gcd(A[i], moduleStruct.module) != 1) {
			A[i] = getRandom(moduleStruct.module - 1) + 1;
		}
		det *= A[i];
	}
	det = modularDeg(det, N);
	do {
		for (ulong i = 0; i < N * N; i++) {
			B[i] = getRandom(moduleStruct.module - 1) + 1;
		}
		temp_det = determinant(B, N);
	} while (gcd(temp_det, moduleStruct.module) != 1);
	ulong /*result[P * P], invert_result[P * P],*/ mult[size * size];
	commonComputeInverseMatrix(B, inv_B, N, temp_det);
	tenzorMult(A, B, firstMatrix, N);
	for (ulong i = 0; i < K; i++) {
		A[i] = modularDiv(1, A[i]);
	}
	tenzorMult(A, inv_B, firstInverseMatrix, N);
	shake(firstMatrix, firstInverseMatrix, size, size);
	// modularMatrixMult(firstMatrix, firstInverseMatrix, mult);
	// printMatrix(firstMatrix, size, size);
	// printMatrix(firstInverseMatrix, size, size);
	// printMatrix(mult, size, size);
}

ulong determinant(ulong *matrix, ulong size) {
	if (size == 1) {
		return matrix[0];
	}
	if (size == 2) {
		return modularSub(modularMult(matrix[0], matrix[3]), modularMult(matrix[1], matrix[2]));
	}
	else {
		ulong i, j = 0, k;
		long long r = 1;
		ulong m = 0;
		ulong *b = (ulong*)calloc(size * size, sizeof(ulong));  // Убрать динамику
		for (i = 0; i < size; i++) {
			if (matrix[i] != 0) {
				k = 0;
				for (j = size; j < size * size; j++) {
					if (j % size != i) {
						b[k] = matrix[j];
						k++;
					}
				}
				if (r > 0) {
					m = modularAdd(m, modularMult(determinant(b, size - 1), matrix[i]));
				}
				else {
					m = modularSub(m, modularMult(determinant(b, size - 1), matrix[i]));
				}
			}
			r = -1 * r;
		}
		free(b);
		return m;
	}
}

void commonComputeInverseMatrix(ulong *inmatrix, ulong* outmatrix, ulong size, ulong det) {
	ulong z = modularDiv(1, det);
	ulong i = 0, j = 0, k = 0, p, n;
	ulong *b = (ulong*)calloc(size * size, sizeof(ulong));
	for (i = 0; i < size * size; i++) {
		k = 0;
		for (j = 0; j < size * size; j++) {
			if (((j % size) != (i % size)) && ((j / size) != (i / size))) {
				b[k] = inmatrix[j];
				k++;
			}
		}
		outmatrix[(i % size)*size + (i / size)] = modularMult(determinant(b, size - 1), z);
		if (((i / size) + i % size) % 2 == 1) {
			outmatrix[(i % size)*size + (i / size)] = modularSub(0, outmatrix[(i % size) * size + (i / size)]);
		}
	}
	return;
}

void tenzorMult(ulong *A, ulong *B, ulong *result, ulong N) {
	for (ulong i = 0; i < size; i++) {
		for ( ulong j = 0; j < size; j++) {
			result[i * size + j] = (i / N == j / N) ? modularMult(A[i / N] , B[(i % N)*N + (j % N)]) : 0;
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