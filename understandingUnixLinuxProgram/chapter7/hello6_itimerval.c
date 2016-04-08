/* hello4.c
 * purpose show how to use erase, time, and draw for animation
 */
#include<stdio.h>
#include<sys/time.h>
#include<curses.h>
 main()
{
  int i;

  initscr();
  clear();
  for(i=0; i<LINES; i++){
    move(i, i+i);
    if(i%2 == 1)
      standout();
    addstr("Hello, world");
    if(i%2 == 1)
      standend();
    refresh();
    set_ticker(500);
    move(i, i+i);
    addstr("            ");
  }
  endwin();
}

int set_ticker(int sec)
{
  struct itimerval new_timeset;
  long n_sec, n_usecs;

  n_sec = sec/100;  /* int part */
  n_usecs = sec;    /* remainder */

  new_timeset.it_value.tv_sec = 5;

  new_timeset.it_interval.tv_sec = n_sec;
  new_timeset.it_interval.tv_usec = n_usecs;

  return setitimer(ITIMER_REAL, &new_timeset, NULL);
}


