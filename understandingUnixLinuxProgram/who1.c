/* who1.c - a first version of the who program
 * open, read UTMP file, and show results
 */
#include<stdio.h>
#include<stdlib.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>

#define SHOWHOST       /* include remote machine on output */

void show_info(struct utmp *);

int main()
{
    struct utmp current_record;   /* read info into here */
    int utmpfd;                   /* read from this descriptor */
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);          /* UTMP_FILE is in utmp.h */
        exit(1);
    }

    while ( read(utmpfd, &current_record, reclen) == reclen )
        show_info(&current_record);
    close(utmpfd);
    return 0;           /* went ok */
}

/*
 * show info()
 * displays contents of the utmp struct in human readable
 * * note * tehese sizes should not be hardwired
 */
void show_info(struct utmp *utbufp)
{
    printf("% -8.8s", utbufp->ut_name); /* the logname */
    printf(" ");                        /* aspace */
    printf("% -8.8s", utbufp->ut_line); /* the tty */
    printf(" ");
    printf("%10ld", utbufp->ut_time);   /* login time */
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);    /* the host */
#endif
    printf("\n");                       /* newline */
}

