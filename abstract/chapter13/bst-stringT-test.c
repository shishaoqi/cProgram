#include <string.h>
#include"genlib.h"
#include"strlib.h"
#include"cmpfn.h"
#include"bst.h"

void IntNodeInit(void *np, void *kp, void *clientData);
void display(void *np, void *clientData);

main()
{
    int i = 0;
    string tempStr;
    //string address;
    string str[11] = {"this", "record", "is", "allocate", "at", "the", "end", \
                      "of", "the", "client", "structure-abcddklfdsjlfkasdfkjlkjkkljkjlkjlkj"
                     };
    bstADT bst;

    bst = NewBST( 20, StringCmpFn, IntNodeInit);//20：数据的字节大小
    while (i < 11) {
        tempStr = CopyString(str[i++]);
        InsertBSTNode(bst, tempStr, NULL);
    }
    MapBST(display, bst, InOrder, NULL);
}

void IntNodeInit(void *np, void *kp, void *clientData)
{
    //20160424时改过来的误区 -- 对应bst.c::RecInsertNode() === 20160425再注：之前的改动是错（代码很早之前，已经测试过）
    //*((void **)np) = *((void**)kp);
    //*((void **)np) = kp;
    memcpy(np, kp, 20);//2016-04-26,对应bst.c::RecInsertNode()内的更改。(此修改成功：内存操作的新认识)
}

void display(void *np, void *clientData)
{
    string str;

    str = (string)np;
    printf("%s\n", str);
}
