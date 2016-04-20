/* 
* @Author: shishaoqi
* @Date:   2016-04-19 13:38:55
* @Last Modified by:   anchen
* @Last Modified time: 2016-04-23 12:04:26
* 
* @Description: A program to list the command line arguments
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    int i, j = 0;
    printf("Program name: %s\n", argv[0]);

    //数组遍历
    for(i=1; i<argc; i++){
        printf("Argument %d: %s\n", i, argv[i]);
    }

    //pointer to traverse
    printf("\nPointer to traverse:\n");
    while(--argc){
        printf("Argument %d: %s\n", ++j, * ++argv);
    }

    return 0;
}