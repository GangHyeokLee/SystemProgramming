/* who3.c - who with buffered reads
 *		  - supresses empty records\
 *		  - formats time nicely
		  - buffers input (using utmplib)
 *    ���ݱ����� �ϳ��� �о�Դµ� �׷��� �ϸ� �ð� ��û �Դ´�
 *    �׷��� �ý����� �ּ�ȭ �ϱ� ���� ��
 */


#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#define SHOWHOST		/* include remote machine on output */

void showtime(time_t);
void show_info( struct utmp* );
int main()
{
	struct utmp *utbufp,			/* holds pointer to next rec */
				*utmp_next(),
				*utmp_close();       /* returns pointer to next   */

	int utmp_open(), utmp_reload();
	if(utmp_open(UTMP_FILE) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	while((utbufp = utmp_next()) != ((struct utmp *) NULL))
		show_info(utbufp);
	utmp_close();
	return 0;
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