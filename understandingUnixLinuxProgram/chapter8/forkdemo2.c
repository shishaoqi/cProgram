/* forkdemo2.c - show how child processes pick up at the return
 *             from fork() and can execute any code they like,
 *             even fok(). Predict number of lines of output
 */

main()
{
  printf("my pid is %d\n", getpid());
  fork();
  fork();
  fork();
  printf("my pid is %d\n", getpid());
}
