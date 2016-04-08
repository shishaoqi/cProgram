/*
 * File: testscan.c
 * ----------------
 * This program tests the scanner abstraction by breaking
 * a single line into tokens.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "scanadt.h"

main()
{
    scannerADT myScanner;
    string token;
    int nTokens;

    myScanner = NewScanner();
    printf("Enter input line: ");
    SetScannerString(myScanner, GetLine());
    nTokens = 0;
    while (MoreTokensExist(myScanner)) {
        token = ReadToken(myScanner);
        nTokens++;
        printf("%2d: \"%s\"\n", nTokens, token);
    }
    FreeScanner(myScanner);
}
