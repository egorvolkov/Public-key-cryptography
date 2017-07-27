#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *inverseSecondMatrix, CubePolynomial *newPublicKey, ulong *constants, ulong *constants3){
	CubePolynomial cubePolynomials[size];
    cubeOfPolynomials(firstMatrix, cubePolynomials, constants, inverseSecondMatrix);
	printCubePolynomials(cubePolynomials);
    addFunctions(firstMatrix, constants, cubePolynomials, inverseSecondMatrix);
    printPolynomialsAfterAddFunctions(cubePolynomials);
	multToSecondMatrix(secondMatrix, cubePolynomials, newPublicKey, constants3);
}

void cubeOfPolynomials(ulong *matrix, CubePolynomial cubePolynomials[], ulong *constants, ulong *inverseSecondMatrix) {
	CubePolynomial *buf;
	ulong polynom[size + 1];
    uint pow = 0;
	for (uint i = 0; i < size; i++) {
        // for (uint j = 0; j < size + 1; j++){
        //     polynom[j] = 0;
        // }
        memset(polynom, 0, 8*(size + 1));
		for (uint j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_FIRST;){
			ulong index = matrix[i*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j];
			ulong number = matrix[i*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j + 1];
			polynom[index] = number;
			j += 2;
		}
        polynom[size] = constants[i];
		buf = &(cubePolynomials[i]);
        polynomialDeg(polynom, buf, ((inverseSecondMatrix[pow * 2 + 1] % 2 == 1) ? 5 : 3));
        pow++;
        if (pow == size * AMOUNT_OF_VAR_IN_LINE_SECOND) {
            pow = 0;
        }
	}
}

void multToSecondMatrix(ulong *matrix, CubePolynomial *cubePolynomials, CubePolynomial *publicKey, ulong *constants3) {
    ulong cur;
    ulong index, fac;
    for(uint i = 0; i < size; i++) {
        memset(publicKey[i].vars, 0, MAX_VARS_IN_KEY);
        // for (uint j = 0; j < MAX_VARS_IN_KEY; j++) {
        //     publicKey[i].vars[j] = 0;
        // }
    }
    for (uint i = 0; i < size; i++) {
        cur = 0;
        for (uint j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_SECOND; j += 2) {
        	index = matrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j];
        	fac = matrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j + 1];
            for (uint k = 0; k < MAX_TERMS_IN_POLY; k++) {
                if (cubePolynomials[index].factor[k] == 0) {
                    continue;
                }
                uint p;
                for (p = 0; p < cur; p++) {
                    if (get5Vars(cubePolynomials[index].vars, k) == get5Vars(publicKey[i].vars, p)) {
                        publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(fac, cubePolynomials[index].factor[k]));
                        break;
                    }
                }
                if (p >= cur) {
                    publicKey[i].factor[cur] = modularMult(fac, cubePolynomials[index].factor[k]);
                    if(publicKey[i].factor[cur] == 0) {
                        break;
                    }
                    write5Vars(cubePolynomials[index].vars, publicKey[i].vars, k, cur);
                    cur++;
                }
            }
        }
		
		constants3[i] = 0;
		for (int c = 0; c < cur; c++) {
            if (get5Vars(publicKey[i].vars, c) == 0) {
                constants3[i] = publicKey[i].factor[c];
                publicKey[i].factor[c] = 0;
            }
        }
		
        for (cur; cur < MAX_TERMS_IN_KEY; cur++) {
            publicKey[i].factor[cur] = 0;
        }
        //memset(publicKey[i].factor + cur, 0, (MAX_TERMS_IN_KEY - cur)*8);
    }
}

void addFunctions(ulong *matrix, ulong *constants, CubePolynomial *bufferMatrix, ulong *inverseSecondMatrix) {
	uint pow = 0;
    for (uint i = 0; i < size - 1; i++){
		for (uint j = i + 1; j < size; j++){
			CubePolynomial buffer;
			ulong polynom[size+1];
			// for (uint k = 0; k < size + 1; k++){
			// 	polynom[k] = 0;
			// }
            memset(polynom, 0, 8*(size + 1));
			for (uint k = 0; k < 2 * AMOUNT_OF_VAR_IN_LINE_FIRST;){
				ulong index = matrix[j*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + k];
				ulong number = matrix[j*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + k + 1];
				polynom[index] = number;
				k += 2;
			}
			polynom[size] = constants[j];//inverseSecondMatrix[pow * 2 + 1] % 6
			polynomialDeg(polynom, &buffer, inverseSecondMatrix[pow * 2 + 1] % 6);
            pow++;
            if (pow == size * AMOUNT_OF_VAR_IN_LINE_SECOND) {
                pow = 0;
            }

            printAddFunction(buffer, i, j, inverseSecondMatrix[(pow - 1) * 2 + 1] % 6);

			uint cur = 0;
			while ((buffer.factor[cur]!=0)&&(cur < MAX_TERMS_IN_POLY)) {
				uint k;
				for (k = 0; k < MAX_TERMS_IN_POLY; k++){
					if ((bufferMatrix[i].factor[k]==0)&&(get5Vars(bufferMatrix[i].vars,k)==0)) {
						if (k == MAX_TERMS_IN_POLY - 1) {
							write5Vars(buffer.vars, bufferMatrix[i].vars, cur, k);
							break;
						}
						else {
							if (get5Vars(bufferMatrix[i].vars,k+1)==0) {
								write5Vars(buffer.vars, bufferMatrix[i].vars, cur, k);
								break;
							}
                            if (get5Vars(bufferMatrix[i].vars,k)==get5Vars(buffer.vars,cur)) {
                                break;
                            }
							continue;
						}
					}
					if (get5Vars(bufferMatrix[i].vars,k)==get5Vars(buffer.vars,cur)) {
						break;
					}
				}
				bufferMatrix[i].factor[k] = modularAdd(bufferMatrix[i].factor[k],buffer.factor[cur]);
				cur++;
			}
            
            // printf("bufferMatrix\n");
            // printCubePolynomial(bufferMatrix[i]);
            // printf("\n");
		}
	}

}

uint getFromVar(uint var, uint pos) {
	uint g = (1 << SIZE_OF_AVAR) - 1;
	return (var >> (32 - SIZE_OF_AVAR - SIZE_OF_AVAR * pos)) & g;
}
