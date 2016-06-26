/*
* @Author: shishao
* @Date:   2016-06-25 11:19:18
* @Last Modified by:   shishao
* @Last Modified time: 2016-06-25 11:59:15
* exec!.c - shows how easy it is for a program to run a program
*/

#include <stdio.h>

int main() {
    char *arglist[3];

    arglist[0] = "ls";
    arglist[1] = "-l";
    arglist[2] = 0;
    printf("*** About to exec ls -l\n");
    execvp("ls", arglist);
    printf("*** ls is done. bye\n");
    return 0;
}