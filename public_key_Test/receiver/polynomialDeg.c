#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

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
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = 0; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
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
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                for (x[2] = 0; x[2] < size + 1; x[2]++) {
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
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = 0; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
                    for (x[3] = x[2] + 1; x[3] < size + 1; x[3]++) {
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
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
                    for (x[3] = x[2] + 1; x[3] < size + 1; x[3]++) {
                        for (x[4] = x[3]+1; x[4] < size + 1; x[4]++) {
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
            bufferMatrix->factor[cur] = bufferFactor;
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
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                if (x[0]==x[1]) {
                    continue;
                }
                bufferFactor = modularMult(6,modularMult(modularDeg(polyn[x[0]],2),modularDeg(polyn[x[1]],2)));
                if (bufferFactor == 0) {
                    continue;
                }
                bufferMatrix->factor[cur] = bufferFactor;
                if (x[0]==size) {
                    for (uint j = 0; j < 2; j++) {
                        writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                    }
                    writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                    for (uint j = 3; j < 5; j++) {
                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
                    }
                }
                else
                {
                    if (x[1]==size) {
                        for (uint j = 0; j < 2; j++) {
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
                        }
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                        for (uint j = 3; j < 5; j++) {
                            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                        }
                    }
                    else {
                        for (uint j = 0; j < 2; j++) {
                            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                        }
                        for (uint j = 2; j < 4; j++) {
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + j);
                        }
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
                    }
                }
                cur++;
            }
        }

        //2+1+1 power
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = 0; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
                    if ((x[0]==x[1]) || (x[0]==x[2])) {
                        continue;
                    }
                    bufferFactor = modularMult(12,modularMult(modularDeg(polyn[x[0]],2),modularMult(polyn[x[1]],polyn[x[2]])));
                    if (bufferFactor == 0) {
                        continue;
                    }
                    bufferMatrix->factor[cur] = bufferFactor;
                    if ((x[0]==size) || (x[1]==size)){
                        for (uint j = 0; j < 2; j++) {
                            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                        }
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
                        writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
                    }
                    else
                    {
                        if (x[2]==size){
                            for (uint j = 0; j < 2; j++) {
                                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                            }
                            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                            writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
                        }
                        else
                        {
                            for (uint j = 0; j < 2; j++) {
                                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
                            }
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 2);
                            writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
                            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
                        }
                    }

                    cur++;
                }
            }
        }

        //1+1+1+1 power
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
                    for (x[3] = x[2] + 1; x[3] < size + 1; x[3]++) {
                        if ((x[0] == x[1]) || (x[0] == x[2]) || (x[0] == x[3])) {
                            continue;
                        }
                        bufferFactor = modularMult(24,modularMult(modularMult(polyn[x[0]],polyn[x[1]]),modularMult(polyn[x[2]],polyn[x[3]])));
                        if (bufferFactor == 0) {
                            continue;
                        }
                        bufferMatrix->factor[cur] = bufferFactor;
                        if (x[0]==size) {
                            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
                            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 2);
                            writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
                            writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
                        }
                        else
                        {
                            if (x[1]==size) {
                                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
                                writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
                                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 2);
                                writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 3);
                                writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
                            }
                            else
                            {
                                if (x[2]==size){
                                    writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
                                    writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 1);
                                    writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 2);
                                    writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
                                    writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 4);
                                }
                                else {
                                    if (x[3]==size){
                                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
                                        writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 1);
                                        writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 2);
                                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
                                        writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
                                    }
                                    else
                                    {
                                        writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
                                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
                                        writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 2);
                                        writeVar_test(bufferMatrix->vars, (x[3] + 1)%(size + 1), (5 * cur) + 3);
                                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
                                    }
                                }
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
            bufferMatrix->factor[cur] = bufferFactor;
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
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                for (x[2] = x[1] + 1; x[2] < size + 1; x[2]++) {
                    if ((x[0]==x[1]) || (x[0]==x[2])) {
                        continue;
                    }
                    bufferFactor = modularMult(6,modularMult(polyn[x[0]],modularMult(polyn[x[1]],polyn[x[2]])));
                    if (bufferFactor == 0) {
                        continue;
                    }
                    bufferMatrix->factor[cur] = bufferFactor;
                    if (x[0] == size) {
                        writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
                        writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
                    }
                    else
                    {
                        if (x[1]==size) {
                            writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 0);
                            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 3);
                            writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 4);
                        }
                        else
                        {
                            if (x[2]==size){
                                writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 0);
                                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 3);
                                writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
                            }
                            else
                            {
                                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
                                writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 1);
                                writeVar_test(bufferMatrix->vars, (x[2] + 1)%(size + 1), (5 * cur) + 2);
                                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
                                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 4);
                            }
                        }
                    }
                    cur++;
                }
            }
        }
    }

    if (deg == 2) {
        //2 power
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            bufferFactor = modularDeg(polyn[x[0]], 2);
            if (bufferFactor == 0) {
                continue;
            }
            bufferMatrix->factor[cur] = bufferFactor;;
            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
            writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
            for (uint j = 3; j < 5; j++) {
                writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + j);
            }
            cur++;
        }

        //1+1 power
        for (x[0] = 0; x[0] < size + 1; x[0]++) {
            for (x[1] = x[0] + 1; x[1] < size + 1; x[1]++) {
                if (x[0]==x[1]) {
                    continue;
                }
                bufferFactor = modularMult(2,modularMult(polyn[x[0]],polyn[x[1]]));
                if (bufferFactor == 0) {
                    continue;
                }
                bufferMatrix->factor[cur] = bufferFactor;
                if (x[0]==size){
                    writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 0);
                    writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                    writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                    writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 3);
                    writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 4);
                } else
                {
                    if (x[1]==size) {
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                        writeVar_test(bufferMatrix->vars, (x[1] + 1)%(size + 1), (5 * cur) + 3);
                        writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 4);
                    }
                    else
                    {
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 0);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 1);
                        writeVar_test(bufferMatrix->vars, 0, (5 * cur) + 2);
                        writeVar_test(bufferMatrix->vars, (x[0] + 1) % (size + 1), (5 * cur) + 3);
                        writeVar_test(bufferMatrix->vars, (x[1] + 1) % (size + 1), (5 * cur) + 4);
                    }
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
            for (uint j = 0; j < 4; j++) {
                writeVar_test(bufferMatrix->vars, 0, (5 * cur) + j);
            }
            writeVar_test(bufferMatrix->vars, (x[0] + 1)%(size + 1), (5 * cur) + 4);
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
    }
}