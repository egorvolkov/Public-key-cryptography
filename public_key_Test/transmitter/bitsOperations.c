#include "transmitter.h"

void writeVar_test(uint* var, uint num, uint pos){
    uint cur = pos * SIZE_OF_AVAR; // Где мы сейчас находимся (можно передавать как аргумент, если нам потребуется идти не подряд по массиву)
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= SIZE_OF_AVAR){ // Если число влезает, то записываем в одну ячейку
        var[cur / bits] |= num << (bits - cur - SIZE_OF_AVAR);
        cur += SIZE_OF_AVAR;
    } else { //Если число не влезает, то делим его на две части раскидываем в соседние ячейки
        uint shift_help = SIZE_OF_AVAR - (bits - (cur % bits));
        var[cur / bits] |= num >> shift_help;
        var[(cur / bits) + 1] |= num << (bits - shift_help);
        cur += SIZE_OF_AVAR;
    }
}

uint getFromVar_test(uint* var, uint pos) {
    uint cur = pos * SIZE_OF_AVAR;
    uint result = 0;
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= SIZE_OF_AVAR){ // Достаём число, если оно лежало в одной ячейке
        result = (var[cur / bits] << (cur % bits)) >> (32 - SIZE_OF_AVAR);
    } else { // Достаём число, если оно было разбито на 2 части
        uint mask = (1 << (bits - (cur % bits))) - 1;
        uint shift_help = SIZE_OF_AVAR - (bits - (cur % bits));
        uint firstPart = (var[cur/bits] & mask) << shift_help;
        uint secondPart = (var[(cur/bits) + 1] >> (bits - shift_help));
        result = firstPart | secondPart;
    }
    cur += SIZE_OF_AVAR;
    return result;
}

ulong get5Vars(uint *var, uint pos){
    ulong result = 0;
    for(uint i = 0; i < 5; i++){
        result <<= SIZE_OF_AVAR;
        result |= getFromVar_test(var, 5 * pos + i);
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