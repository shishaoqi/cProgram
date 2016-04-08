/*
 * File: iverson.c
 * ---------------
 * This file implements a simple version of ReadExp that uses
 * the simple precedence rule that all operators have equal
 * precedence and are applied in right-to-left order.  Thus,
 * under this rule, the expression
 *
 *        x = 2 * x + y
 *
 * is interpreted as if it had been written
 *
 *        x = (2 * (x + y))
 *
 * with the operators executed strictly in right-to-left order.
 *
 * While this rule is different from conventional mathematical
 * practice and seems a bit bizarre, it is the precedence rule
 * used in the programming language APL, developed by Ken
 * Iverson in the 1960s.  The rule is called Iversonian
 * precedence after its inventor.
 *
 * Iversonian precedence has the advantage that it is very
 * easy to parse by using the following grammar:
 *
 *       E  ->  T
 *       E  ->  T op E
 *
 *       T  ->  integer
 *       T  ->  identifier
 *       T  ->  ( E )
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "scanadt.h"
#include "parser.h"
#include "exp.h"

/* Private function prototypes */

static expressionADT ReadE(scannerADT scanner);
static expressionADT ReadT(scannerADT scanner);
static bool IsOperator(string token);

/* Public entries */

/*
 * Implementation notes: ParseExp
 * ------------------------------
 * This function just calls ReadE to read an expression and then
 * checks to make sure no tokens are left over.
 */

expressionADT ParseExp(scannerADT scanner)
{
    expressionADT exp;

    exp = ReadE(scanner);
    if (MoreTokensExist(scanner)) {
        Error("ParseExp: %s unexpected", ReadToken(scanner));
    }
    return (exp);
}

/* Private functions */

/*
 * Implementation notes: ReadE
 * Usage: exp = ReadE(scanner);
 * ----------------------------
 * This function reads the next expression from the scanner by
 * matching the input to one of the following grammatical rules:
 *
 *       E  ->  T
 *       E  ->  T op E
 *
 * Both right-hand sides start with T, so the code can begin by
 * calling ReadT.  If the next token is an operator, the code
 * creates a compound expression from the term, the operator,
 * and the expression after the operator.
 */

static expressionADT ReadE(scannerADT scanner)
{
    expressionADT exp, rhs;
    string token;

    exp = ReadT(scanner);
    token = ReadToken(scanner);
    if (IsOperator(token)) {
        rhs = ReadE(scanner);
        exp = NewCompoundExp(token[0], exp, rhs);
    } else {
        SaveToken(scanner, token);
    }
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

static expressionADT ReadT(scannerADT scanner)
{
    expressionADT exp;
    string token;

    token = ReadToken(scanner);
    if (StringEqual(token, "(")) {
        exp = ReadE(scanner);
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

/*
 * Function: IsOperator
 * Usage: if (IsOperator(token)) . . .
 * -----------------------------------
 * This function returns TRUE if the token is a legal operator.
 */

static bool IsOperator(string token)
{
    if (StringLength(token) != 1) return (FALSE);
    switch (token[0]) {
      case '+': case '-': case '*': case '/': case '=':
        return (TRUE);
      default:
        return (FALSE);
    }
}
