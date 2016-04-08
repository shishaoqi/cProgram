/*
 * File: eval.h
 * ------------
 * This file is the interface to the evaluator module, which
 * computes the value represented by an expression.  This module
 * also maintains the state of the variables, which are only
 * updated or inspected during the evaluation phase.
 */

#ifndef _eval_h
#define _eval_h

#include "exp.h"

/*
 * Function: EvalExp
 * Usage: n = EvalExp(exp);
 * ------------------------
 * This function evaluates an expression and returns its value.
 */

int EvalExp(expressionADT exp);

/*
 * Function: InitVariableTable
 * Usage: InitVariableTable();
 * ---------------------------
 * This function initializes the table used to hold the variables.
 * All variables are initially undefined.
 */

void InitVariableTable(void);

/*
 * Function: GetIdentifierValue
 * Usage: value = GetIdentifierValue(name);
 * ----------------------------------------
 * This function returns the value of an identifier.
 */

int GetIdentifierValue(string name);

/*
 * Function: SetIdentifierValue
 * Usage: SetIdentifierValue(name, value);
 * ---------------------------------------
 * This function sets the identifier to have the specified value.
 */

void SetIdentifierValue(string name, int value);

#endif
