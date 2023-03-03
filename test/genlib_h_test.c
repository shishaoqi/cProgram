/* 
* @Author: shishao
* @Date:   2018-08-31 17:31:17
* @Last Modified by:   shishao
* @Last Modified time: 2018-08-31 17:46:08
* -----------------------------------------------------------
* genlib.h bool unit test
*/

#include <stdio.h>
#include <stdbool.h>

int main() {

#ifdef THINK_C
    typedef int bool;
    printf("Defined THINK_C, executed \"typedef int bool\"\n");
#else
#   ifdef TRUE
#       ifndef bool
#           define bool int
            printf("Defined TRUE, not defined bool, executed \"define bool int\"\n");
#       endif
#   else
#       ifdef bool
#           define FALSE 0
#           define TRUE 1
            printf("Not defined TRUE, defined bool, executed: \n\"define FALSE 0\n define TRUE 1\"\n"); // 打开 #include <stdbool.h>，会执行到此分支
#       else
            typedef enum {FALSE, TRUE} bool;
            printf("Not defined TRUE, not defined bool, executed \"typedef enum {FALSE, TRUE} bool;\"\n");
#       endif
#  endif
#endif
    bool t = TRUE;
    bool f = FALSE;
    printf("t is: %d\n", t);
    printf("f is: %d\n", f);
    return 0;
}