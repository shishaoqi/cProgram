/*
 * File: remcom.c
 * --------------
 * This program eliminates comments from a file.  This version
 * does not ignore comments that appear in a string constant;
 * that change is left to the reader as an exercise.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

/* Private function prototypes */

static void CopyRemovingComments(FILE *infile, FILE *outfile);

/* Main program */


main()
{
    string filename, temp;
    FILE *infile, *outfile;

    printf("This program removes comments from a file.\n");
    while (TRUE) {
        printf("File name: ");
        filename = GetLine();
        infile = fopen(filename, "r");
        if (infile != NULL) break;
        printf("File %s not found -- try again.\n", filename);
    }
    temp = tmpnam(NULL);
    outfile = fopen(temp, "w");
    if (outfile == NULL) Error("Can't open temporary file");
    CopyRemovingComments(infile, outfile);
    fclose(infile);
    fclose(outfile);
    if (remove(filename) != 0 || rename(temp, filename) != 0) {
        Error("Unable to rename temporary file");
    }
}

/*
 * Function: CopyRemovingComments
 * Usage: CopyRemovingComments(infile, outfile);
 * ---------------------------------------------
 * This function copies one file to another, removing comments
 * as it goes.  The status indicator as to whether a comment is
 * being read is stored in commentFlag.
 */

static void CopyRemovingComments(FILE *infile, FILE *outfile)
{
    int ch, nch;
    bool commentFlag;

    commentFlag = FALSE;
    while ((ch = getc(infile)) != EOF) {
        if (commentFlag) {
            if (ch == '*') {
                nch = getc(infile);
                if (nch == '/') {
                    commentFlag = FALSE;
                } else {
                    ungetc(nch, infile);
                }
            }
        } else {
            if (ch == '/') {
                nch = getc(infile);
                if (nch == '*') {
                    commentFlag = TRUE;
                } else {
                    ungetc(nch, infile);
                }
            }
            if (!commentFlag) putc(ch, outfile);
        }
    }
}
