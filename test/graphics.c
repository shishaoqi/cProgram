#include <stdio.h>
#include <math.h>

#include "genlib.h"

/*
 * Constants: WindowHeight, WindowWidth
 * ------------------------------------
 * These constants are the values returned by GetWindowHeight
 * and GetWindowWidth.  The assumption here is that output is
 * being directed to an 8.5 x 11 page.
 */

#define WindowHeight 11.0
#define WindowWidth   8.5

/*
 * Constant: PSFileName
 * --------------------
 * The name of the PostScript output file.
 */

#define PSFileName "graphics.ps"

/*
 * Private variables
 * -----------------
 * initialized    Set to TRUE when InitGraphics is called.
 * psfile         The file stream used for PostScript.
 * cx, cy         The current x, y positions.
 * nextWrite      The file index for the next PostScript
 *                command (see the general notes above).
 */

static bool initialized = FALSE;

static FILE *psfile;

static double cx, cy;

static long nextWrite;

/* Private function prototypes */

static void InitCheck(void);
static void WritePostScriptHeader(void);
static void WritePostScriptTrailer(void);
static void ResetFilePointer(void);
static double Pts(double inches);
static double Radians(double degrees);

/* Public functions */

void InitGraphics(void)
{
    if (initialized) fclose(psfile);
    psfile = fopen(PSFileName, "w");
    WritePostScriptHeader();
    WritePostScriptTrailer();
    cx = cy = 0;
    initialized = TRUE;
}

void MovePen(double x, double y)
{
    InitCheck();
    cx = x;
    cy = y;
}

void DrawLine(double dx, double dy)
{
    InitCheck();
    ResetFilePointer();
    fprintf(psfile, "newpath %g %g moveto", Pts(cx), Pts(cy));
    fprintf(psfile, " %g %g rlineto stroke\n", Pts(dx), Pts(dy));
    cx += dx;
    cy += dy;
    WritePostScriptTrailer();
}

void DrawArc(double r, double start, double sweep)
{
    double x, y;

    InitCheck();
    x = cx + r * cos(Radians(start + 180));
    y = cy + r * sin(Radians(start + 180));
    ResetFilePointer();
    fprintf(psfile, "newpath %g %g %g", Pts(x), Pts(y), Pts(r));
    fprintf(psfile, " %g %g", start, start + sweep);
    fprintf(psfile, " %s stroke\n", (sweep < 0) ? "arcn" : "arc");
    WritePostScriptTrailer();
    cx = x + r * cos(Radians(start + sweep));
    cy = y + r * sin(Radians(start + sweep));
}

double GetWindowWidth(void)
{
    InitCheck();
    return (WindowWidth);
}

double GetWindowHeight(void)
{
    InitCheck();
    return (WindowHeight);
}

double GetCurrentX(void)
{
    InitCheck();
    return (cx);
}

double GetCurrentY(void)
{
    InitCheck();
    return (cy);
}

/* Private functions */
static void InitCheck(void)
{
    if (!initialized) printf("InitGraphics has not been called");//Error("InitGraphics has not been called");
}

static void WritePostScriptHeader(void)
{
    fprintf(psfile, "%%!PS-Adobe-1.0\n");
    fprintf(psfile, "%%%%Title: graphics window\n");
    fprintf(psfile, "%%%%Pages: 1\n");
    fprintf(psfile, "%%%%EndComments\n");
}

static void WritePostScriptTrailer(void)
{
    nextWrite = ftell(psfile);
    fprintf(psfile, "showpage\n");
}

static void ResetFilePointer(void)
{
    fseek(psfile, nextWrite, 0);
}

static double Pts(double inches)
{
    return (72 * inches);
}

static double Radians(double degrees)
{
    return (degrees / 180 * 3.1415926535);
}

main()
{
    InitGraphics();
    MovePen(2.0, 0.5);
    DrawLine(1.0, 0.0);
    DrawLine(0.0, 1.0);
    DrawArc(0.5, 0, 180);
    DrawLine(0.0, -1.0);
}