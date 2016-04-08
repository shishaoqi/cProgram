#include"genlib.h"
#include"stack.h"
#include"ref.h"


int main()
{
    int n;
    stackElementT val;
    stackADT stack;

    stack = NewStack();
    printf("Enter a list of integers, ending with 0:\n");
    while(TRUE)
    {
        printf(">");
        scanf("%d",&n);
        if(n==0){
            printf("Those integers in reverse order are:\n");
            while(StackIsEmpty(stack)==FALSE)
            {
                val=Pop(stack);
                printf("%d\n", RefToInt(val));
            }
            break;
        }else
            Push(stack,NewRefInt(n));
        }
        return 0;
    }
}


