#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h> 
#include <X11/X.h> 
#include <X11/Xlib.h> 
#include <X11/Xutil.h> 
#include <assert.h>
#include <unistd.h>
#include <malloc.h>
// #include <unistd.h>
#include <time.h>
#include <errno.h>

/*
 * Note: you must link the X library when compiling for this to work 
 * `gcc <program-name>.c -lX11`
 * https://stackoverflow.com/questions/2433447/how-to-set-mouse-cursor-position-in-c-on-linux
*/

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
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

// static int _XlibErrorHandler(Display *display, XErrorEvent *event)
// {
// 	fprintf(stderr, "An error occured detecting the mouse position\n");
//     return True;
// }

struct cursor_position
{
    int x, y; 
};

struct cursor_position getCursorPosition(Display *display, Window root_window)
{
	int number_of_screens;
    int i;
    Bool result;
    Window *root_windows;
    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;

    //// added to parameters 
    //// Display *display = XOpenDisplay(NULL);
    //// not sure what this does; error handling? 
    //// assert(display);

    //// XSetErrorHandler(_XlibErrorHandler);
    number_of_screens = XScreenCount(display);
    //// fprintf(stderr, "There are %d screens available in this X session\n", number_of_screens);
    
    //// passing in root window in as argument
    //// root_windows = malloc(sizeof(Window) * number_of_screens);
    
    //// Do I really need number of screens? Let's find out 
    // for (i = 0; i < number_of_screens; i++) {
    //     root_windows[i] = XRootWindow(display, i);
    // }
    // for (i = 0; i < number_of_screens; i++) {
    //     result = XQueryPointer(display, root_windows[i], &window_returned,
    //             &window_returned, &root_x, &root_y, &win_x, &win_y,
    //             &mask_return);
    //     if (result == True) {
    //         break;
    //     }
    // }
    // if (result != True) {
    //     fprintf(stderr, "No mouse found.\n");
    //     // return -1;
    // }
    // printf("Mouse is at (%d,%d)\n", root_x, root_y);
    
    // definition 
    // Bool  XQueryPointer(display, w,           root_return,      child_return,     root_x_return, root_y_return, win_x_return, win_y_return, mask_return)
    result = XQueryPointer(display, root_window, &window_returned, &window_returned, &root_x,       &root_y,       &win_x,       &win_y,       &mask_return);

    struct cursor_position cp;
    cp.x = root_x;
    cp.y = root_y; 
    
    //// free(root_windows);
    //// No need to close display 
    //// XCloseDisplay(display);
    //// returning coords instead of status code 
    // return 0;
    return cp; 
}
 
void moveMouse(int x, int y, Display *dpy, Window root_window)
{
	// Request that the xserver report events associated with the specific event mask 
    // Initially, X will not report any of these events. ; 
    XSelectInput(dpy, root_window, KeyReleaseMask);
	// Move the cursor 
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
	// flush the output buffer 
    // not sure if this event should happen here or in main 
    XFlush(dpy);
}

void printEventInfo(struct js_event e)
{
    printf("Time:%d\nValue:%d\nType:%d\nNumber:%d\n----------------------------------\n", e.time, e.value, e.type, e.number);
}

void processEvent(struct js_event e)
{
    struct js_event buf[2]; 
    buf[0] = e;
}

int main()
{
    // event struct, contains the following: 
    // __u32 time  - event timestamp in miliseconds 
    // __s16 value - value 
    // __u8 type   - event type 
    // __u8 number - axis/button number 
    struct js_event e;
    // open js0 in non-blocking mode 
    // I suspect the non-blocking aspect is why 100% of CPU is used 
    int fd = open("/dev/input/js0", O_NONBLOCK);
    
    struct js_event current_event;

    // copied from old main

    // x11 display struct 
    // Looks like it contains:
    // hostname - the hostname of the machine in which the display is physically attached 
    // number   - the number of the display server on the host machine (starting w/ 0)
    // screen_number - specifies the the screen to be used on that server
    //                 multiple screens can be controlled by a single X server 
    Display *display;
    // TODO: research Window struct 
    Window root_window; 
    // open with default display server (0)
    display = XOpenDisplay(0);
	root_window = XRootWindow(display, 0);
    
    struct cursor_position cp;
    // cp = getCursorPosition(); 
    // const char *device;
    
    // TODO make these constants 
    // These are just here to make the code more readible in the decision logic 
    __u8 x_button = 0;
    __u8 a_button = 1; 
    __u8 b_button = 2;
    __u8 y_button = 3; 
    __u8 left_trigger = 4; 
    __u8 right_trigger = 5;
    __u8 select_button = 8; 
    __u8 start_button = 9;
    
    __u8 is_pressed = 1; 
    __u8 is_released = 0; 

    // values of cursor new coordinates
    int new_x;
    int new_y;
    // how many pixels the mouse moves at one time 
    int move_interval = 10; // pixels

    // The delay the cursor movement 
    // 1000 * 1000 = 1 second
    useconds_t u = 1000 * 50;

    int is_running = 1; 
    while(is_running)
    {
        // read event queau
        while(read(fd, &e, sizeof(e)) > 0)
        {
            current_event = e;  
        }
        if(errno != EAGAIN)
        {
            printf("Read error occured."); 
        }

        // THE DESCISION TREE
        // TODO convert this to switch 
        if (current_event.type == JS_EVENT_BUTTON)
        {
            if (current_event.value == is_pressed)
            {
                if(current_event.number == x_button)
                {
                    // printf("X button was pressed\n");
                    putchar('0');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == y_button)
                {
                    // printf("Y button was pressed\n");
                    putchar('3');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == a_button)
                {
                    // printf("A button was pressed\n");
                    putchar('1');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == b_button)
                {
                    // printf("B button was pressed\n");
                    putchar('2');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == right_trigger)
                {
                    // printf("R button was pressed\n");
                    putchar('5');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == left_trigger)
                {
                    // printf("LT button was pressed\n");
                    putchar('4');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                if(current_event.number == select_button)
                {
                    // printf("Select button was pressed\n");
                    putchar('8');
                    // putchar('\n');
                    // if(current_event.value == is_released)
                    // {
                    //     break; 
                    // }
                }
                else if(current_event.number == start_button)
                {
                    is_running = 0; 
                }
            }
        } 
        else if(current_event.type == JS_EVENT_AXIS)
        {
            switch(current_event.number)
                {
                    // x joystick axis  
                    case(0):
                    {
                        cp = getCursorPosition(display, root_window);
                        // usleep(u); 
                        // printf("Position before manipulating x/y: %d, %d\n", cp.x, cp.y); 
                        if(current_event.value > 0)
                        {                             
                            // printf("x before=%d\n", cp.x);
                            cp.x += move_interval; 
                            // printf("x after=%d\n", cp.x);
                            moveMouse(cp.x, cp.y, display, root_window);
                            // printf("right axis positive triggered\n");
                        
                            usleep(u);
                            
                            if(current_event.value == is_released){
                                // Am I ever even entering this condition?
                                printf("Breaking from x axis\n"); 
                                break; 
                            }
                        }
                        // left
                        else if(current_event.value < 0)
                        {
                            // printf("x before=%d\n", cp.x);
                            cp.x -= move_interval; 
                            // printf("x after=%d\n", cp.x);
                            moveMouse(cp.x, cp.y, display, root_window);
                            // printf("right axis positive triggered\n");
                        
                            usleep(u);
                            
                            if(current_event.value == is_released){
                                // Am I ever even entering this condition?
                                printf("Breaking from x axis\n"); 
                                break; 
                            }
                        }
                    }
                    // y joystick axis
                    case(1):
                    {
                        // down
                        if(current_event.value > 0 && current_event.number == 1)
                        {
                            // printf("x before=%d\n", cp.x);
                            cp.y += move_interval; 
                            // printf("x after=%d\n", cp.x);
                            moveMouse(cp.x, cp.y, display, root_window);
                            // printf("right axis positive triggered\n");
                        
                            usleep(u);
                            
                            if(current_event.value == is_released){
                                // Am I ever even entering this condition?
                                printf("Breaking from x axis\n"); 
                                break; 
                            }
                        }
                        // up
                        else if(current_event.value < 0 && current_event.number == 1)
                        {
                            // printf("x before=%d\n", cp.x);
                            cp.y -= move_interval; 
                            // printf("x after=%d\n", cp.x);
                            moveMouse(cp.x, cp.y, display, root_window);
                            // printf("right axis positive triggered\n");
                        
                            usleep(u);
                            
                            if(current_event.value == is_released){
                                // Am I ever even entering this condition?
                                printf("Breaking from x axis\n"); 
                                break; 
                            }
                        }  
                    }
                }
        }
    }
    close(fd); 
    return 0;
}