/* 
* @Author: anchen
* @Date:   2016-03-06 11:41:49
* @Last Modified by:   anchen
* @Last Modified time: 2016-03-07 00:18:08
*/

#include <stdio.h>
#include "genlib.h"
#include "graphics.h"

int main() {
    InitGraphics();
    MovePen(2.0, 0.5);
    DrawLine(1.0, 0.0);
    DrawLine(0.0, 1.0);
    DrawArc(0.5, 0, 180);
    DrawLine(0.0, -1.0);
    return 0;
}