/*
 * File: parser.c
 * --------------
 * This file implements a version of ReadExp that uses
 * conventional precedence rules.  Thus, the expression
 *
 *        x = 2 * x + y
 *
 * is interpreted as if it had been written
 *
 *        x = ((2 * x) + y))
 *
 * This language can be parsed using the following ambiguous
 * grammar:
 *
 *       E  ->  T
 *       E  ->  E op E
 *
 *       T  ->  integer
 *       T  ->  identifier
 *       T  ->  ( E )
 *
 * Unfortunately, this grammar is not sufficient by itself.  The
 * parser must also provide some way to determine what operators
 * take precedence over others.  Moreover, it must avoid the
 * problem of going into an infinite recursion of trying to read
 * an expression by reading an expression, and so on forever.
 *
 * To solve these problems, this implementation passes a numeric
 * value to the ReadE function that specifies the precedence
 * level for the current subexpression.  As long as ReadE finds
 * operators with a higher precedence, it will read in those
 * operators along with the following subexpression.  If the
 * precedence of the new operator is the same or lower than
 * the prevailing precedence, ReadE returns to the next higher
 * level in the recursive-descent parsing and reads the operator
 * there.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "scanadt.h"
#include "parser.h"
#include "exp.h"

/*
 * Implementation notes: ParseExp
 * ------------------------------
 * This function just calls ReadE to read an expression and then
 * checks to make sure no tokens are left over.
 */

expressionADT ParseExp(scannerADT scanner)
{
    expressionADT exp;

    exp = ReadE(scanner, 0);
    if (MoreTokensExist(scanner)) {
        Error("ParseExp: %s unexpected", ReadToken(scanner));
    }
    return (exp);
}

/*
 * Implementation notes: ReadE
 * Usage: exp = ReadE(scanner, prec);
 * ----------------------------------
 * This function reads an expression from the scanner stream,
 * stopping when it encounters an operator whose precedence is
 * less that or equal to prec.
 */

expressionADT ReadE(scannerADT scanner, int prec)
{
    expressionADT exp, rhs;
    string token;
    int newPrec;

    exp = ReadT(scanner);
    while (TRUE) {
        token = ReadToken(scanner);
        newPrec = Precedence(token);
        if (newPrec <= prec) break;
        rhs = ReadE(scanner, newPrec);
        exp = NewCompoundExp(token[0], exp, rhs);
    }
    SaveToken(scanner, token);
    return (exp);
}

/*
 * Function: ReadT
 * Usage: exp = ReadT(scanner);
 * ----------------------------
 * This function reads a single term from the scanner by matching
 * the input to one of the following grammatical rules:
 *
 *       T  ->  integer
 *       T  ->  identifier
 *       T  ->  ( E )
 *
 * In each case, the first token identifies the appropriate rule.
 */

expressionADT ReadT(scannerADT scanner)
{
    expressionADT exp;
    string token;

    token = ReadToken(scanner);
    if (StringEqual(token, "(")) {
        exp = ReadE(scanner, 0);
        if (!StringEqual(ReadToken(scanner), ")")) {
            Error("Unbalanced parentheses");
        }
    } else if (isdigit(token[0])) {
        exp = NewIntegerExp(StringToInteger(token));
    } else if (isalpha(token[0])) {
        exp = NewIdentifierExp(token);
    } else {
        Error("Illegal term in expression");
    }
    return (exp);
}

int Precedence(string token)
{
    if (StringLength(token) > 1) return (0);
    switch (token[0]) {
      case '=': return (1);
      case '+': case '-': return (2);
      case '*': case '/': return (3);
      default:  return (0);
    }
}
