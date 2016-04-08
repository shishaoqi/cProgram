/*
 * File: exp.h
 * -----------
 * This interface defines an abstract type for expressions,
 * which allows the client to represent and manipulate simple
 * binary expression trees.
 */

#ifndef _exp_h
#define _exp_h

#include "genlib.h"

/*
 * Type: expressionADT
 * -------------------
 * This type is used to represent the abstract notion of an
 * expression, such as you might encounter in a C program.
 * An expression is defined recursively to be one of the
 * following:
 *
 * 1. A constant
 * 2. A string representing an identifier
 * 3. Two expressions combined by an operator
 */

typedef struct expressionCDT *expressionADT;

/*
 * Type: exptypeT
 * --------------
 * This enumeration type is used to differentiate the three
 * expression types: integers, identifiers, and compounds.
 */

typedef enum {
    IntegerType,
    IdentifierType,
    CompoundType
} exptypeT;

/* Constructor functions */

/*
 * Function: NewIntegerExp
 * Usage: exp = NewIntegerExp(n);
 * ------------------------------
 * This function allocates a new expression node of type
 * IntegerType containing the integer n as its value.
 */

/*
 * Function: NewIntegerExp
 * Usage: exp = NewIntegerExp(n);
 * ------------------------------
 * This function allocates a new expression node of type
 * IntegerType containing the integer n as its value.
 */

expressionADT NewIntegerExp(int n);

/*
 * Function: NewIdentifierExp
 * Usage: exp = NewIdentifierExp(id);
 * ----------------------------------
 * This function allocates a new expression node of type
 * IdentifierType containing the specified id.
 */

expressionADT NewIdentifierExp(string id);

/*
 * Function: NewCompoundExp
 * Usage: exp = NewCompoundExp(op, lhs, rhs);
 * ------------------------------------------
 * This function allocates a new expression node of type
 * CompoundType, which is composed of the operator (op)
 * and the left and right subexpressions (lhs and rhs).
 */

expressionADT NewCompoundExp(char op,
                             expressionADT lhs,
                             expressionADT rhs);

/* Selector functions */

/*
 * Function: ExpType
 * Usage: tc = ExpType(exp);
 * -------------------------
 * This function returns the type of the expression.
 */

exptypeT ExpType(expressionADT exp);


/*
 * Function: ExpInteger
 * Usage: n = ExpInteger(exp);
 * ---------------------------
 * This function returns the integer stored in an expression,
 * which must be of type IntegerType.
 */

int ExpInteger(expressionADT exp);

/*
 * Function: ExpIdentifier
 * Usage: id = ExpIdentifier(exp);
 * -------------------------------
 * This function returns the identifier stored in an
 * expression, which must be of type IdentifierType.
 */

string ExpIdentifier(expressionADT exp);

/*
 * Function: ExpOperator
 * Usage: op = ExpOperator(exp);
 * -----------------------------
 * This function returns the operator stored in an expression,
 * which must be of type CompoundType.
 */

char ExpOperator(expressionADT exp);

/*
 * Functions: ExpLHS, ExpRHS
 * Usage: lhs = ExpLHS(exp);
 *        rhs = ExpRHS(exp);
 * -------------------------
 * These functions return the left and right subexpressions
 * from exp, which must be of type CompoundType.
 */

expressionADT ExpLHS(expressionADT exp);
expressionADT ExpRHS(expressionADT exp);

#endif
