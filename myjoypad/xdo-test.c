#include <stdio.h>
#include <stdlib.h>


/*
 * Moving the cursor position with xdotool
 * https://www.semicomplete.com/projects/xdotool/
 * https://github.com/jordansissel/xdotool
 * https://archlinux.org/packages/community/x86_64/xdotool/
 * ^ note: installed xdo using yay  
*/

Display *display = NULL;
xdo_t *xdo = NULL;

void mouse_left_down(int x, int y)
{
	xdo_mousemove(xdo, x, y, 0)
	xdo_mousedown(xdo, CURRENTWINDOW, Button1); 
}

void mouse_left_up(int x, int y)
{
	xdo_mouseup(xdo, CURRENTWINDOW, Button1, 1, 0); 
}

void mouse_left_double_click(int x, int y)
{
	xdo_mousemove(xdo, x, y, 0);
	xdo_click_multiple(xdo, CURRENTWINDOW, Button1, 1, 0);
	doubleclick = TRUE;
}

int main()
{
	display = XOpenDisplay(NULL);
	if(display == NULL) 
	{
		fprintf(stderr, "can't open display!\n"); 
		return -1;
	}
	
	xdo = xdo_new((char*) display);

	//some task here
	// ...

	return 0;
}
