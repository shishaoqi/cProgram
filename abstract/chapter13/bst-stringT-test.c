#include"genlib.h"
#include"strlib.h"
#include"cmpfn.h"
#include"bst.h"

void IntNodeInit(void *np, void *kp, void *clientData);
void display(void *np, void *clientData);

main()
{
    int i = 0;
    //string address;
    string str[11] = {"this", "record", "is", "allocate", "at", "the", "end", \
                      "of", "the", "client", "structure"
                     };
    bstADT bst;

    bst = NewBST( 20, StringCmpFn, IntNodeInit);
    while (i < 11) {
        InsertBSTNode(bst, CopyString(str[i++]), NULL);
    }
    MapBST(display, bst, InOrder, NULL);
}

void IntNodeInit(void *np, void *kp, void *clientData)
{
    //20160424时改过来的误区 -- 对应bst.c::RecInsertNode()
    //*((void **)np) = kp;
    *((void **)np) = *((void**)kp);
}

void display(void *np, void *clientData)
{
    string str;

    str = (string)np;
    printf("%s\n", str);
}
