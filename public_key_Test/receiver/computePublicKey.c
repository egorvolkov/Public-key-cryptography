#include "receiver.h"

extern const uint size;
extern struct Module moduleStruct;

#ifdef PRINT
extern FILE *output;
#endif

void computePublicKey(ulong *firstMatrix, ulong *secondMatrix, FullCubePolynomial *publicKey, ulong *constants, ulong *constants3) {
	CubePolynomial bufferMatrix[size];
	cubeOfPolynomials(firstMatrix, bufferMatrix, constants);
#ifdef PRINT
	printf("Constants:\n");
	for(uint i = 0; i < size; i++){
		printf("%u ", constants[i]);
	}
	printf("\n\n");
	fprintf(output, "Cube polynomials\n"); printf("Cube polynomials\n");
	fPrintCubePolynomials(bufferMatrix); printCubePolynomials(bufferMatrix);
	fprintf(output, "\n"); printf("\n");
#endif

	multToSecondMatrix(secondMatrix, bufferMatrix, publicKey, constants3);
}


void computeNewPublicKey(ulong *firstMatrix, ulong *secondMatrix, ulong *funcMatrix, FullCubePolynomial *newPublicKey, ulong *constants, ulong *constants3){
	CubePolynomial newBufferMatrix[size];
	cubeOfNewPolynomials(firstMatrix, newBufferMatrix, constants);
#ifdef PRINT
	printf("Constants:\n");
	for(uint i = 0; i < size; i++){
		printf("%u ", constants[i]);
	}
	printf("\n\n");
	fprintf(output, "New cube polynomials\n"); printf("New cube polynomials\n");
	fPrintCubePolynomials(newBufferMatrix); printCubePolynomials(newBufferMatrix);
	fprintf(output, "\n"); printf("\n");
#endif
   // addFunctions(firstMatrix, constants, newBufferMatrix, funcMatrix);
	newMultToSecondMatrix(secondMatrix, newBufferMatrix, newPublicKey, constants3);
}

void cubeOfNewPolynomials(ulong *matrix, CubePolynomial newBufferMatrix[], ulong *constants) {
	CubePolynomial *buf;
	ulong polinom[size + 1];
	for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size + 1; j++){
            polinom[j] = 0;
        }
		for (uint j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_FIRST;){
			ulong index = matrix[i*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j];
			ulong number = matrix[i*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + j + 1];
			polinom[index] = number;
			j += 2;
		}
        polinom[size] = constants[i];
		buf = &(newBufferMatrix[i]);
		polynomialDeg(polinom,buf,3);
	}
}

void newMultToSecondMatrix(ulong *matrix, CubePolynomial *newBufferMatrix, FullCubePolynomial *publicKey, ulong *constants3) {
    ulong cur;
    ulong index, fac;
    for(uint i = 0; i < size; i++) {
        for (uint j = 0; j < MAX_VARS_IN_KEY; j++) {
            publicKey[i].vars[j] = 0;
        }
    }
    for (uint i = 0; i < size; i++) {
        cur = 0;
        for (uint j = 0; j < 2 * AMOUNT_OF_VAR_IN_LINE_SECOND; j += 2) {
        	index = matrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j];
        	fac = matrix[i * 2 * AMOUNT_OF_VAR_IN_LINE_SECOND + j + 1];
            for (uint k = 0; k < MAX_TERMS_IN_POLY; k++) {
                if (newBufferMatrix[index].factor[k] == 0) {
                    break;
                }
                uint p;
                for (p = 0; p < cur; p++) {
                    if (get5Vars(newBufferMatrix[index].vars, k) == get5Vars(publicKey[i].vars, p)) {
                        publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(fac, newBufferMatrix[index].factor[k]));
                        break;
                    }
                }
                if (p >= cur) {
                    publicKey[i].factor[cur] = modularMult(fac, newBufferMatrix[index].factor[k]);
                    if(publicKey[i].factor[cur] == 0) {
                        break;
                    }
                    write5Vars(newBufferMatrix[index].vars, publicKey[i].vars, k, cur);
                    //print_bin(get5Vars(bufferMatrix[j].vars, k), 9);
                    //printf("(%llu); ", bufferMatrix[j].factor[k]);
                    //print_bin(get5Vars(publicKey[i].vars, cur), 9);
                    //printf("(%llu);\n", publicKey[i].factor[cur]);
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
    }
}

void cubeOfPolynomials(ulong *matrix, CubePolynomial bufferMatrix[], ulong *constants) {
	CubePolynomial *buf;
	ulong polinom[size + 1];
	for (uint i = 0; i < size; i++) {
		for (uint j = 0; j < size; j++){
			polinom[j] = matrix[j + i*size];
		}
        polinom[size] = constants[i];
		buf = &(bufferMatrix[i]);
		polynomialDeg(polinom, buf, 3);
	}
}

void polynomialCube(ulong *polyn, CubePolynomial *bufferMatrix) {
	uint cur = 0;
	ulong bufferFactor;

	for (uint i = 0; i < MAX_VARS_IN_POLY; i++) {
		bufferMatrix->vars[i] = 0;
	}

	// Calculating 3rd power
	for (uint i = 0; i < size; i++) {
		bufferFactor = modularDeg(polyn[i], 3);
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		//bufferMatrix->vars[cur] = 0;
		for (uint j = 0; j < 3; j++) {
			writeVar_test(bufferMatrix->vars, i + 1, (5 * cur) + j);
		}
		cur++;
	}

	// Calculating 2nd+1st power
	for (uint i = 0; i < size; i++) {
		for (uint j = 0; j < size; j++) {
			if (i == j)
				continue;
			bufferFactor = modularMult(3, modularMult(modularDeg(polyn[i], 2), polyn[j]));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			//bufferMatrix->vars[cur] = 0;
			for (uint k = 0; k < 2; k++) {
				writeVar_test(bufferMatrix->vars, i + 1, (5 * cur) + k);
			}
			writeVar_test(bufferMatrix->vars, j + 1, (5 * cur) + 2);
			cur++;
		}
	}


	//Calculating 1st+1st+1st power
	for (uint i = 0; i < size; i++) {
		for (uint j = i + 1; j < size; j++) {
			for (uint k = j + 1; k < size; k++) {
				bufferFactor = modularMult(6, modularMult(modularMult(polyn[i], polyn[j]), polyn[k]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				//bufferMatrix->vars[cur] = 0;
				writeVar_test(bufferMatrix->vars, i + 1, (5 * cur) + 0);
				writeVar_test(bufferMatrix->vars, j + 1, (5 * cur) + 1);
				writeVar_test(bufferMatrix->vars, k + 1, (5 * cur) + 2);
				cur++;
			}
		}
	}


	//Calculating 1st+1st power
	for (uint i = 0; i < size; i++) {
		for(uint j = i; j < size; j++) {
			bufferFactor = modularMult(i==j?3:6, modularMult(polyn[size], modularMult(polyn[i], polyn[j])));
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			//bufferMatrix->vars[cur] = 0;
			writeVar_test(bufferMatrix->vars, i + 1, (5 * cur) + 0);
			writeVar_test(bufferMatrix->vars, j + 1, (5 * cur) + 1);
			cur++;
		}
	}


	//Calculating 1st power
	for(uint i = 0; i < size; i++) {
		bufferFactor = modularMult(3, modularMult(modularDeg(polyn[size], 2), polyn[i]));
		if (bufferFactor == 0) {
			continue;
		}
		bufferMatrix->factor[cur] = bufferFactor;
		//bufferMatrix->vars[cur] = 0;
		writeVar_test(bufferMatrix->vars, i + 1, (5 * cur) + 0);
		cur++;
	}

	//Calculating constant in cube
	bufferFactor = modularDeg(polyn[size],3);
	if (bufferFactor != 0) {
		bufferMatrix->factor[cur] = bufferFactor;
		for (int i = 0; i < 3; i++) {
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + i);
		}
		cur++;
	}

	for (cur; cur < MAX_TERMS_IN_POLY; cur++) {
		bufferMatrix->factor[cur] = 0;
		//bufferMatrix->vars[cur] = 0;
	}
}

void polynomialDeg(ulong *polyn, CubePolynomial *bufferMatrix, ulong deg) {
	uint cur = 0;
	ulong bufferFactor;
	uint x[5];

	for (uint i = 0; i < MAX_VARS_IN_POLY; i++) {
		bufferMatrix->vars[i] = 0;
	}

	if (deg == 5) {
		// 5 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			bufferFactor = modularDeg(polyn[x[0]], 5);
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;
			for (uint j = 0; j < 5; j++) {
				writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
			}
			cur++;
		}

		// 4+1 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(5,modularMult(modularDeg(polyn[x[0]],4),polyn[x[1]]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				for (uint j = 0; j < 4; j++) {
					writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
				}
				writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
				cur++;
			}
		}

		//3+2 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(10,modularMult(modularDeg(polyn[x[0]],3),modularDeg(polyn[x[1]],2)));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				for (uint j = 0; j < 3; j++) {
					writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
				}
				for (uint j = 3; j < 5; j++) {
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
				}
				cur++;
			}
		}

		//3+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = 0; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					if ((x[0]==x[1]) || (x[0]==x[2])) {
						continue;
					}
					bufferFactor = modularMult(20,modularMult(modularDeg(polyn[x[0]],3),modularMult(polyn[x[1]],polyn[x[2]])));
					if (bufferFactor == 0) {
						continue;
					}
					bufferMatrix->factor[cur] = bufferFactor;
					for (uint j = 0; j < 3; j++) {
						writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
					}
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
					cur++;
				}
			}
		}

		//2+2+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = x[1] + 1; x[1] < size; x[1]++) {
				for (x[2] = 0; x[2] < size; x[2]++) {
					if ((x[0] == x[2]) || (x[1]==x[2])) {
						continue;
					}
					bufferFactor = modularMult(30,modularMult(modularMult(modularDeg(polyn[x[0]],2),modularDeg(polyn[x[1]],2)),polyn[x[2]]));
					if (bufferFactor == 0) {
						continue;
					}
					bufferMatrix->factor[cur] = bufferFactor;
					for (uint j = 0; j < 2; j++) {
						writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
					}
					for (uint j = 2; j < 4; j++) {
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
					}
					writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
					cur++;
				}
			}
		}

		//2+1+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = 0; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					for (x[3] = x[2] + 1; x[3] < size; x[3]++) {
						if ((x[0] == x[1]) || (x[0] == x[2]) || (x[0] == x[3])) {
							continue;
						}
						bufferFactor = modularMult(60,modularMult(modularMult(modularDeg(polyn[x[0]],2),polyn[x[1]]),modularMult(polyn[x[2]],polyn[x[3]])));
						if (bufferFactor == 0) {
							continue;
						}
						bufferMatrix->factor[cur] = bufferFactor;
						for (uint j = 0; j < 2; j++) {
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
						}
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
						cur++;
					}
				}
			}
		}

		//1+1+1+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = x[0] + 1; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					for (x[3] = x[2] + 1; x[3] < size; x[3]++) {
						for (x[4] = x[3]+1; x[4] < size; x[4]++) {
							bufferFactor = modularMult(120,modularMult(modularMult(polyn[x[0]],polyn[x[1]]),modularMult(modularMult(polyn[x[2]],polyn[x[3]]),polyn[x[4]])));
							if (bufferFactor == 0) {
								continue;
							}
							bufferMatrix->factor[cur] = bufferFactor;
							for (uint j = 0; j < 5; j++) {
								writeVar_test(bufferMatrix->vars, (x[j] + 1)%(size + 1), (5 * cur) + j);
							}
							cur++;
						}
					}
				}
			}
		}
	}

	if (deg == 4) {
		//4 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			bufferFactor = modularDeg(polyn[x[0]], 4);
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;;
			for (uint j = 0; j < 4; j++) {
				writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
			}
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
			cur++;
		}

		//3+1 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(4,modularMult(modularDeg(polyn[x[0]],3),polyn[x[1]]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				for (uint j = 0; j < 3; j++) {
					writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
				}
				if (x[0]==size){
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
				} else
				{
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
				}
				cur++;
			}
		}

		//2+2 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(6,modularMult(modularDeg(polyn[x[0]],2),modularDeg(polyn[x[1]],2)));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				for (uint j = 0; j < 2; j++) {
					writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
				}
				if (x[0]==size){
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
					for (uint j = 3; j < 5; j++) {
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
					}
				}
				else
				{
					for (uint j = 2; j < 4; j++) {
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
					}
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
				}
				cur++;
			}
		}

		//2+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = 0; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					if ((x[0]==x[1]) || (x[0]==x[2])) {
						continue;
					}
					bufferFactor = modularMult(12,modularMult(modularDeg(polyn[x[0]],2),modularMult(polyn[x[1]],polyn[x[2]])));
					if (bufferFactor == 0) {
						continue;
					}
					bufferMatrix->factor[cur] = bufferFactor;
					if ((x[0] == size) || (x[1] == size)) {
						for (uint j = 0; j < 2; j++) {
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
						}
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
					}else
					{
						for (uint j = 0; j < 2; j++) {
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
						}
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
					}

					cur++;
				}
			}
		}

		//1+1+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = 0; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					for (x[3] = x[2] + 1; x[3] < size; x[3]++) {
						if ((x[0] == x[1]) || (x[0] == x[2]) || (x[0] == x[3])) {
							continue;
						}
						bufferFactor = modularMult(24,modularMult(modularMult(polyn[x[0]],polyn[x[1]]),modularMult(polyn[x[2]],polyn[x[3]])));
						if (bufferFactor == 0) {
							continue;
						}
						bufferMatrix->factor[cur] = bufferFactor;
						if ((x[0]==size) || (x[1] == size)){
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
							writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
							writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 2);
							writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
							writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
						}
						else
						{
							if ((x[2]==size) || (x[3]==size)) {
								writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
								writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
								writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 2);
								writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
								writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
							}
							else
							{
								writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
								writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
								writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 2);
								writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 3);
								writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 5);
							}
						}
						cur++;
					}
				}
			}
		}
	}

	if (deg == 3) {
		//3 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			bufferFactor = modularDeg(polyn[x[0]], 3);
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;;
			for (uint j = 0; j < 3; j++) {
				writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
			}
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
			cur++;
		}

		//2+1 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(3,modularMult(modularDeg(polyn[x[0]],2),polyn[x[1]]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				if (x[0]==size){
					for (uint j = 0; j < 2; j++) {
						writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
					}
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
				} else
				{
					if (x[1]==size){
						writeVar_test(bufferMatrix->vars, (x[1] + 1) % (size + 1), (5 * cur) + 0);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
						for (uint j = 3; j < 5; j++) {
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
						}
					}else {
						for (uint j = 0; j < 2; j++) {
							writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
						}
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, (x[1] + 1) % (size + 1), (5 * cur) + 4);
					}
				}
				cur++;
			}
		}

		//1+1+1 power
		for (x[0] = 0; x[0] < size; x[0]++) {
			for (x[1] = 0; x[1] < size; x[1]++) {
				for (x[2] = x[1] + 1; x[2] < size; x[2]++) {
					if ((x[0]==x[1]) || (x[0]==x[2])) {
						continue;
					}
					bufferFactor = modularMult(6,modularMult(polyn[x[0]],modularMult(polyn[x[1]],polyn[x[2]])));
					if (bufferFactor == 0) {
						continue;
					}
					bufferMatrix->factor[cur] = bufferFactor;
					if ((x[0] == size) || (x[1] == size)) {
						writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
					}else
					{
						writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
						writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
						writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 2);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
						writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
					}

					cur++;
				}
			}
		}
	}

	if (deg == 2) {
		//2 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			bufferFactor = modularDeg(polyn[x[0]], 3);
			if (bufferFactor == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = bufferFactor;;
			for (uint j = 0; j < 2; j++) {
				writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
			}
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
			cur++;
		}

		//1+1 power
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			for (x[1] = 0; x[1] < size + 1; x[1]++) {
				if (x[0]==x[1]) {
					continue;
				}
				bufferFactor = modularMult(5,modularMult(polyn[x[0]],polyn[x[1]]));
				if (bufferFactor == 0) {
					continue;
				}
				bufferMatrix->factor[cur] = bufferFactor;
				writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
				if (x[0]==size){
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
				} else
				{
					writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
					writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
				}
				cur++;
			}
		}
	}

	if (deg == 1) {
		for (x[0] = 0; x[0] < size + 1; x[0]++) {
			if (polyn[x[0]] == 0) {
				continue;
			}
			bufferMatrix->factor[cur] = polyn[x[0]];
			writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
			for (uint j = 1; j < 5; j++) {
				writeVar_test(bufferMatrix->vars, 0, (5 * cur) + j);
			}
			cur++;
		}
	}

	if (deg == 0) {
		bufferMatrix->factor[cur] = 1;
		for (uint j = 0; j < 5; j++) {
			writeVar_test(bufferMatrix->vars, 0, (5 * cur) + j);
		}
		cur++;
	}

	for (cur; cur < MAX_TERMS_IN_POLY; cur++) {
		bufferMatrix->factor[cur] = 0;
		bufferMatrix->vars[cur] = 0;
	}
}

void addFunctions(ulong *matrix, ulong *constants, CubePolynomial *bufferMatrix, ulong *funcMatrix) {
	for (uint i = 0; i < size - 1; i++){
		for (uint j = i + 1; j < size; j++){
			CubePolynomial buffer;
			ulong polynom[size+1];
			for (uint k = 0; k < size + 1; k++){
				polynom[k] = 0;
			}
			for (uint k = 0; k < 2 * AMOUNT_OF_VAR_IN_LINE_FIRST;){
				ulong index = matrix[j*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + k];
				ulong number = matrix[j*2 * AMOUNT_OF_VAR_IN_LINE_FIRST + k + 1];
				polynom[index] = number;
				k += 2;
			}
			polynom[size] = constants[j];
			polynomialDeg(polynom, &buffer, funcMatrix[j + i * size]%6);
			uint cur = 0;
			while (buffer.factor[cur]!=0) {
				uint k;
				for (k = 0; k < MAX_TERMS_IN_POLY; k++){
					if (bufferMatrix[i].factor[k]==0) {
						write5Vars(buffer.vars, bufferMatrix[i].vars, cur, k);
						break;
					}
					if (get5Vars(bufferMatrix[i].vars,k)==get5Vars(buffer.vars,cur)) {
						break;
					}
				}
				bufferMatrix[i].factor[k] = modularAdd(bufferMatrix[i].factor[k],buffer.factor[cur]);
				cur++;
			}

		}
	}

}

void multToSecondMatrix(ulong *matrix, CubePolynomial *bufferMatrix, FullCubePolynomial *publicKey, ulong *constants3) {
	ulong cur;
	for(uint i = 0; i < size; i++) {
		for (uint j = 0; j < MAX_VARS_IN_KEY; j++) {
			publicKey[i].vars[j] = 0;
		}
	}
	for (uint i = 0; i < size; i++) {
		cur = 0;
		for (uint j = 0; j < size; j++) {
			for (uint k = 0; k < MAX_TERMS_IN_POLY; k++) {
				if (bufferMatrix[j].factor[k] == 0) {
					break;
				}
				uint p;
				for (p = 0; p < cur; p++) {
					if (get5Vars(bufferMatrix[j].vars, k) == get5Vars(publicKey[i].vars, p)) {
						publicKey[i].factor[p] = modularAdd(publicKey[i].factor[p], modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]));
						break;
					}
				}
				if (p >= cur) {
					publicKey[i].factor[cur] = modularMult(matrix[i*size + j], bufferMatrix[j].factor[k]);
					if(publicKey[i].factor[cur] == 0) {
						break;
					}
                    write5Vars(bufferMatrix[j].vars, publicKey[i].vars, k, cur);
					//print_bin(get5Vars(bufferMatrix[j].vars, k), 9);
					//printf("(%llu); ", bufferMatrix[j].factor[k]);
					//print_bin(get5Vars(publicKey[i].vars, cur), 9);
					//printf("(%llu);\n", publicKey[i].factor[cur]);
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
	}
}

void writeVar(uint *var, uint number, uint pos) {
	for (uint i = 0; i < SIZE_OF_AVAR; i++) {
		*var &= (~(1 << (31 - i - SIZE_OF_AVAR * pos)));
	}
	*var |= number << (32 - SIZE_OF_AVAR * pos - SIZE_OF_AVAR);
}

uint getFromVar(uint var, uint pos) {
	uint g = (1 << SIZE_OF_AVAR) - 1;
	return (var >> (32 - SIZE_OF_AVAR - SIZE_OF_AVAR * pos)) & g;
}
