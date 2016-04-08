/*
 * File: koch.c
 * ---------------
 * This program draws a Koch fractal.
 */

#include <stdio.h>
#include <math.h>
#include "genlib.h"
#include "simpio.h"
#include "graphics.h"

/* Mathematical constants */

#define Pi 3.1415926535

/* Private function prototypes */

static void KochFractal(double size, int order);
static void DrawFractalLine(double len, double theta, int order);
static void DrawPolarLine(double r, double theta);

/* Main program */

main()
{
    double size;
    int order;

    InitGraphics();
    printf("Program to draw Koch fractals\n");
    printf("Enter edge length in inches: ");
    size = GetReal();
    printf("Enter order of fractal: ");
    order = GetInteger();
    KochFractal(size, order);
}

/*
 * Function: KochFractal
 * Usage: KochFractal(size, order);
 * --------------------------------
 * This function draws a Koch fractal snowflake centered in
 * the graphics window of the indicated size and order.
 */

static void KochFractal(double size, int order)
{
    double x0, y0;

    x0 = GetWindowWidth() / 2 - size / 2;
    y0 = GetWindowHeight() / 2 - sqrt(3) * size / 6;
    MovePen(x0, y0);
    DrawFractalLine(size, 0, order);
    DrawFractalLine(size, 120, order);
    DrawFractalLine(size, 240, order);
}

/*
 * Function: DrawFractalLine
 * Usage: DrawFractalLine(len, theta, order);
 * ------------------------------------------
 * This function draws a fractal line of the given length, starting
 * from the current point and moving in direction theta.  If order
 * is 0, the fractal line is just a straight line.  If order is
 * greater than zero, the line is divided into four line segments,
 * each of which is a fractal line of the next lower order.  The
 * four segments connect the same endpoints as the straight line,
 * but include a triangular wedge replacing the center third of
 * the segment.
 */

static void DrawFractalLine(double len, double theta, int order)
{
    if (order == 0) {
        DrawPolarLine(len, theta);
    } else {
        DrawFractalLine(len/3, theta, order - 1);
        DrawFractalLine(len/3, theta - 60, order - 1);
        DrawFractalLine(len/3, theta + 60, order - 1);
        DrawFractalLine(len/3, theta, order - 1);
    }
}

/*
 * Function: DrawPolarLine
 * Usage: DrawPolarLine(r, theta);
 * -------------------------------
 * This function draws a line of length r in the direction
 * specified by the angle theta.  As in the DrawArc function,
 * theta is measured in degrees counterclockwise from the +x
 * axis.  This style of measurement is called "polar coordinates."
 */

static void DrawPolarLine(double r, double theta)
{
    double radians;

    radians = theta / 180 * Pi;
    DrawLine(r * cos(radians), r * sin(radians));
}
