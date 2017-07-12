#include "transmitter.h"

void writeVar_test(uint* var, uint num, uint pos){
    uint cur = pos * SIZE_OF_AVAR; // ��� �� ������ ��������� (����� ���������� ��� ��������, ���� ��� ����������� ���� �� ������ �� �������)
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= SIZE_OF_AVAR){ // ���� ����� �������, �� ���������� � ���� ������
        var[cur / bits] |= num << (bits - cur - SIZE_OF_AVAR);
        cur += SIZE_OF_AVAR;
    } else { //���� ����� �� �������, �� ����� ��� �� ��� ����� ����������� � �������� ������
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
    if(bits - (cur % bits) >= SIZE_OF_AVAR){ // ������ �����, ���� ��� ������ � ����� ������
        result = (var[cur / bits] << (cur % bits)) >> (32 - SIZE_OF_AVAR);
    } else { // ������ �����, ���� ��� ���� ������� �� 2 �����
        uint mask = (1 << (bits - (cur % bits))) - 1;
        uint shift_help = SIZE_OF_AVAR - (bits - (cur % bits));
        uint firstPart = (var[cur/bits] & mask) << shift_help;
        uint secondPart = (var[(cur/bits) + 1] >> (bits - shift_help));
        result = firstPart | secondPart;
    }
    cur += SIZE_OF_AVAR;
    return result;
}

ulong get3Vars_test (uint* var, uint pos){
    ulong result = 0;
    for(uint i = 0; i < 3; i++){
        result <<= SIZE_OF_AVAR;
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