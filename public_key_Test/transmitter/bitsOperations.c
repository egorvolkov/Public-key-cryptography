#include "transmitter.h"

uint bitsForVar;

void writeVar_test(uint* var, uint num, uint pos){
    bitsForVar = bitsForVariable();
    uint cur = pos * bitsForVar; // ��� �� ������ ��������� (����� ���������� ��� ��������, ���� ��� ����������� ���� �� ������ �� �������)
    uint bits = sizeof(var[0]) * 8;
    if(bits - (cur % bits) >= bitsForVar){ // ���� ����� �������, �� ���������� � ���� ������
        var[cur / bits] |= num << (bits - cur - bitsForVar);
        cur += bitsForVar;
    } else { //���� ����� �� �������, �� ����� ��� �� ��� ����� ����������� � �������� ������
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
    if(bits - (cur % bits) >= bitsForVar){ // ������ �����, ���� ��� ������ � ����� ������
        result = (var[cur / bits] << (cur % bits)) >> (32 - bitsForVar);
    } else { // ������ �����, ���� ��� ���� ������� �� 2 �����
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