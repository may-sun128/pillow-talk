#include <stdio.h>
#include <stdlib.h>
#include <stdio.h> 
#include <X11/X.h> 
#include <X11/Xlib.h> 
#include <X11/Xutil.h> 
#include <assert.h>
#include <unistd.h>
#include <malloc.h>

/*
 * Note: you must link the X library when compiling for this to work 
 * `gcc <program-name>.c -lX11`
 * https://stackoverflow.com/questions/2433447/how-to-set-mouse-cursor-position-in-c-on-linux
*/

static int _XlibErrorHandler(Display *display, XErrorEvent *event)
{
	fprintf(stderr, "An error occured detecting the mouse position\n");
    return True;
}

struct cursor_position
{
    int x, y; 
};

// working, though not returning cursor position but printing it 
struct cursor_position getCursorPosition()
{
	int number_of_screens;
    int i;
    Bool result;
    Window *root_windows;
    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;

    Display *display = XOpenDisplay(NULL);
    assert(display);
    XSetErrorHandler(_XlibErrorHandler);
    number_of_screens = XScreenCount(display);
    fprintf(stderr, "There are %d screens available in this X session\n", number_of_screens);
    root_windows = malloc(sizeof(Window) * number_of_screens);
    for (i = 0; i < number_of_screens; i++) {
        root_windows[i] = XRootWindow(display, i);
    }
    for (i = 0; i < number_of_screens; i++) {
        result = XQueryPointer(display, root_windows[i], &window_returned,
                &window_returned, &root_x, &root_y, &win_x, &win_y,
                &mask_return);
        if (result == True) {
            break;
        }
    }
    if (result != True) {
        fprintf(stderr, "No mouse found.\n");
        // return -1;
    }
    // printf("Mouse is at (%d,%d)\n", root_x, root_y);
    
    // -------------------------------------------------------------
    struct cursor_position cp;
    cp.x = root_x;
    cp.y = root_y; 
    // -------------------------------------------------------------
    
    free(root_windows);
    XCloseDisplay(display);
    // return 0;
    return cp; 
}

// working 
void moveMouse()
{
	Display *dpy;
	Window root_window; 

	dpy = XOpenDisplay(0);
	root_window = XRootWindow(dpy, 0);
	XSelectInput(dpy, root_window, KeyReleaseMask);

	// XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y)

	// top left
	XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, 0, 0);

	XFlush(dpy);
}

int main()
{
    // Variables 
    // int number_of_screens;
    // int i;
    // Bool result;
    // Window *root_windows;
    // Window window_returned;
    // int root_x, root_y;
    // int win_x, win_y;
    // unsigned int mask_return;

    // Display *dpy;
	// Window root_window;

	// moveMouse();
    struct cursor_position cp; 
	cp = getCursorPosition();
	printf("Mouse is at (%d,%d)\n", cp.x, cp.y);
    return 0;
}

