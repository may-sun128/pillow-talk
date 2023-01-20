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

static int _XlibErrorHandler(Display *display, XErrorEvent *event)
{
	fprintf(stderr, "An error occured detecting the mouse position\n");
    return True;
}

struct cursor_position
{
    int x, y; 
};

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
    // fprintf(stderr, "There are %d screens available in this X session\n", number_of_screens);
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
 
void moveMouse(int x, int y, Display *dpy, Window root_window)
{
	// Request that the xserver report events associated with the specific event mask 
    // Initially, X will not report any of these events. ; 
    XSelectInput(dpy, root_window, KeyReleaseMask);
	// Move the cursor 
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
	// flush the output buffer 
    //      not sure if this event should happen here or in main 
    XFlush(dpy);
}

// int checkEvent(struct js_event e)
// {
//     printf("CheckEvent event value: %d\n", e.value); 
//     if(e.value == 0)
//     {
//         return 0; 
//     }
//     return -1; 
// }

void printEventInfo(struct js_event e)
{
    printf("Time:%d\nValue:%d\nType:%d\nNumber:%d\n----------------------------------\n", e.time, e.value, e.type, e.number);
}

int _main(int argc, char *argv[])
{
    // Originally, these types were local to moveMouse()
    Display *display;
    Window root_window; 
    display = XOpenDisplay(0);
	root_window = XRootWindow(display, 0);
    
    struct cursor_position cp; 
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

    // original
    // js = open(device, O_RDONLY);

    // non-blocking
    js = open(device, O_NONBLOCK); 

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

    int is_running = 0; 

    int new_x;
    int new_y;
    int move_interval = 20;

    /* This loop will exit if the controller is unplugged. */
    // Original 
    while (read_event(js, &event) == 0 && is_running == 0)
    {
        // buttons or axis 
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
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
                        // exit program (to avoid memory leak)
                        is_running = 1;
                    }
                }

            case JS_EVENT_AXIS:
                cp = getCursorPosition();
                // printf("X:%d\nY:%d\n", cp.x, cp.y);
                printf("Time:%d\nValue:%d\nType:%d\nNumber:%d\n----------------------------------\n", event.time, event.value, event.type, event.number);
                
                // x or y axis
                switch(event.number)
                {
                    // x joystick axis 
                    case(0):
                    {
                        // right
                        if(event.value > 0)
                        { 
                            // printf("Time:%d\nValue:%d\nType:%d\nNumber:%d\n----------------------------------\n", event.time, event.value, event.type, event.number);
                            // printf("%d", event.value);
                            // useconds_t u = 1000 * 4000;
                            
                            cp.x += move_interval;
                            moveMouse(cp.x, cp.y, display, root_window);
                            // usleep(u);
                        }
                        // left
                        else if(event.value < 0)
                        {
                            cp.x -= move_interval;
                            moveMouse(cp.x, cp.y, display, root_window); 
                        }
                    }
                    // y joystick axis
                    case(1):
                    {
                        // down
                        if(event.value > 0)
                        {
                            cp.y += move_interval;
                            moveMouse(cp.x, cp.y, display, root_window);
                        }
                        // up
                        else if(event.value < 0)
                        {
                            cp.y -= move_interval;
                            moveMouse(cp.x, cp.y, display, root_window);
                        }
                       
                    }
                }
            default:
                printf("Default Case Event Value: %d\n------------------------------------\n", event.value); 
                /* Ignore init events. */
                break;
        }
        
        fflush(stdout);
    }
    
    printf("Exited while loop");
    close(js);
    return 0;
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
            // printf("event time:%d\n", e.time); 
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
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == y_button)
                {
                    // printf("Y button was pressed\n");
                    putchar('3');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == a_button)
                {
                    // printf("A button was pressed\n");
                    putchar('1');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == b_button)
                {
                    // printf("B button was pressed\n");
                    putchar('2');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == right_trigger)
                {
                    // printf("R button was pressed\n");
                    putchar('5');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == left_trigger)
                {
                    // printf("LT button was pressed\n");
                    putchar('4');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
                }
                if(current_event.number == select_button)
                {
                    // printf("Select button was pressed\n");
                    putchar('8');
                    putchar('\n');
                    if(current_event.value == is_released)
                    {
                        break; 
                    }
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
                        cp = getCursorPosition();
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