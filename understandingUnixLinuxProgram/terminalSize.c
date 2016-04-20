/*
* @Author: anchen
* @Date:   2016-04-19 17:57:31
* @Last Modified by:   anchen
* @Last Modified time: 2016-04-23 16:10:07
*
* @Description: linux终端窗口大小的获得
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main() {
    struct winsize size;

    if (isatty(STDOUT_FILENO) == 0) exit(1);

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl TIOCGWINSZ error");
        exit(1);   
    }
    printf("Terminal size is: %d rows, %d columns\n", size.ws_row, size.ws_col);

    return 0;
}