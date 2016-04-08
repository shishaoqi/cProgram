/* 
* @Author: anchen
* @Date:   2015-06-23 05:58:23
* @Last Modified by:   anchen
* @Last Modified time: 2015-07-20 23:15:20
*/

#include <stdio.h>
#include <stdlib.h>
double RandomReal(double low, double high);

int main() {
    srand((int) time(NULL));
    printf("RandomReal: %lf\n", RandomReal(1,100));
    return 0;
}

double RandomReal(double low, double high)
{
    double d;

    d = (double) rand() / ((double) RAND_MAX + 1);
    return (low + d * (high - low));
}