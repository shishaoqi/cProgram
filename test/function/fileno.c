/* 
* @Author: anchen
* @Date:   2016-04-19 14:38:17
* @Last Modified by:   anchen
* @Last Modified time: 2016-04-23 12:48:05
*
* @Description: fileno()用来取得参数stream指定的文件流所使用的文件描述词。
*/

#include <stdio.h>

int main() {
    FILE * fp;
    int fd;
    fp=fopen("/etc/passwd","r");
    fd=fileno(fp);
    printf("fd=%d\n",fd);
    fclose(fp);
    return 0;
}