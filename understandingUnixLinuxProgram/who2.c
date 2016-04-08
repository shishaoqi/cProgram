/* who2.c - read /etc/utmp and list info therein
 *        - suppresses empty records
 *        - formats time nicely
 */
#include<stdio.h>
#include<unistd.h>
#include<utmp.h>
#include<fcntl.h>
#include<time.h>

#define SHOWHOST       /* include remote machine on output */

void showtime(long);
void show_info(struct utmp *);

int main()
{
  struct utmp utbuf;                  /* read info into here */
  int utmpfd;                   /* read from this descriptor */

  if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
    perror(UTMP_FILE);          /* UTMP_FILE is in utmp.h */
    exit(1);
}

    while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf) )
      show_info(&utbuf);
    close(utmpfd);
    return 0;           /* went ok */
  }

/*
 * show info()
 * displays the contents of the utmp struct
 * in human readable form
 * *displays nothing if record has no user name
 */
void show_info(struct utmp *utbufp)
{
  if( utbufp->ut_type != USER_PROCESS )
    return;
  printf("% -8.8s", utbufp->ut_name); /* the logname */
  printf(" ");                        /* aspace */
  printf("% -8.8s", utbufp->ut_line); /* the tty */
  printf(" ");
  showtime(utbufp->ut_time);          /* display time */
  printf(" ");
#ifdef SHOWHOST
  if(utbufp->ut_host[0]!='\0')
   printf("(%s)", utbufp->ut_host);    /* the host */
#endif
  printf("\n");                       /* newline */
}

void showtime( long timeval)
{
  char *cp;
  cp = ctime(&timeval);
  printf("%12.12s", cp+4);             /* pick 12 chars from pos 4 */
}
