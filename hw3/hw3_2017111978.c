#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

void tty_mode(int);
void insert();

void main()
{
	int c;

	tty_mode(0); //save original mode
	tty_mode(3); //set noncanonical, noecho
	while((c=getchar())!='q')
	{
		switch(c)
		{
			case 105:
			       printf("\ninsert mode!\n");
			       insert();
			       break;
			case 49:
				printf("\necho on!\n");
				tty_mode(1);
		 		break;
			case 50:
				printf("\necho off!\n");
				tty_mode(2);
				break;		
		}
	}
	tty_mode(-1); //restore
	printf("\n");

	return;
}

void tty_mode(int how)
{
	static struct termios original_mode;
	struct termios mode;

	tcgetattr(0, &mode);

	if(how == 0) //save current mode
		tcgetattr(0, &original_mode);
	else if(how == -1) //restore
		tcsetattr(0, TCSANOW, &original_mode);
	else if(how == 3) //set start mode
	{	
		mode.c_lflag &= ~ECHO;
		mode.c_lflag &= ~ICANON;
		mode.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &mode);
	}
	else if(how == 1) //c == 1, turn on echo
	{
		mode.c_lflag |= ECHO;	 //turn on echo
		mode.c_lflag &= ~ICANON; //turn off canonical
		mode.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &mode);
	}
	else if(how == 2) //c == 2, turn off echo
	{
		mode.c_lflag &= ~ECHO;   //turn off echo
		mode.c_lflag &= ~ICANON; //turn off canonical
		mode.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &mode);
	}

	return;
}

void insert()
{
	int c;
	while((c=getchar())!=27)
	{
		if(c == 'z')
			c='a';
		else if(islower(c))
			c++;
		putchar(c);
	}
	printf("\ncommand mode!\n");

	return;
}
