#!/usr/bin/python

from pynput import keyboard
import pyautogui 
import os
import subprocess
import time
import multiprocessing

import timer 


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

### Controller Keys Translator ###

def close():
    execute_bash('pkill -f florence')

def on_press(key):  
    try: 
        # Stop listener
        # Escape key not assinged to key on physical controller 
        # Just for debugging/development
        if key == keyboard.Key.esc:
            close()
            return False  
        
        # Start Button
        # Cycle through modes
        elif key.char == '1':
            pyautogui.press('backspace')

            mode = get_mode()
            
            if mode == 'firefox':
                change_mode('desktop')
                print('**** MODE = DESKTOP ****')
            elif mode == 'desktop':
                change_mode('firefox')
                print('**** MODE = FIREFOX ****')

        # Y Button 
        # Open virtual keyboard
        elif key.char == '2':
            pyautogui.press('backspace')
            handle_virtual_keyboard()

        # Select Button
        # if in desktop mode, open rofi 
        # if in firefox, click to open in new tab  
        elif key.char == '3':
            pyautogui.press('backspace')
            
            mode = get_mode()
            if mode == 'desktop':
                pyautogui.hotkey('winleft', 'r')
            elif mode == 'firefox':
                pyautogui.keyDown('ctrl')
                pyautogui.click()
                pyautogui.keyUp('ctrl')
        
        # X Button 
        # q as in quit -- close the current mode
        elif key.char == '4':
            pyautogui.press('backspace')
            mode = get_mode()
            if mode == 'desktop':
                pyautogui.hotkey('winleft', 'w')
            elif mode == 'firefox':
                pyautogui.hotkey('ctrl', 'w')
                print('crtl+w was pressed?')
        
        # Right Trigger
        # if in desktop mode, arrow down 
        # if in firefox, zoom 
        elif key.char == '5':
            pyautogui.press('backspace')

            mode = get_mode()
            if mode == 'firefox':
                pyautogui.hotkey('ctrl', '+')
            elif mode == 'desktop':
                pyautogui.press('down')

        # Left Trigger 
        # if in desktop mode, arrow up 
        # if in firefox, zoom
        elif key.char == '6':
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

def main():
    
    listener = keyboard.Listener(on_press=on_press)
    listener.start()  # start to listen on a separate thread
    listener.join()  # remove if main thread is polling self.keys

main()
