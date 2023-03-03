#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

void ListSubsets(string s);
void RecursiveSubsets(string s, int i);

int main() {
    string s;

    // s = GetLine();
    // printf("%s\n", s);

    ListSubsets("ABCD");

    return 0;
}

void ListSubsets(string s) {
    int i;
    int l = StringLength(s);
    for (i = 0; i <= l; i++) {
        //printf("%s\n", SubString(s, i, l-1));
        RecursiveSubsets(SubString(s, i, l-1), 0);   
    }
}

void RecursiveSubsets(string s, int i) {
    int j = 0;
    int l = StringLength(s);
    
    // if (i == l) {
    //     if (l == 0) printf("{}\n");
    //     if (l == 1) printf("{%s}\n", s);
    // } else {
    //     printf("{%s}\n", s);
    //     RecursiveSubsets(s, i+1);
    // }

    if (i >= l) {
        printf("1{%s}\n", s);
    } else {
        int n = StringLength(s) - 1;
        for(j = 0; j < i; j++) {
            printf("i=%d, 2{%s}\n", i, s);
        }
        RecursiveSubsets(s, i+1);
    }
}