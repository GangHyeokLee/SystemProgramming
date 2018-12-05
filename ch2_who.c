/* whol.c - a first version of the who program \
 *			   open, readm UTMP file, and show results
 */


#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define SHOWHOST		/* include remote machine on output */

void show_info( struct utmp* );
int main()
{
	struct utmp current_record; /* read info into here		 */
	int			utmpfd;			/* read from this descriptor */
	int			reclen = sizeof(current_record);

	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}
	while(read (utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
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
	
		printf("%-8.8s", utbufp->ut_name);   /* the logname */
		printf("  ");
		printf("%-8.8s", utbufp->ut_line);  /* the tty     */
		printf("  ");
		printf("%101d", utbufp->ut_time);  /* login time  */
		printf("  ");

#ifdef SHOWHOST
		printf(" (%s)", utbufp->ut_host);   /* the host    */
#endif
		printf("\n");                       /* newline     */
}
