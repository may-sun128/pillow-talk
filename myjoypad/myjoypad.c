/**
 * Author: Jason White
 *
 * Description:
 * Reads joystick/gamepad events and displays them.
 *
 * Compile:
 * gcc joystick.c -o joystick
 *
 * Run:
 * ./joystick [/dev/input/jsX]
 *
 * See also:
 * https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>

#include <stdio.h>
#include <stdlib.h>
// #include <stdio.h> 
#include <X11/X.h> 
#include <X11/Xlib.h> 
#include <X11/Xutil.h> 
#include <assert.h>
#include <unistd.h>
#include <malloc.h>

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[3])
{
    size_t axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}

void MoveMouse()
{
    //
}

int main(int argc, char *argv[])
{
    struct cursor_position cp; 
	cp = getCursorPosition();
	printf("Mouse is at (%d,%d)\n", cp.x, cp.y);
    return 0;
    
    // ----------------- ^ mouse.c ----------------------------

    const char *device;
    int js;
    // js_event is from linux/joystick.h
    struct js_event event;
    struct axis_state axes[3] = {0};
    size_t axis;

    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    __u8 x_button = 0;
    __u8 a_button = 1; 
    __u8 b_button = 2;
    __u8 y_button = 3; 
    __u8 left_trigger = 4; 
    __u8 right_trigger = 5;
    __u8 select_button = 8; 
    __u8 start_button = 9;

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                // printf("Event Number:%u\n Event Value:%s\n", event.number, event.value ? "pressed" : "released");
                // printf("Event Number:%u\n Event Value:%s\n", event.number, event.value);
                // printf("Event Number:%u\n", event.number);
                // printf("Event Type:%u\n", event.type);
                // printf("Event Value:%s\n", event.value);
                
                // if event.value == true, ie if is_pressed 
                if(event.value) 
                {
                    if(event.number == x_button)
                    {
                        putchar('0');
                        putchar('\n');
                    }
                    else if(event.number == a_button)
                    {
                        putchar('1');
                        putchar('\n');
                    }
                    else if(event.number == b_button)
                    {
                        putchar('2');
                        putchar('\n');
                    }
                    else if(event.number == y_button)
                    {
                        putchar('3');
                        putchar('\n');
                    }
                    else if(event.number == left_trigger)
                    {
                        putchar('4');
                        putchar('\n');
                    }
                    else if(event.number == right_trigger)
                    {
                        putchar('5');
                        putchar('\n');
                    }
                    else if(event.number == select_button)
                    {
                        putchar('8');
                        putchar('\n');
                    }
                    else if(event.number == start_button)
                    {
                        putchar('9');
                        putchar('\n');
                    }
                }

            // case JS_EVENT_AXIS:
            //     axis = get_axis_state(&event, axes);
            //     if (axis < 3)
            //         printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
            //     break;
            default:
                /* Ignore init events. */
                break;
        }
        
        fflush(stdout);
    }

    close(js);
    return 0;
}


// -------------- mouse.c -----------------------------------------------------------------

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

// int main()
// {
//     struct cursor_position cp; 
// 	cp = getCursorPosition();
// 	printf("Mouse is at (%d,%d)\n", cp.x, cp.y);
//     return 0;
// }