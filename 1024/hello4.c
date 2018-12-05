#include <stdio.h>
#include <curses.h>
#include <unistd.h>

void main()
{
	int i;
	initscr();
	clear();
	for(i = 0;i<LINES;i++)
	{
		move(i, i+i);
		if(i%2 == 1)
			standout();
		addstr("Hello, world");
		if(i%2 == 1)
			standend();
	sleep(1);
	refresh();
	move(i, i+i);
	addstr("			");
	}
	endwin();
}

