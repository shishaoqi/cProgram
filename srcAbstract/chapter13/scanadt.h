/*
 * File: scanadt.h
 * ---------------
 * This file is the interface to a module that exports an abstract
 * data type to facilitate dividing a string into logical units
 * called "tokens," which are either
 *
 * 1. Strings of consecutive letters and digits representing words
 * 2. One-character strings representing punctuation or separators
 *
 * To use this package, you must first create an instance of a
 * scannerADT by calling
 *
 *      scanner = NewScanner();
 *
 * All other calls to the scanner package take this variable as their
 * first argument to identify a particular instance of the abstract
 * scanner type.
 *
 * You initialize the scanner to hold a particular string by calling
 *
 *      SetScannerString(scanner, str);
 *
 * where str is the string from which tokens should be read.  To
 * retrieve each individual token, you make the following call:
 *
 *      token = ReadToken(scanner);
 *
 * To determine whether any tokens remain to be read, you can call
 * the predicate function MoreTokensExist(scanner).  The ReadToken
 * function returns the empty string after the last token is read.
 *
 * The following code fragment serves as an idiom for processing
 * each token in the string inputString:
 *
 *      scanner = NewScanner();
 *      SetScannerString(scanner, inputString);
 *      while (MoreTokensExist(scanner)) {
 *          token = ReadToken(scanner);
 *          . . . process the token . . .
 *      }
 *
 * This version of scanadt.h also supports the following extensions,
 * which are documented later in the interface:
 *
 *   SaveToken
 *   SetScannerSpaceOption
 *   SetScannerStringOption
 *   SetScannerNumberOption
 */

#ifndef _scanadt_h
#define _scanadt_h

#include "genlib.h"

/*
 * Type: scannerADT
 * ----------------
 * This type is the abstract type used to represent a single instance
 * of a scanner.  As with any abstract type, the details of the
 * internal representation are hidden from the client.
 */

typedef struct scannerCDT *scannerADT;

/*
 * Function: NewScanner
 * Usage: scanner = NewScanner();
 * ------------------------------
 * This function creates a new scanner instance.  All other functions
 * in this interface take this scanner value as their first argument
 * so that they can identify what particular instance of the scanner
 * is in use.  This design makes it possible for clients to have more
 * than one scanner process active at the same time.
 */

scannerADT NewScanner(void);

/*
 * Function: FreeScanner
 * Usage: FreeScanner(scanner);
 * ----------------------------
 * This function frees the storage associated with scanner.
 */

void FreeScanner(scannerADT scanner);

/*
 * Function: SetScannerString
 * Usage: SetScannerString(scanner, str);
 * --------------------------------------
 * This function initializes the scanner so that it will start
 * extracting tokens from the string str.
 */

void SetScannerString(scannerADT scanner, string str);

/*
 * Function: ReadToken
 * Usage: token = ReadToken(scanner);
 * ----------------------------------
 * This function returns the next token from scanner.  If
 * ReadToken is called when no tokens are available, it returns
 * the empty string.  The token returned by ReadToken is always
 * allocated in the heap, which means that clients can call
 * FreeBlock when the token is no longer needed.
 */

string ReadToken(scannerADT scanner);

/*
 * Function: MoreTokensExist
 * Usage: if (MoreTokensExist(scanner)) . . .
 * ------------------------------------------
 * This function returns TRUE as long as there are additional
 * tokens for the scanner to read.
 */

bool MoreTokensExist(scannerADT scanner);

/*
 * Function: SaveToken
 * Usage: SaveToken(scanner, token);
 * ---------------------------------
 * This function stores the token in the scanner data structure
 * in such a way that the next time ReadToken is called, it will
 * return that token without reading any additional characters
 * from the input.
 */

void SaveToken(scannerADT scanner, string token);

/*
 * Functions: SetScannerSpaceOption, GetScannerSpaceOption
 * Usage: SetScannerSpaceOption(scanner, option);
 *        option = GetScannerSpaceOption(scanner);
 * -----------------------------------------------
 * The SetScannerSpaceOption function controls whether the scanner
 * ignores whitespace characters or treats them as valid tokens.
 * By default, the ReadToken function treats whitespace characters,
 * such as spaces and tabs, just like any other punctuation mark.
 * If, however, you call
 *
 *    SetScannerSpaceOption(scanner, IgnoreSpaces);
 *
 * the scanner will skip over any white space before reading a token.
 * You can restore the original behavior by calling
 *
 *    SetScannerSpaceOption(scanner, PreserveSpaces);
 *
 * The GetScannerSpaceOption function returns the current setting
 * of this option.
 */

typedef enum { PreserveSpaces, IgnoreSpaces } spaceOptionT;

void SetScannerSpaceOption(scannerADT scanner, spaceOptionT option);
spaceOptionT GetScannerSpaceOption(scannerADT scanner);

/*
 * Functions: SetScannerNumberOption, GetScannerNumberOption
 * Usage: SetScannerNumberOption(scanner, option);
 *        option = GetScannerNumberOption(scanner);
 * ------------------------------------------------
 * This function controls whether the scanner treats numeric values
 * specially.  The default behavior for the scanner is to treat
 * digits as equivalent to letters.  If you call
 *
 *    SetScannerNumberOption(scanner, ScanNumbersAsIntegers);
 *
 * a token beginning with a digit will end at the first nondigit.
 * (Note that digits can still be scanned as part of a token as in
 * the token "x1".)  If you call
 *
 *    SetScannerNumberOption(scanner, ScanNumbersAsReals);
 *
 * the scanner will return the longest token string that represents
 * a real number, if the next character to be scanned is a digit.
 * The format for a real number is a sequence of digit characters
 * that may include at most one decimal point, optionally followed
 * by the letter 'E' in either upper- or lowercase, an optional sign,
 * and an exponent.  You can restore the default behavior by calling
 *
 *    SetScannerNumberOption(scanner, ScanNumbersAsLetters);
 *
 * Even if the number options are enabled, ReadToken always returns
 * its result as a string, which means that you need to call
 * StringToInteger or StringToReal to convert the token to a number.
 */

typedef enum {
    ScanNumbersAsLetters,
    ScanNumbersAsIntegers,
    ScanNumbersAsReals
} numberOptionT;

void SetScannerNumberOption(scannerADT scanner, numberOptionT option);
numberOptionT GetScannerNumberOption(scannerADT scanner);

/*
 * Functions: SetScannerStringOption, GetScannerStringOption
 * Usage: SetScannerStringOption(scanner, option);
 *        option = GetScannerStringOption(scanner);
 * ------------------------------------------------
 * This function controls how the scanner treats double quotation
 * marks in the input.  The default behavior for the scanner is
 * to treat quotes just like any other punctuation character.
 * If, however, you call
 *
 *    SetScannerStringOption(scanner, ScanQuotesAsStrings);
 *
 * a token beginning with a quotation mark will be scanned up to
 * the closing quotation mark.  The quotation marks are returned
 * as part of the scanned token so that clients can differentiate
 * strings from other token types.  The original behavior can be
 * restored by calling
 *
 *    SetScannerStringOption(scanner, ScanQuotesAsPunctuation);
 *
 * When scanning a string, the scanner recognizes the standard
 * escape sequences from ANSI C, such as \n and \t.
 */

typedef enum {
    ScanQuotesAsPunctuation,
    ScanQuotesAsStrings
} stringOptionT;

void SetScannerStringOption(scannerADT scanner, stringOptionT option);
stringOptionT GetScannerStringOption(scannerADT scanner);

#endif
