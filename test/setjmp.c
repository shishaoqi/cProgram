#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

int main(){
    char *buf;

    int Allocation_handled = 0;
    jmp_buf Allocate_Failed;
    if(setjmp(Allocate_Failed)){
        fprintf(stderr, "couldn't allocate the buffer\n");
        //exit(EXIT_FAILURE);
    }
    buf = (char *)malloc(4096);
    Allocation_handled = 0;

    return 0;
}