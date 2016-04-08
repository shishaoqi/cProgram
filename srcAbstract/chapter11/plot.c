/*
 * File: plot.c
 * ------------
 * This program can be used to plot the graph of an
 * arbitrary function.
 */

#include <stdio.h>
#include <math.h>
#include "genlib.h"
#include "graphics.h"

/*
 * Constants
 * ---------
 * Pi     -- Mathematical constant pi
 * DeltaX -- Distance between x points
 */

#define Pi     3.14159265
#define DeltaX 0.05

/*
 * Type: doubleFnT
 * ---------------
 * This type defines the class of functions that maps doubles
 * to doubles.
 */

typedef double (*doubleFnT)(double);

/* Function prototypes */

void PlotFunction(doubleFnT fn, double start, double finish);
static void DrawLineTo(double x, double y);

/* Main program */

main()
{
    InitGraphics();
    PlotFunction(sin, 0, 2 * Pi);
}

/*
 * Function: PlotFunction
 * Usage: PlotFunction(fn, start, finish);
 * ---------------------------------------
 * This function plots the indicated function for x values between
 * start and finish, advancing x in units of DeltaX.
 */

void PlotFunction(doubleFnT fn, double start, double finish)
{
    double x;

    MovePen(start, fn(start));
    for (x = start + DeltaX; x <= finish; x += DeltaX) {
        DrawLineTo(x, fn(x));
    }
}


/*
 * Function: DrawLineTo
 * Usage: DrawLineTo(x, y);
 * ------------------------
 * This function is like DrawLine, except that it uses the
 * absolute coordinates of the endpoint rather than the relative
 * displacement from the current point.
 */

static void DrawLineTo(double x, double y)
{
    DrawLine(x - GetCurrentX(), y - GetCurrentY());
}
