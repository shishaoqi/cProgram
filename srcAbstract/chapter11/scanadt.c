/*
 * File: scanadt.c
 * ---------------
 * This file implements the scanadt.h interface.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "scanadt.h"

/*
 * Type: scannerCDT
 * ----------------
 * This structure is the concrete representation of the type
 * scannerADT, which is exported by this interface.  Its purpose
 * is to maintain the state of the scanner between calls.  The
 * details of the representation are invisible to the client,
 * but consist of the following fields:
 *
 * str          -- Copy of string passed to SetScannerString
 * len          -- Length of string, saved for efficiency
 * cp           -- Current character position in the string
 * savedToken   -- String saved by SaveToken (NULL indicates none)
 * spaceOption  -- Setting of the space option extension
 * numberOption -- Setting of the number option extension
 * stringOption -- Setting of string option extension
 */

struct scannerCDT {
    string str;
    int len;
    int cp;
    string savedToken;
    spaceOptionT spaceOption;
    numberOptionT numberOption;
    stringOptionT stringOption;
};

/* Private function prototypes */

static void SkipSpaces(scannerADT scanner);
static int ScanToEndOfIdentifier(scannerADT scanner);
static int ScanToEndOfInteger(scannerADT scanner);
static int ScanToEndOfReal(scannerADT scanner);
static string ScanQuotedString(scannerADT scanner);
static char ScanEscapeCharacter(scannerADT scanner);

/* Exported entries */

scannerADT NewScanner(void)
{
    scannerADT scanner;

    scanner = New(scannerADT);
    scanner->str = NULL;
    scanner->spaceOption = PreserveSpaces;
    scanner->numberOption = ScanNumbersAsLetters;
    scanner->stringOption = ScanQuotesAsPunctuation;
    return (scanner);
}

void FreeScanner(scannerADT scanner)
{
    if (scanner->str != NULL) FreeBlock(scanner->str);
    FreeBlock(scanner);
}

void SetScannerString(scannerADT scanner, string str)
{
    if (scanner->str != NULL) FreeBlock(scanner->str);
    scanner->str = CopyString(str);
    scanner->len = StringLength(str);
    scanner->cp = 0;
    scanner->savedToken = NULL;
}

string ReadToken(scannerADT scanner)
{
    char ch;
    string token;
    int start, finish;

    if (scanner->str == NULL) {
        Error("SetScannerString has not been called");
    }
    if (scanner->savedToken != NULL) {
        token = scanner->savedToken;
        scanner->savedToken = NULL;
        return (token);
    }
    if (scanner->spaceOption == IgnoreSpaces) SkipSpaces(scanner);
    start = finish = scanner->cp;
    if (start >= scanner->len) return (CopyString(""));
    ch = scanner->str[scanner->cp];
    if (scanner->stringOption == ScanQuotesAsStrings
            && ch == '"') {
        return (ScanQuotedString(scanner));
    }
    if (isdigit(ch)) {
        switch (scanner->numberOption) {
          case ScanNumbersAsLetters:
            finish = ScanToEndOfIdentifier(scanner);
            break;
          case ScanNumbersAsIntegers:
            finish = ScanToEndOfInteger(scanner);
            break;
          case ScanNumbersAsReals:
            finish = ScanToEndOfReal(scanner);
            break;
        }
    } else if (isalnum(ch)) {
        finish = ScanToEndOfIdentifier(scanner);
    } else {
        scanner->cp++;
    }
    return (SubString(scanner->str, start, finish));
}

bool MoreTokensExist(scannerADT scanner)
{
    if (scanner->str == NULL) {
        Error("SetScannerString has not been called");
    }
    if (scanner->savedToken != NULL) {
        return (!StringEqual(scanner->savedToken, ""));
    }
    if (scanner->spaceOption == IgnoreSpaces) SkipSpaces(scanner);
    return (scanner->cp < scanner->len);
}

void SaveToken(scannerADT scanner, string token)
{
    if (scanner->str == NULL) {
        Error("SetScannerString has not been called");
    }
    if (scanner->savedToken != NULL) {
        Error("Token has already been saved");
    }
    scanner->savedToken = token;
}

void SetScannerSpaceOption(scannerADT scanner, spaceOptionT option)
{
    scanner->spaceOption = option;
}

spaceOptionT GetScannerSpaceOption(scannerADT scanner)
{
    return (scanner->spaceOption);
}

void SetScannerNumberOption(scannerADT scanner, numberOptionT option)
{
    scanner->numberOption = option;
}

numberOptionT GetScannerNumberOption(scannerADT scanner)
{
    return (scanner->numberOption);
}

void SetScannerStringOption(scannerADT scanner, stringOptionT option)
{
    scanner->stringOption = option;
}

stringOptionT GetScannerStringOption(scannerADT scanner)
{
    return (scanner->stringOption);
}

/* Private functions */

/*
 * Function: SkipSpaces
 * Usage: SkipSpaces(scanner);
 * ---------------------------
 * This function advances the position of the scanner until the
 * current character is not a whitespace character.
 */

static void SkipSpaces(scannerADT scanner)
{
    while (isspace(scanner->str[scanner->cp])) {
        scanner->cp++;
    }
}

/*
 * Function: ScanToEndOfIdentifier
 * Usage: finish = ScanToEndOfIdentifier(scanner);
 * -----------------------------------------------
 * This function advances the position of the scanner until it
 * reaches the end of a sequence of letters or digits that make
 * up an identifier.  The return value is the index of the last
 * character in the identifier; the value of the stored index
 * cp is the first character after that.
 */

static int ScanToEndOfIdentifier(scannerADT scanner)
{
    while (isalnum(scanner->str[scanner->cp])) {
        scanner->cp++;
    }
    return (scanner->cp - 1);
}

/*
 * Function: ScanToEndOfInteger
 * Usage: finish = ScanToEndOfInteger(scanner);
 * --------------------------------------------
 * This function advances the position of the scanner until it
 * reaches the end of a sequence of digits.  The return value is
 * the index of the last character in the integer; the value
 * of the stored index cp is the first character after that.
 */

static int ScanToEndOfInteger(scannerADT scanner)
{
    while (isdigit(scanner->str[scanner->cp])) {
        scanner->cp++;
    }
    return (scanner->cp - 1);
}

/*
 * Function: ScanToEndOfReal
 * Usage: finish = ScanToEndOfReal(scanner);
 * -----------------------------------------
 * This function advances the position of the scanner until it
 * reaches the end of a legal number.  The return value is the
 * index of the last character in the identifier; the value
 * of the stored index cp is the first character after that.
 *
 * The function operates by simulating what computer scientists
 * call a finite-state machine.  The program uses the variable
 * state to record the history of the process and determine
 * what characters would be legal at this point in time.
 */

#define InitialState        0
#define BeforeDecimalPoint  1
#define AfterDecimalPoint   2
#define StartingExponent    3
#define FoundExponentSign   4
#define ScanningExponent    5
#define FinalState         -1

static int ScanToEndOfReal(scannerADT scanner)
{
    int state;
    char ch;

    state = InitialState;
    while (state != FinalState) {
        ch = scanner->str[scanner->cp];
        switch (state) {
          case InitialState:
            if (!isdigit(ch)) Error("ScanToEndOfReal called on nonnumber");
             state = BeforeDecimalPoint;
            break;
          case BeforeDecimalPoint:
            if (ch == '.') {
                state = AfterDecimalPoint;
            } else if (ch == 'E' || ch == 'e') {
                state = StartingExponent;
            } else if (!isdigit(ch)) {
                state = FinalState;
            }
            break;
          case AfterDecimalPoint:
            if (ch == 'E' || ch == 'e') {
                state = StartingExponent;
            } else if (!isdigit(ch)) {
                state = FinalState;
            }
            break;
          case StartingExponent:
            if (ch == '+' || ch == '-') {
                state = FoundExponentSign;
            } else if (isdigit(ch)) {
                state = ScanningExponent;
            } else {
                scanner->cp--;
                state = FinalState;
            }
            break;
          case FoundExponentSign:
            if (isdigit(ch)) {
                state = ScanningExponent;
            } else {
                scanner->cp -= 2;
                state = FinalState;
            }
            break;
          case ScanningExponent:
            if (!isdigit(ch)) {
                state = FinalState;
            }
            break;
        }
        if (state != FinalState) scanner->cp++;
    }
    return (scanner->cp - 1);
}

/*
 * Function: ScanQuotedString
 * Usage: str = ScanQuotedString(scanner);
 * ---------------------------------------
 * This function reads and returns a quoted string from the
 * scanner.  The current position of the scanner must be the
 * opening quotation mark.  At the end, the current position
 * marker will be set to the character position after the
 * closing quote.  The scanner generates an error if there
 * is no closing quotation mark before the end of the string.
 */

static string ScanQuotedString(scannerADT scanner)
{
    string token;
    char ch;

    token = "\"";
    scanner->cp++;
    while ((ch = scanner->str[scanner->cp++]) != '"') {
        if (ch == '\0') Error("Unterminated string");
        if (ch == '\\') ch = ScanEscapeCharacter(scanner);
        token = Concat(token, CharToString(ch));
    }
    return (Concat(token, "\""));
}

/*
 * Function: ScanEscapeCharacter
 * Usage: ch = ScanEscapeCharacter(scanner);
 * -----------------------------------------
 * This function is used by ScanQuotedString to read an escape
 * character sequence from the scanner string.  The opening
 * backslash character has already been read.  Most of the
 * complexity comes in reading the '\ddd' and '\xdd' forms.
 */

static char ScanEscapeCharacter(scannerADT scanner)
{
    char ch, result;
    int digit, base;

    ch = scanner->str[scanner->cp];
    if (isdigit(ch) || ch == 'x') {
        base = 8;
        if (ch == 'x') {
            base = 16;
            scanner->cp++;
        }
        result = 0;
        while (TRUE) {
            ch = scanner->str[scanner->cp];
            if (isdigit(ch)) {
                digit = ch - '0';
            } else if (isalpha(ch)) {
                digit = toupper(ch) - 'A' + 10;
            } else {
                digit = base;
            }
            if (digit >= base) break;
            result = base * result + digit;
            scanner->cp++;
        }
        return (result);
    }
    scanner->cp++;
    switch (ch) {
      case 'a':  return ('\a');
      case 'b':  return ('\b');
      case 'f':  return ('\f');
      case 'n':  return ('\n');
      case 'r':  return ('\r');
      case 't':  return ('\t');
      case 'v':  return ('\v');
      case '\\': return ('\\');
      case '"':  return ('"');
    }
}

