/*
* @Author: shishaoqi
* @Date:   2016-06-29 18:07:37
* @Last Modified by:   shishaoqi
* @Last Modified time: 2016-06-30 09:37:02
*/

#include <stdio.h>


#ifdef THINK_C
   typedef int bool;
#else
#  ifdef TRUE
#    ifndef bool
#       define bool int
#    endif
#  else
#    ifdef bool
#       define FALSE 0
#       define TRUE 1
#    else
       typedef enum {FALSE, TRUE} bool;
#    endif
#  endif
#endif

int main() {
    //printf("THINK_C: %d\n", THINK_C);
    printf("TRUE：%d\n", TRUE);
    printf("FALSE：%d\n", FALSE);
    return 0;
}