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

def get_example_joystick():
    # create joystick object 
    example = joystick.JoyStick()

    # create mouse object 
    m = joystick.Mouse()

    # map user defined functions to joystick object 
    example.x_button_pressed = lambda: os.system('echo bash was executed')
    example.y_button_pressed = lambda: os.system('echo bash was executed')
    example.right_trigger_pressed = volume_up #
    example.left_trigger_pressed = volume_down # 
    example.start_button_pressed = launch_gui # 
    example.select_button_pressed = lambda: pyautogui.hotkey('win', 'r') #
    example.a_button_pressed = m.right_click #
    example.b_button_pressed = m.left_click # 

    # dpad events
    example.up_dpad_pressed = m.move_up #
    example.down_dpad_pressed = m.move_down # 
    example.right_dpad_pressed = m.move_right # 
    example.left_dpad_pressed = m.move_left # 

    example.process_events()
