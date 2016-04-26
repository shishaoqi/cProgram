/* 
* @Author: anchen
* @Date:   2016-04-27 00:46:30
* @Last Modified by:   anchen
* @Last Modified time: 2016-04-27 23:04:47
*/

#include <stdio.h>
#include "genlib.h"

static string monthNames[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August", "September",
    "October", "November", "December"
};

int main() {
    int i, n = sizeof monthNames / sizeof (string);
    //string str = GetBlock(20);
    string str;

    for (i = 0; i < n; i++) {
        str = *(monthNames + i);
        printf("%s\n", str);
    }
    return 0;
}