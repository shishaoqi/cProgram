/*
 * File: archway.c
 * ---------------
 * This program is a simple example of the graphics library
 * that draws an archway composed of a 1" square in which the
 * top side has been replaced by a semicircle.
 */

#include <stdio.h>
#include "genlib.h"
#include "graphics.h"

main()
{
    InitGraphics();
    MovePen(2.0, 0.5);
    DrawLine(1.0, 0.0);
    DrawLine(0.0, 1.0);
    DrawArc(0.5, 0, 180);
    DrawLine(0.0, -1.0);
}
