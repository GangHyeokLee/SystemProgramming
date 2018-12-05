/* who2.c - read /var/adm/utmp and list info therein
 *		  - supresses empty records\
 *		  - formats time nicely
 *    시간 알아볼 수 있게 만드는거래 리눅스가 1970년 언젠가부터 계속 1초씩 더해와서 못알아봄
 *		그래서 사람말로 만드는거
 */


#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define SHOWHOST		/* include remote machine on output */

void showtime(long);
void show_info( struct utmp* );
int main()
{
	struct utmp utbuf;			/* read info into here		 */
	int			utmpfd;			/* read from this descriptor */

	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	while(read (utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	close(utmpfd);
	return 0;					/* went ok */
}

/*
 *  show info()
 *      displays contents of the utmp struct in human readable form
 *		*note* these sizes should not be hardwired
 */
void show_info( struct utmp* utbufp)
{
		if(utbufp->ut_type != USER_PROCESS)
			return;
	
		printf("%-8.8s", utbufp->ut_name);  /* the logname */
		printf("  ");
		printf("%-8.8s", utbufp->ut_line);  /* the tty     */
		printf("  ");
		showtime( utbufp->ut_time);		    /* display time*/	

#ifdef SHOWHOST
		printf(" (%s)", utbufp->ut_host);   /* the host    */
#endif
		printf("\n");                       /* newline     */
}

void showtime( long timeval)
/*
 *		display time in a format fit for human consumption
 *		uses ctime to build a string then picks parts out of it
 *		Note : %12.12s prints a string 12 chars wide and LIMITS
 *		it to 12chars
 */

{
		char	*cp;					/* to hold address of time	   */
		cp = ctime(&timeval);			/* convert time to string	   */
										/* string looks like	       */
										/* Mon Feb 4 00:46:00 EST 1991 */
		printf("%12.12s", cp+4);		/* pick 12 chars from pos 4	   */
}
