#!/usr/bin/python

from pynput import keyboard
import pyautogui 
import os
import subprocess
import time
import multiprocessing



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
    start_virtual_keyboard_command = 'florence; echo $!'

    pid = execute_bash(start_virtual_keyboard_command)


# This function calls the start_virtual_keyboard function, moves it to a seperate thread,
# kills the thread, and finally kills the process of the virtual keyboard 
def handle_virtual_keyboard():
    process = multiprocessing.Process(target=start_virtual_keyboard, args=())
    process.start()
    # Keep the virtual keyboard open for 15 seconds 
    # (hopefully enough to finish typing what you're typing)
    # Ideally, the keyboard state would not be managed by time, but by user input 
    time.sleep(15)
    process.terminate()
    execute_bash('pkill -f florence')
    print('florence process killed')

### Controller Keys Translator ###

def on_press(key):  
    try: 
        # Stop listener
        # Escape key not assinged to key on physical controller 
        # Just for debugging/development
        if key == keyboard.Key.esc:
            return False  
        
        # Cycle through modes
        elif key.char == 's':
            pyautogui.press('backspace')

            mode = get_mode()
            
            if mode == 'firefox':
                change_mode('desktop')
                print('**** MODE = DESKTOP ****')
            elif mode == 'desktop':
                change_mode('firefox')
                print('**** MODE = FIREFOX ****')

        # open virtual keyboard
        elif key.char == 't':
            pyautogui.press('backspace')
            handle_virtual_keyboard()

        # if in desktop mode, open rofi 
        # if in firefox, click to open in new tab  
        elif key.char == 'k':
            pyautogui.press('backspace')
            
            mode = get_mode()
            if mode == 'desktop':
                pyautogui.hotkey('winleft', 'r')
            elif mode == 'firefox':
                pyautogui.keyDown('ctrl')
                pyautogui.click()
                pyautogui.keyUp('ctrl')
        
        # q as in quit -- close the cuqrrent node
        elif key.char == 'q':
            pyautogui.press('backspace')

            pyautogui.hotkey('winleft', 'w')
        
        # if in desktop mode, arrow down 
        # if in firefox, zoom 
        elif key.char == 'p':
            pyautogui.press('backspace')

            mode = get_mode()
            if mode == 'firefox':
                pyautogui.hotkey('ctrl', '+')
            elif mode == 'desktop':
                pyautogui.press('down')

        # if in desktop mode, arrow up 
        # if in firefox, zoom
        elif key.char == 'm':
            pyautogui.press('backspace')

            mode = get_mode()
            if mode == 'firefox':
                pyautogui.hotkey('ctrl', '-')
            elif mode == 'desktop':
                pyautogui.press('up')

        else:
            print(key)
    except:
        #print()
        pass

listener = keyboard.Listener(on_press=on_press)
listener.start()  # start to listen on a separate thread
listener.join()  # remove if main thread is polling self.keys


