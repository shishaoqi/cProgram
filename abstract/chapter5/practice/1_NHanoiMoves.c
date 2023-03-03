#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

// 1. 按照 MoveTower 函数的逻辑，编写一个递归函数 NHanoiMoves(n) 来计算解决 n 只圆盘的汉诺塔问题所需移动的次数

int number = 0;
void MoveTower(int n, char s, char f, char t);

int main() {
    int count;
    count = GetInteger();
    char start = 'A', finish = 'B', tmp = 'C';
    MoveTower(count, start, finish, tmp);

    printf("single move count is: %d\n", number);
    return 0;
}

void MoveTower(int n, char start, char finish, char tmp) {
    if (n == 1) {
        printf("%c --> %c\n", start, finish);
        number++;
    } else {
        MoveTower(n-1, start, tmp, finish);
        printf("%c --> %c\n", start, finish);
        number++;
        MoveTower(n-1, tmp, finish, start);
    }
}