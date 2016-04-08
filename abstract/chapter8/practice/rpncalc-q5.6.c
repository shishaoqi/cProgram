#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "void_stack.h"
#include "ref.h"

static void ApplyOperator(char op, stackADT operandStack);
static void HelpCommand(void);
static void ClearStack(stackADT operandStack);
static void DisplayStack(stackADT operandStack);

int main()
{
	stackADT operandStack;
	string line;
	char ch;
	printf("RPN Calculator Simulation (type H for help)\n");
	operandStack = NewStack();
	while(TRUE){
		printf(">");
		line = GetLine();
		ch = toupper(line[0]);
		switch(ch){
		case 'Q': exit(0);
		case 'H': HelpCommand(); break;
		case 'C': ClearStack(operandStack); break;
		case 'S': DisplayStack(operandStack); break;
		default:
			if(isdigit(ch)){
				Push(operandStack, NewRefLong(StringToInteger(line)));
			}else{
				ApplyOperator(ch, operandStack);
			}
			break;
		}
	}
	return 0;
}

static void ApplyOperator(char op, stackADT operandStack)
{
	long lhs, rhs, result;

	rhs = RefToLong(Pop(operandStack));
	lhs = RefToLong(Pop(operandStack));
	switch(op)
	{
		case '+': result = lhs+rhs; break;
		case '-': result = lhs-rhs; break;
		case '*': result = lhs*rhs; break;
		case '/': if(rhs==0)
					  Error("Illegal rhs");
				  else
				      result = lhs/rhs; break;
		default: Error("Illegal operator %c", op);
	}
	printf("%d\n", result);
	Push(operandStack, NewRefLong(result));
}

static void HelpCommand(void)
{
	printf("Enter expressions in Reverse Polish Notation,\n");
	printf("in which operators follow the operands to which\n");
	printf("they apply. Each line consists of a number , an\n");
	printf("operator, or one of the following commands:\n");
	printf("   Q -- Quit the program\n");
	printf("   H -- Display this help message\n");
	printf("   C -- Clear the calculator stack\n");
	printf("   S -- Display all values in the stack\n");
}
static void ClearStack(stackADT stack)
{
	while (!StackIsEmpty(stack)){
		(void) Pop(stack);
	}
}

static void DisplayStack(stackADT stack)
{
	int i, depth;

	printf("Stack: ");
	depth = StackDepth(stack);
	if(depth == 0)
		printf("empty\n");
	else
	{
		for (i=depth-1; i>=0; i--){
		if(i<depth-1) printf(",");
		printf("%d", RefToLong(GetStackElement(stack, i)));
		}
		printf("\n");
	}
}