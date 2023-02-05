from pynput import keyboard
import pyautogui 
import os
import subprocess
import time
import multiprocessing
import shlex

# import controller
import player
import joystick


# bash 
def execute_bash(command: str):
    obj = subprocess.Popen(f"{command}", stdout=subprocess.PIPE, shell=True)
    obj_bin, err = obj.communicate()

    status = obj.wait()

    val = obj_bin.decode('ascii') 
    return val

# functions to be mapped
def volume_up():
    volume = int(execute_bash('awk -F"[][]" \'/Left:/ { print $2 }\' <(amixer sget Master)').replace('%', '').strip())
    execute_bash(f'amixer set Master {str(volume + 5)}%') 

def volume_down():
    volume = int(execute_bash('awk -F"[][]" \'/Left:/ { print $2 }\' <(amixer sget Master)').replace('%', '').strip())
    execute_bash(f'amixer set Master {str(volume - 5)}%')

def launch_gui():
    print('GUI launched.')
    # TODO implement this 

def hello_world():
    print('Hello World')

# create joystick object 
js = joystick.JoyStick()

# create mouse object 
m = joystick.Mouse()

# map user defined functions to joystick object 
js.x_button_pressed = lambda: os.system('echo bash was executed')
js.y_button_pressed = lambda: os.system('echo bash was executed')
js.right_trigger_pressed = volume_up #
js.left_trigger_pressed = volume_down # 
js.start_button_pressed = launch_gui # 
js.select_button_pressed = lambda: pyautogui.hotkey('win', 'r') #
js.a_button_pressed = m.right_click #
js.b_button_pressed = m.left_click # 

# dpad events
js.up_dpad_pressed = m.move_up #
js.down_dpad_pressed = m.move_down # 
js.right_dpad_pressed = m.move_right # 
js.left_dpad_pressed = m.move_left # 

js.process_events()
