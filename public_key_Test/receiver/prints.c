#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

#ifdef PRINT
	FILE *output;
#endif

void printSecretKey(Matrices matrices) {
	#ifndef PRINT
		return;
	#endif
	output = fopen(PATH_TO_OUTPUT, "w");

	fprintf(output, "SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);
	printf("SIZE: %d\nSIZE_OF_VARIABLE: %d\nSIZE_OF_MODULE: %d\n\n", size, SIZE_OF_VARIABLE, SIZE_OF_MODULE);

	fprintf(output, "Module: %llu = ", moduleStruct.module); printf("Module: %llu = ", moduleStruct.module);

	for (int i = 0; i < moduleStruct.masSize; i++) {
		if (i != 0) {
			printf("*");
			fprintf(output, "*");
		}
		printf("%llu", moduleStruct.partsOfModule[i]);
		fprintf(output, "%llu", moduleStruct.partsOfModule[i]); 
	}
	printf("\n\n");
	fprintf(output, "\n\n"); 

    printf("Matrix A\n");
    printRareMatrix(matrices.firstMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    printf("\n");
    fprintf(output, "Matrix A\n");
    fprintRareMatrix(matrices.firstMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    fprintf(output, "\n");

    printf("Matrix A^(-1)\n");
    printRareMatrix(matrices.firstInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    printf("\n");
    fprintf(output, "Matrix A^(-1)\n");
    fprintRareMatrix(matrices.firstInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    fprintf(output, "\n");

    printf("Matrix B\n");
    printRareMatrix(matrices.secondMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    printf("\n");
    fprintf(output, "Matrix B\n");
    fprintRareMatrix(matrices.secondMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    fprintf(output, "\n");

    printf("Matrix B^(-1)\n");
    printRareMatrix(matrices.secondInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    printf("\n");
    fprintf(output, "Matrix B^(-1)\n");
    fprintRareMatrix(matrices.secondInverseMatrix, size, size, AMOUNT_OF_VAR_IN_LINE_FIRST);
    fprintf(output, "\n");

	printf("Constants: ");
	fprintf(output, "Constants: ");
	for(uint i = 0; i < size; i++){
		printf("%u ", matrices.constants[i]);
		fprintf(output, "%u ", matrices.constants[i]);
	}
	printf("\n\n");
	fprintf(output, "\n\n"); 
}

void printCubePolynomials(CubePolynomial *cubePolynomials) {
	#ifndef PRINT
		return;
	#endif
		
	printf("Cube polynomials\n");
	fprintf(output, "Cube polynomials\n"); 

	printPolynomials(cubePolynomials);
    fprintPolynomials(cubePolynomials); 

    printf("\n");
    fprintf(output, "\n"); 
}

void printPolynomialsAfterAddFunctions(CubePolynomial *polynomials) {
	#ifndef PRINT
		return;
	#endif
		
	printf("Polynomials after adding functions\n");
	fprintf(output, "Polynomials after adding functions\n"); 

	printPolynomials(polynomials);
    fprintPolynomials(polynomials); 

    printf("\n");
    fprintf(output, "\n"); 
}

void printPublicKey(CubePolynomial *publicKey, uint bytes) {
	#ifndef PRINT
		return;
	#endif

	printf("Public key\n");
	fprintf(output, "Public key\n"); 

	printPolynomials(publicKey);
    fprintPolynomials(publicKey); 

    printf("\n");
    fprintf(output, "\n"); 

    printf("Bytes written: %d\n\n", bytes);
	fprintf(output, "Bytes written: %d\n\n", bytes);
}

void printEncodedMessage(ulong *encodedMessage) {
	#ifndef PRINT
		return;
	#endif

	printf("Encoded message by module: ");
	fprintf(output, "Encoded message by module: "); 
	for (int i = 0; i < size; i++) {
		printf("%llu ", encodedMessage[i]);
		fprintf(output, "%llu ", encodedMessage[i]); 
	}
	printf("\n\n");
	fprintf(output, "\n\n"); 
}

void printDecodedMessage(ulong *decodedMessage) {
	#ifndef PRINT
		return;
	#endif

	printf("Decoded message: ");
	fprintf(output, "Decoded message: ");
	for (int i = 0; i < size; i++) {
		printf("%llu ", decodedMessage[i]);
		fprintf(output, "%llu ", decodedMessage[i]);
	}
	printf("\n\n"); 

	fclose(output);
}

void printMatrix(ulong *matrix, uint size1, uint size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			printf("%llu ", matrix[i * size2 + j]);
		}
		printf("\n");
	}
}

void fprintMatrix(ulong *matrix, uint size1, uint size2) {
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			fprintf(output, "%llu ", matrix[i * size2 + j]);
		}
		fprintf(output, "\n");
	}
}

void printPolynomials(CubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		printPolynomial(cubePolynomials[i]);
	}
}

void fprintPolynomials(CubePolynomial *cubePolynomials) {
	for (int i = 0; i < size; i++) {
		fprintPolynomial(cubePolynomials[i]);
	}
}

void printPolynomial(CubePolynomial cubePolynomials){
    for (int j = 0; j < MAX_TERMS_IN_POLY; j++) {
        if (cubePolynomials.factor[j] == 0) {
            continue;
        }
        if (j != 0) {
            printf( " + ");
        }
        printf("%llu", cubePolynomials.factor[j]);
        for (int k = 0; k < 5; k++) {
            printf("*x%lu", getFromVar_test(cubePolynomials.vars, (5 * j) + k));
        }
    }
    printf("\n");
}

void fprintPolynomial(CubePolynomial cubePolynomials){
    for (int j = 0; j < MAX_TERMS_IN_POLY; j++) {
        if (cubePolynomials.factor[j] == 0) {
            continue;
        }
        if (j != 0) {
            fprintf(output, " + ");
        }
        fprintf(output, "%llu", cubePolynomials.factor[j]);
        for (int k = 0; k < 5; k++) {
            fprintf(output, "*x%lu", getFromVar_test(cubePolynomials.vars, (5 * j) + k));
        }
    }
    fprintf(output, "\n");
}

void printRareMatrix(ulong* matrix, int size1, int size2, int amount) {
	int was_printed;
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			was_printed = 0;
			for (int k = 0; k < amount; k++) {
				if (matrix[i*2*amount+2*k] == j) {
					printf("%u ", matrix[i*2*amount+2*k + 1]);
					was_printed = 1;
					break;
				}
			}
			if (!was_printed) {
				printf("0 ");
			}
		}
		printf("\n");
	}
}

void fprintRareMatrix(ulong* matrix, int size1, int size2, int amount) {
	int was_printed;
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			was_printed = 0;
			for (int k = 0; k < amount; k++) {
				if (matrix[i*2*amount+2*k] == j) {
					fprintf(output, "%u ", matrix[i*2*amount+2*k + 1]);
					was_printed = 1;
					break;
				}
			}
			if (!was_printed) {
				fprintf(output, "0 ");
			}
		}
		fprintf(output, "\n");
	}
}