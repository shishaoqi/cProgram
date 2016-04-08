/*
 * File: bigint.c
 * --------------
 * This file implements the bigint.h interface, which provides
 * addition and multiplication operations for nonnegative
 * integers of arbitrary size.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "bigint.h"

/*
 * Type: bigIntCDT
 * ---------------
 * This type is the concrete counterpart of the abstract type
 * bigIntADT.  The underlying representation is a linked list
 * of cells, each of which contains a single digit.  The digits
 * in the list appear in reverse order, with the units digit at
 * the beginning of the list, the tens digit next, and so on.
 * In its recursive formulation, a bigIntADT is either
 *
 * 1. NULL, representing the integer 0, or
 * 2. A digit d and a bigIntADT n, representing 10 * n + d
 */

struct bigIntCDT {
    int finalDigit;
    bigIntADT leadingDigits;
};

/* Private function prototypes */

static bigIntADT AddWithCarry(bigIntADT n1, bigIntADT n2, int carry);
static bigIntADT MultiplyDigit(int digit, bigIntADT n);
static bigIntADT DigitCons(bigIntADT n, int digit);
static bigIntADT LeadingDigits(bigIntADT n);
static int FinalDigit(bigIntADT n);

/* Exported entries */

bigIntADT NewBigInt(int i)
{
    if (i < 0) Error("Negative integers are not permitted");
    if (i == 0) return (NULL);
    return (DigitCons(NewBigInt(i / 10), i % 10));
}

bigIntADT StringToBigInt(string str)
{
    int len;
    char ch;

    len = StringLength(str);
    if (len == 0) return (NULL);
    ch = str[len - 1];
    if (!isdigit(ch)) Error("Illegal digit %c", ch);
    return (DigitCons(StringToBigInt(SubString(str, 0, len - 2)),
                      ch - '0'));
}

string BigIntToString(bigIntADT n)
{
    string str;

    str = CharToString(FinalDigit(n) + '0');
    if (LeadingDigits(n) != NULL) {
        str = Concat(BigIntToString(LeadingDigits(n)), str);
    }
    return (str);
}

/*
 * Implementation notes: AddBigInt
 * -------------------------------
 * The AddBigInt function itself is a simple wrapper that calls
 * AddWithCarry to do the real work.   The third argument to
 * AddWithCarry is the carry from the preceding digit position.
 */

bigIntADT AddBigInt(bigIntADT n1, bigIntADT n2)
{
    return (AddWithCarry(n1, n2, 0));
}

static bigIntADT AddWithCarry(bigIntADT n1, bigIntADT n2, int carry)
{
    int sum;
    bigIntADT p1, p2;

    p1 = LeadingDigits(n1);
    p2 = LeadingDigits(n2);
    sum = FinalDigit(n1) + FinalDigit(n2) + carry;
    if (sum == 0 && p1 == NULL && p2 == NULL) return (NULL);
    return (DigitCons(AddWithCarry(p1, p2, sum / 10), sum % 10));
}

/*
 * Function: MultiplyBigInt
 * ------------------------
 * The MultiplyBigInt implementation uses the standard
 * elementary-school algorithm of multiplying by each
 * digit in turn, except that the algorithm is implemented
 * recursively here.  The recursive insight is that the
 * product of n1 and n2 is
 *
 *   FinalDigit(n1) * n2 + LeadingDigits(n1) * n2 * 10
 *
 * Note that multiplying by 10 is implemented simply by
 * adding a 0 to the number using DigitCons.  Single-digit
 * multiplication is implemented by recursive addition.
 */

bigIntADT MultiplyBigInt(bigIntADT n1, bigIntADT n2)
{
    if (n1 == NULL) return (NULL);
    return (AddBigInt(MultiplyDigit(FinalDigit(n1), n2),
                      MultiplyBigInt(LeadingDigits(n1),
                                     DigitCons(n2, 0))));
}

static bigIntADT MultiplyDigit(int digit, bigIntADT n)
{
    if (digit == 0) return (NULL);
    return (AddBigInt(n, MultiplyDigit(digit - 1, n)));
}

/*
 * Function: DigitCons
 * Usage: n = DigitCons(leadingDigits, finalDigit);
 * ------------------------------------------------
 * This low-level constructor function combines leadingDigits
 * and finalDigit to make a new bigIntADT.  Note that the
 * direction of this function is opposite to that of ListCons
 * in that finalDigit is added to the beginning of the linked
 * list.  The reason for using this argument order is that
 * doing so makes it easier to understand the function in
 * its arithmetic sense: finalDigit is added to the end of
 * leadingDigits.  The implementation also includes a check
 * to make sure that 0 is always represented as NULL.
 */

static bigIntADT DigitCons(bigIntADT leadingDigits, int finalDigit)
{
    bigIntADT cp;

    if (leadingDigits == NULL && finalDigit == 0) return (NULL);
    cp = New(bigIntADT);
    cp->finalDigit = finalDigit;
    cp->leadingDigits = leadingDigits;
    return (cp);
}

/*
 * Functions: LeadingDigits, FinalDigit
 * Usage: prefix = LeadingDigits(n);
 *        last = FinalDigit(n);
 * ------------------------------------
 * These functions select the leading digits and final digit
 * of a bigIntADT, respectively.  Because these functions are
 * defined locally in the implementation, callers could select
 * the fields directly.  However, by making these functions,
 * the special-case handling of NULL as 0 becomes much easier
 * to implement.
 */

static bigIntADT LeadingDigits(bigIntADT n)
{
    return ((n == NULL) ? NULL : n->leadingDigits);
}

static int FinalDigit(bigIntADT n)
{
    return ((n == NULL) ? 0 : n->finalDigit);
}
