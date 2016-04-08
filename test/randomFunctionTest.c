/* 
* @Author: anchen
* @Date:   2015-06-23 05:20:39
* @Last Modified by:   anchen
* @Last Modified time: 2015-07-21 10:52:06
*/

#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "random.h"

int main() {
    Randomize();
    printf("RandomInteger-1: %d\n", RandomInteger(1,10));
    printf("RandomInteger-2: %d\n", RandomInteger(1,10));
    printf("RandomReal-1: %f\n", RandomReal(2.0, 5.0));
    printf("RandomReal-2: %f\n", RandomReal(2.0, 5.0));
    printf("RandomChance: %d\n", RandomChance(100));
    return 0;
}