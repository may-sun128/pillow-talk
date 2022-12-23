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

int main(int argc, char *argv[])
{
    const char *device;
    int js;
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

    //////////////////////////////////////////////////////////

    __u8 x_button = 0;
    __u8 a_button = 1; 
    __u8 b_button = 2;
    __u8 y_button = 3;  
    __u8 start_button = 9;

    //////////////////////////////////////////////////////////

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
                // Y listener key: 2
                else if(event.number == y_button)
                {
                    // printf("y button was pressed\n");
                    putchar('3');
                    putchar('\n');
                    // printf("2 character was put??");
                }
                else if(event.number == y_button)
                {
                    break; 
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