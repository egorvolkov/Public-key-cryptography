#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

void generateSecretKey(struct Matrices *matrices) {
	generateModule();
	generateFirstMatrices(matrices->firstMatrix, matrices->firstInverseMatrix);
	generateSecondMatrices(matrices->secondMatrix, matrices->secondInverseMatrix);
}

void generateModule() {

#ifdef DEBUG
	if (pow(getModules(MODULES - MODULES/10), moduleStruct.masSize) < (double)((ulong)1 << SIZE_OF_VARIABLE)) {
		printf("Too little masSize\n");
		exit(1);
	}
#endif

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
	} while (pow(getModules(MODULES - MODULES/10), moduleStruct.masSize - 1) < (double)((ulong)1 << SIZE_OF_VARIABLE) / k[0] || k[0] >= ((ulong)1 << SIZE_OF_VARIABLE));
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


#ifdef DEBUG
	if (moduleStruct.module < ((ulong)1 << SIZE_OF_VARIABLE) || moduleStruct.module >= ((ulong)1 << SIZE_OF_MODULE)) {
		printf("ERROR OF CREATING A MODULE\n");
		getchar();
	}
#endif
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
