/*
 * File: parser.h
 * --------------
 * This file acts as the interface to the parser module, which
 * exports functions to parse expressions from a scanner.
 */

#ifndef _parser_h
#define _parser_h

#include "exp.h"
#include "scanadt.h"

/*
 * Function: ParseExp
 * Usage: exp = ParseExp(scanner);
 * -------------------------------
 * This function parses an expression by reading tokens from
 * the scanner, which must be provided by the client.
 */

expressionADT ParseExp(scannerADT scanner);

/*
 * Functions: ReadE, ReadT
 * Usage: exp = ReadE(scanner, prec);
 *        exp = ReadT(scanner);
 * ----------------------------------
 * These functions provide low-level entry points to the parser,
 * which are useful to clients who need to use expression
 * parsing in larger applications.  ReadE(scanner, prec) returns
 * the next expression involving operators whose precedence
 * is at least prec; ReadT returns the next individual term.
 */

expressionADT ReadE(scannerADT scanner, int prec);
expressionADT ReadT(scannerADT scanner);

/*
 * Function: Precedence
 * Usage: prec = Precedence(token);
 * --------------------------------
 * This function returns the precedence of the specified operator
 * token.  If the token is not an operator, Precedence returns 0.
 */

int Precedence(string token);

#endif
