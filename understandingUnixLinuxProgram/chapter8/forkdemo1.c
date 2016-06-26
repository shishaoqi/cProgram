/* forkdemo1.c
 * shows how fork creates two processes, distinguishable
 * by the different return values from fork()
 */

#include<stdio.h>

main()
{
    int ret_from_fork, mypid;

    mypid = getpid();                /* who am i? */
    printf("Before: my pid is %d\n", mypid);  /* tell the word */

    ret_from_fork = fork();

    sleep(3);
    printf("After: my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
}
