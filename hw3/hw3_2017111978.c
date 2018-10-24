#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main()
{
	int c;

	tty_mode(0);
	while((c=getchar())!='q')
	{
		switch(c)
		{
			case 27:
				printf("command mode!\n");
				break;
			case 
	}
	tty_mode(1);
}

void tty_mode(int how)
{
	static struct terminos original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);
}
