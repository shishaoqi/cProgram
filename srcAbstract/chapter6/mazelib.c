/*
 * File: mazelib.c
 * ---------------
 * This file implements the mazelib.h interface.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "extgraph.h"
#include "mazelib.h"

/*
 * Constants
 * ---------
 * LeftMargin    -- Margin between maze and left screen edge
 * BottomMargin  -- Margin at the bottom of the screen
 * SquareSize    -- Size of each square in inches
 * MarkSize      -- Size of the mark symbol
 * StartRadius   -- Radius of the start circle
 * PauseTime     -- Delay at each display point
 */

#define LeftMargin    0.15
#define BottomMargin  0.15
#define SquareSize    (20 / 72.0)
#define MarkSize      (4 / 72.0)
#define StartRadius   (5 / 72.0)
#define PauseTime     0.1

/*
 * Type: squareT
 * -------------
 * This type holds the state information required for each corner
 * in the maze.  The walls array is indexed with a directionT.
 */

typedef struct {
    bool marked;
    bool walls[4];
} squareT;

/*
 * Private data
 * ------------
 * maze         -- Dynamic 2-D array that stores the maze
 * mazeWidth    -- The width of the stored maze
 * mazeHeight   -- The height of the stored maze
 * startSquare  -- The pointT corresponding to the start square
 * initialized  -- Flag showing package has been initialized
 */

static squareT **maze;
static int mazeWidth, mazeHeight;
static pointT startSquare;
static bool initialized = FALSE;

/* Private function prototypes */

static void ComputeMazeSize(FILE *infile);
static void InitMazeArray(void);
static void ProcessMazeFile(FILE *infile);
static void ProcessDividerLine(string line, int y);
static void ProcessPassageLine(string line, int y);
static void SetHorizontalWall(pointT pt);
static void SetVerticalWall(pointT pt);
static void SetStartSquare(pointT pt);
static void DrawCenteredCircle(double x, double y, double r);
static void DrawMarkInSquare(pointT pt);
static bool InRange(int x, int y);
static void TrimTrailingSpace(char buffer[]);
static void FreeOldStorage(void);

/* Public entries */

void ReadMazeMap(string filename)
{
    FILE *infile;

    InitGraphics();
    infile = fopen(filename, "r");
    if (infile == NULL) Error("Can't open %s", filename);
    ComputeMazeSize(infile);
    fclose(infile);
    InitMazeArray();
    startSquare.x = -1;
    infile = fopen(filename, "r");
    if (infile == NULL) Error("Can't reopen maze file");
    ProcessMazeFile(infile);
    if (startSquare.x == -1) Error("Maze contains no start square");
    initialized = TRUE;
}

pointT GetStartPosition(void)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    return (startSquare);
}

bool OutsideMaze(pointT pt)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    return (!InRange(pt.x, pt.y));
}

bool WallExists(pointT pt, directionT dir)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    if (OutsideMaze(pt)) Error("Coordinates are out of range");
    return (maze[pt.x][pt.y].walls[dir]);
}

void MarkSquare(pointT pt)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    if (OutsideMaze(pt)) Error("Coordinates are out of range");
    maze[pt.x][pt.y].marked = TRUE;
    DrawMarkInSquare(pt);
}

void UnmarkSquare(pointT pt)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    if (OutsideMaze(pt)) Error("Coordinates are out of range");
    maze[pt.x][pt.y].marked = FALSE;
    SetPenColor("White");
    DrawMarkInSquare(pt);
    SetPenColor("Black");
}

bool IsMarked(pointT pt)
{
    if (!initialized) Error("ReadMazeMap has not been called");
    if (OutsideMaze(pt)) Error("Coordinates are out of range");
    return (maze[pt.x][pt.y].marked);
}

/* Private functions */

/*
 * Function: ComputeMazeSize
 * Usage: ComputeMazeSize(infile);
 * -------------------------------
 * This function reads the data file and computes the dimensions
 * of the maze.  The file must then be read a second time to read
 * the actual data.  The values indicating the size of the maze
 * are stored in the private variables mazeWidth and mazeHeight.
 */

static void ComputeMazeSize(FILE *infile)
{
    string line;
    int nLines, len;
    bool trailing;

    nLines = 0;
    trailing = FALSE;
    while ((line = ReadLine(infile)) != NULL) {
        TrimTrailingSpace(line);
        len = StringLength(line);
        if (len == 0) {
            trailing = TRUE;
        } else if (trailing) {
            Error("Illegal blank lines in data file");
        } else if (nLines == 0) {
            if (len % 2 != 1) Error("Illegal maze width");
            mazeWidth = (len - 1) / 2;
            nLines++;
        } else {
            nLines++;
        }
    }
    if (nLines % 2 != 1) Error("Illegal maze height");
    mazeHeight = (nLines - 1) / 2;
}

/*
 * Function: InitMazeArray
 * Usage: InitMazeArray();
 * -----------------------
 * This file initializes the maze array and must be called after
 * the dimensions of the maze have been set by ComputeMazeSize.
 */

static void InitMazeArray(void)
{
    int x, y;
    directionT dir;

    if (initialized) FreeOldStorage();
    maze = NewArray(mazeWidth, squareT *);
    for (x = 0; x < mazeWidth; x++) {
        maze[x] = NewArray(mazeHeight, squareT);
        for (y = 0; y < mazeHeight; y++) {
            maze[x][y].marked = FALSE;
            for (dir = North; dir <= West; dir++) {
                maze[x][y].walls[dir] = FALSE;
            }
        }
    }
}

/*
 * Function: ProcessMazeFile
 * Usage: ProcessMazeFile(infile);
 * -------------------------------
 * This function reads the actual maze data from the file.  The
 * functions ComputeMazeSize and InitMazeArray must be called
 * prior to calling ProcessMazeFile.  It is also the caller's
 * responsibility to make sure that infile has been set back
 * to the beginning of the file.
 */

static void ProcessMazeFile(FILE *infile)
{
    int y;

    for (y = mazeHeight; y > 0; y--) {
        ProcessDividerLine(ReadLine(infile), y);
        ProcessPassageLine(ReadLine(infile), y - 1);
    }
    ProcessDividerLine(ReadLine(infile), 0);
}

/*
 * Function: ProcessDividerLine
 * Usage: ProcessDividerLine(line, y);
 * -----------------------------------
 * This function reads the odd-numbered lines in the data file,
 * which specify the positions of the horizontal walls.  The
 * line have the form
 *
 *     +-+-+-+-+-+-+-+-+
 *
 * where the - symbols may be replaced by spaces to indicate a
 * corridor square.  The y value gives the index of the squares
 * immediately to the north of this line.
 */

static void ProcessDividerLine(string line, int y)
{
    pointT pt;

    if (StringLength(line) != 2 * mazeWidth + 1) {
        Error("Divider line has incorrect width");
    }
    pt.y = y;
    for (pt.x = 0; pt.x < mazeWidth; pt.x++) {
        if (line[2 * pt.x] != '+') Error("Missing corner symbol");
        switch (line[2 * pt.x + 1]) {
          case ' ': break;
          case '-': SetHorizontalWall(pt); break;
          default:  Error("Illegal character in maze file");
        }
    }
    if (line[2 * mazeWidth] != '+') Error("Missing corner symbol");
}

/*
 * Function: ProcessPassageLine
 * Usage: ProcessPassageLine(line, y);
 * -----------------------------------
 * This function reads the even-numbered lines in the data file,
 * which specify the passageways and locations of the vertical
 * walls.  These lines have the form
 *
 *     | | | | | | | | |
 *
 * where the | symbols may be replaced by spaces to indicate a
 * corridor square.  One of the open passageway squares in the
 * file may also be marked with an 'S' to indicate the start
 * square.  The y argument gives the index of the squares on
 * this line.
 */

static void ProcessPassageLine(string line, int y)
{
    pointT pt;
    int len;

    pt.y = y;
    len = StringLength(line);
    for (pt.x = 0; pt.x < (len - 1) / 2; pt.x++) {
        if (line[2 * pt.x] == '|') {
            SetVerticalWall(pt);
        }
        switch (line[2 * pt.x + 1]) {
          case ' ': case '\0': break;
          case 'S': SetStartSquare(pt); break;
          default:  Error("Illegal character in maze file");
        }
    }
    if (len % 2 == 1 && line[len - 1] == '|') {
        pt.x = (len - 1) / 2;
        SetVerticalWall(pt);
    }
}

/*
 * Function: SetHorizontalWall
 * Usage: SetHorizontalWall(pt);
 * -----------------------------
 * This function sets a horizontal wall to the south of the square
 * at pt.  To maintain consistency in the data structure, it is
 * usually also necessary to create a wall to the north of the
 * square just south of this one.
 */

static void SetHorizontalWall(pointT pt)
{
    if (InRange(pt.x, pt.y)) {
        maze[pt.x][pt.y].walls[South] = TRUE;
    }
    if (InRange(pt.x, pt.y - 1)) {
        maze[pt.x][pt.y - 1].walls[North] = TRUE;
    }
    MovePen(LeftMargin + pt.x * SquareSize,
            BottomMargin + pt.y * SquareSize);
    DrawLine(SquareSize, 0);
}

/*
 * Function: SetVerticalWall
 * Usage: SetVerticalWall(pt);
 * ---------------------------
 * This function sets a vertical wall to the west of the square
 * at pt.  To maintain consistency in the data structure, it is
 * usually also necessary to create a wall to the east of the
 * square just west of this one.
 */

static void SetVerticalWall(pointT pt)
{
    if (InRange(pt.x, pt.y)) {
        maze[pt.x][pt.y].walls[West] = TRUE;
    }
    if (InRange(pt.x - 1, pt.y)) {
        maze[pt.x - 1][pt.y].walls[East] = TRUE;
    }
    MovePen(LeftMargin + pt.x * SquareSize,
            BottomMargin + pt.y * SquareSize);
    DrawLine(0, SquareSize);
}

/*
 * Function: SetStartSquare
 * Usage: SetStartSquare(pt);
 * --------------------------
 * This function sets the start square to the indicated point and
 * draws a circle on the screen.
 */

static void SetStartSquare(pointT pt)
{
    if (startSquare.x != -1) {
        Error("Multiple start squares specified");
    }
    startSquare = pt;
    DrawCenteredCircle(LeftMargin + (pt.x + 0.5) * SquareSize,
                       BottomMargin + (pt.y + 0.5) * SquareSize,
                       StartRadius);
}

/*
 * Function: DrawCenteredCircle
 * Usage: DrawCenteredCircle(x, y, r);
 * -----------------------------------
 * This function draws a circle of radius r with its
 * center at (x, y).
 */

static void DrawCenteredCircle(double x, double y, double r)
{
    MovePen(x + r, y);
    DrawArc(r, 0, 360);
}

/*
 * Function: DrawMarkInSquare
 * Usage: DrawMarkInSquare(pt);
 * ----------------------------
 * This function draws a mark symbol in the specified square.
 */

static void DrawMarkInSquare(pointT pt)
{
    double x, y;

    x = LeftMargin + (pt.x + 0.5) * SquareSize;
    y = BottomMargin + (pt.y + 0.5) * SquareSize;
    MovePen(x - MarkSize / 2, y - MarkSize / 2);
    DrawLine(MarkSize, MarkSize);
    MovePen(x - MarkSize / 2, y + MarkSize / 2);
    DrawLine(MarkSize, -MarkSize);
    if (PauseTime != 0) Pause(PauseTime);
}

/*
 * Function: InRange
 * Usage: if (InRange(x, y)) . . .
 * -------------------------------
 * This function checks to see if a particular coordinate pair
 * is inside the maze.  This function is included because it is
 * often more convenient than OutsideMaze, which takes a point.
 */

static bool InRange(int x, int y)
{
    return (x >= 0 && x < mazeWidth && y >= 0 && y < mazeHeight);
}

/*
 * Function: TrimTrailingSpace
 * Usage: TrimTrailingSpace(buffer);
 * ---------------------------------
 * This function removes any trailing spaces from a string buffer.
 */

static void TrimTrailingSpace(char buffer[])
{
    int cp;

    cp = StringLength(buffer) - 1;
    while (cp >= 0 && isspace(buffer[cp])) {
        cp--;
    }
    buffer[cp + 1] = '\0';
}


/*
 * Function: FreeOldStorage
 * Usage: FreeOldStorage();
 * ------------------------
 * This function frees any memory allocated to the maze and is
 * called only if more than one maze is read in during the same
 * session.
 */

static void FreeOldStorage(void)
{
    int x;

    for (x = 0; x < mazeWidth; x++) {
        FreeBlock(maze[x]);
    }
    FreeBlock(maze);
}

