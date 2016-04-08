/*
 * File: exp.c
 * -----------
 * This file implements the exp.h interface.  The exported
 * functions are standard constructor and selector functions
 * that require no individual documentation.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"

/*
 * Type: expressionCDT
 * -------------------
 * The type expressionCDT is the concrete counterpart of an
 * expressionADT.  The type is implemented as a pointer to a
 * node.  The value of the node consists of a tagged union that
 * allows the node to have multiple representations.
 */

struct expressionCDT {
    exptypeT type;
    union {
        int intRep;
        string idRep;
        struct {
            char op;
            expressionADT lhs;
            expressionADT rhs;
        } compoundRep;
    } value;
};

/* Constructor functions */

expressionADT NewIntegerExp(int n)
{
    expressionADT exp;

    exp = New(expressionADT);
    exp->type = IntegerType;
    exp->value.intRep = n;
    return (exp);
}

expressionADT NewIdentifierExp(string id)
{
    expressionADT exp;

    exp = New(expressionADT);
    exp->type = IdentifierType;
    exp->value.idRep = CopyString(id);
    return (exp);
}

expressionADT NewCompoundExp(char op,
                             expressionADT lhs,
                             expressionADT rhs)
{
    expressionADT exp;

    exp = New(expressionADT);
    exp->type = CompoundType;
    exp->value.compoundRep.op = op;
    exp->value.compoundRep.lhs = lhs;
    exp->value.compoundRep.rhs = rhs;
    return (exp);
}

/* Selector functions */

exptypeT ExpType(expressionADT exp)
{
    return (exp->type);
}

int ExpInteger(expressionADT exp)
{
    if (ExpType(exp) != IntegerType) {
        Error("Integer expression required");
    }
    return (exp->value.intRep);
}

string ExpIdentifier(expressionADT exp)
{
    if (ExpType(exp) != IdentifierType) {
        Error("Identifier expression required");
    }
    return (exp->value.idRep);
}

char ExpOperator(expressionADT exp)
{
    if (ExpType(exp) != CompoundType) {
        Error("Compound expression required");
    }
    return (exp->value.compoundRep.op);
}

expressionADT ExpLHS(expressionADT exp)
{
    if (ExpType(exp) != CompoundType) {
        Error("Compound expression required");
    }
    return (exp->value.compoundRep.lhs);
}

expressionADT ExpRHS(expressionADT exp)
{
    if (ExpType(exp) != CompoundType) {
        Error("Compound expression required");
    }
    return (exp->value.compoundRep.rhs);
}
