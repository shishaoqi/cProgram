/* 
* @Author: shishao
* @Date:   2018-08-31 17:31:17
* @Last Modified by:   shishao
* @Last Modified time: 2018-08-31 17:46:08
* -----------------------------------------------------------
* genlib.h bool unit test
*/

#include <stdio.h>

int main() {

#ifdef THINK_C
    printf("Defined THINK_C, executed \"typedef int bool\"\n");
#else
#  ifdef TRUE
#    ifndef bool
#      printf("Defined TRUE, not defined bool, executed \"define bool int\"\n");
#    endif
#  else
#    ifdef bool
#      printf("Not defined TRUE, defined bool, executed \"define FALSE 0\n\"\ndefine TRUE 1");
#    else
        printf("Not defined TRUE, not defined bool, executed \"typedef enum {FALSE, TRUE} bool;\"\n");
#    endif
#  endif
#endif

    return 0;
}