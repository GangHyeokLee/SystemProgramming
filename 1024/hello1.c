#include <stdio.h>
#include <curses.h>

void main()
{
	initscr();

	clear();
	move(10,20);
	addstr("Hello, world");
	move(LINES-1, 0);

	refresh();
	getch();

	endwin();
}

