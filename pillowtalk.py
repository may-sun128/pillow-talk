#!/usr/bin/python

from pynput import keyboard
import pyautogui 
import os
import subprocess
import time
import multiprocessing
import shlex

import controller
import player
import joystick


### File IO ### 

def get_keyboard_state():
    f = open('keyboard-state.txt', 'r')
    keyboard_state = f.read()
    f.close()
    return keyboard_state.strip()

def change_keyboard_state(new_state):
    f = open('keyboard-state.txt', 'w')
    f.write(new_state)
    f.close()

def get_mode():
    f = open('mode.txt', 'r')
    current_mode = f.read()
    f.close()
    return current_mode.strip()

def change_mode(new_mode):
    f = open('mode.txt', 'w')
    f.write(new_mode)
    f.close()

### Bash ###

def execute_bash(command: str):
    obj = subprocess.Popen(f"{command}", stdout=subprocess.PIPE, shell=True)
    obj_bin, err = obj.communicate()

    # Not sure what the point of this is
    status = obj.wait()

    val = obj_bin.decode('ascii') 
    return val   

### Virtual Keyboard MGMT ###

def start_virtual_keyboard():
    # start florence (virtual keyboard) and echo its process ID
    start_virtual_keyboard_command = 'florence'

    pid = execute_bash(start_virtual_keyboard_command)

# This function calls the start_virtual_keyboard function, moves it to a seperate thread,
# kills the thread, and finally kills the process of the virtual keyboard 
def handle_virtual_keyboard():
    process = multiprocessing.Process(target=start_virtual_keyboard, args=())
    process.start()
    # Keep the virtual keyboard open for 15 seconds 
    # (hopefully enough to finish typing what you're typing)
    # Ideally, the keyboard state would not be managed by time, but by user input
    # But for now, if you don't want to see the keyboard anymore, hide it with florence's functionality 
    time.sleep(20)

    # counter = timer.Counter()
    # counter.start()
    # counter.wait()

    process.terminate()
    execute_bash('pkill -f florence')
    print('florence process killed') 

### Variables & Objects ###

volume = int(execute_bash('awk -F"[][]" \'/Left:/ { print $2 }\' <(amixer sget Master)').replace('%', '').strip()) 

### User-Defined Functions ### 

def x_pressed():
    # print('x button clicked')
    mode = get_mode()
    if mode == 'desktop':
        pyautogui.hotkey('winleft', 'w')
    elif mode == 'firefox':
        pyautogui.hotkey('ctrl', 'w')
        print('crtl+w was pressed?')

def b_pressed():
    # print('b button clicked')
    pyautogui.click()
    # print('pyauthogui clicked')

def y_pressed():
    # print('y button clicked')
    handle_virtual_keyboard()

def a_pressed():
    # print('a button clicked')
    pyautogui.click(button='right')

def start_pressed():
    pyautogui.press('backspace')

    mode = get_mode()
    
    if mode == 'firefox':
        change_mode('desktop')
        player.play_alert('gui/piano-notes/C5_pp.wav')
        print('**** MODE = DESKTOP ****')
        # playsound('./piano-notes/C4_pp.wav')
    elif mode == 'desktop':
        change_mode('general')
        player.play_alert('gui/piano-notes/C4_pp.wav')
        print('**** MODE = GENERAL ****')
    elif mode == 'general':
        change_mode('firefox')
        player.play_alert('gui/piano-notes/G4_pp.wav')
        print('**** MODE = FIREFOX ****')

def select_pressed():
    mode = get_mode()
    if mode == 'desktop':
        pyautogui.hotkey('winleft', 'r')
    elif mode == 'firefox':
        pyautogui.keyDown('ctrl')
        pyautogui.click()
        pyautogui.keyUp('ctrl')
    elif mode == 'general':
        execute_bash('amixer set Master toggle')

def rt_pressed():
    mode = get_mode()
    if mode == 'firefox':
        pyautogui.press('down')
    elif mode == 'desktop':
        pyautogui.hotkey('ctrl', '+')
    elif mode == 'general':
        global volume
        volume += 5 
        print(volume)
        execute_bash(f'amixer set Master {str(volume)}%')
def lt_pressed():
    mode = get_mode()
    if mode == 'firefox':
        pyautogui.press('up')
    elif mode == 'desktop':
        pyautogui.hotkey('ctrl', '-')
    elif mode == 'general':
        # global volume
        volume -= 5 
        print(volume)
        execute_bash(f'amixer set Master {str(volume)}%')

### STDOUT ### 

def event_equals_previous(event_value):
    l = [-1, -1]
    if l[0] == -1:
        l[0] = event_value
    elif l[0] != -1:
        l[1] = l[0]
        l[0] = event_value
        
    print(f'l[0] = {l[0]}')
    print(f'l[1] = {l[1]}')
    print('-----------------------')


def read_stdout(joystick):
    command = 'myjoypad/./a.out'
    process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
    init = 10
    while True:
        output = process.stdout.readline()
        # print(output.decode().strip())
        # if output is different from the previous output
        if output.decode().strip() == init:
            if output == '' and process.poll() is not None:
                break
            # X Button
            elif output.decode().strip() == '0':
                joystick.x_button_pressed()
            # A Button
            elif output.decode().strip() == '1':
                joystick.a_button_pressed()
            # B Button
            elif output.decode().strip() == '2':
                joystick.b_button_pressed()
            # Y Button
            elif output.decode().strip() == '3':
                joystick.y_button_pressed()
            # Left Trigger
            elif output.decode().strip() == '4':
                joystick.left_trigger_pressed()
            # Right Trigger
            elif output.decode().strip() == '5':
                joystick.right_trigger_pressed()
            # Select Button
            elif output.decode().strip() == '8':
                joystick.select_button_pressed()
            # Start Button
            elif output.decode().strip() == '9':
                joystick.start_button_pressed()
            else:
                print(output.decode().strip())
                break
        else:
            print('did not enter for loop')
    rc = process.poll()
    return rc

def main():
    # instanciate joystick object 
    js = joystick.JoyStick()

    # assign joystick events to custom functions 
    js.a_button_pressed = a_pressed
    js.b_button_pressed = b_pressed
    js.x_button_pressed = x_pressed
    js.y_button_pressed = y_pressed
    js.start_button_pressed = start_pressed
    js.select_button_pressed = select_pressed
    js.right_trigger_pressed = rt_pressed
    js.left_trigger_pressed = lt_pressed
    
    # read stdin/out from myjoypad.c 
    read_stdout(js)

    # for i in range(10):
    #     event_equals_previous(i)


main()
