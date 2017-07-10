#include "transmitter.h"

uint bitsForVar;

void writeVar_test(uint* var, uint num, uint pos){
    bitsForVar = bitsForVariable();
    uint cur = pos * bitsForVar; // Где мы сейчас находимся (можно передавать как аргумент, если нам потребуется идти не подряд по массиву)
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= bitsForVar){ // Если число влезает, то записываем в одну ячейку
        var[cur / bits] |= num << (bits - cur - bitsForVar);
        cur += bitsForVar;
    } else { //Если число не влезает, то делим его на две части раскидываем в соседние ячейки
        uint shift_help = bitsForVar - (bits - (cur % bits));
        var[cur / bits] |= num >> shift_help;
        var[(cur / bits) + 1] |= num << (bits - shift_help);
        cur += bitsForVar;
    }
}

uint getFromVar_test(uint* var, uint pos) {
    bitsForVar = bitsForVariable();
    uint cur = pos * bitsForVar;
    uint result = 0;
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= bitsForVar){ // Достаём число, если оно лежало в одной ячейке
        result = (var[cur / bits] << (cur % bits)) >> (32 - bitsForVar);
    } else { // Достаём число, если оно было разбито на 2 части
        uint mask = (1 << (bits - (cur % bits))) - 1;
        uint shift_help = bitsForVar - (bits - (cur % bits));
        uint firstPart = (var[cur/bits] & mask) << shift_help;
        uint secondPart = (var[(cur/bits) + 1] >> (bits - shift_help));
        result = firstPart | secondPart;
    }
    cur += bitsForVar;
    return result;
}

ulong get3Vars_test (uint* var, uint pos){
    bitsForVar = bitsForVariable();
    ulong result = 0;
    for(uint i = 0; i < 3; i++){
        result <<= bitsForVar;
        result = getFromVar_test(var, pos + i);
    }
    return result;
}

void print_bin(uint code, uint size_of_code){
    int i;
    for(i= size_of_code - 1;  i >= 0 ; --i){
        printf("%d", (code >> i)&1);
    }
    printf(" ");
}