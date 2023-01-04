#include <stdio.h>
#include "genlib.h"

//常量定义
#define LowerLimit 0
#define UpperLimit 12

//函数原型
int RiseIntToPower(int num, int power);

int main() {
    printf("  |   2  |   N\n");
    printf("N |   N  |   2\n");
    printf("--+------+------\n");

    int i;
    for (i = LowerLimit; i < UpperLimit; i++) {
        printf("  | %4d | %4d\n", RiseIntToPower(i, 2), RiseIntToPower(2, i));   
    }
}

int RiseIntToPower(int num, int power) {
    int j, res;
    res = 1;
    for (j = 0; j < power; j++) {
        res *= num;
    }
    return res;
}