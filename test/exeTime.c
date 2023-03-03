/*
* @Author: shishao
* @Date:   2017-04-03 23:18:21
* @Last Modified by:   shishao
* @Last Modified time: 2017-04-03 23:20:49
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( ) 
{ 
    long i = 1000000000L; 
    clock_t start, finish; 
    double Total_time; 
    /* 测量一个事件持续的时间*/ 
    printf( "Time to do %ld empty loops is ", i ); 
    start = clock(); 
    while(i--){}; 
    finish = clock(); 
    Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
    printf( "%f seconds\n", Total_time); 
    return 0;
}