from nicegui import ui
import joystick
import os

# TODO figure out how to import joystick module 


js = joystick.JoyStick()

def get_joystick(result):
    js.a_button_pressed = lambda: os.system(result)

ui.markdown('## Bind Buttons')
ui.label('A Button')
a_button_select = ui.select(['Bash', 'Script', 'HotKey', 'Mouse Action'], value="Initial")
ui.input(label='Text', placeholder='Enter bash code...')
result = ui.label()
print(f'ui label result = {ui.label}')
ui.button('Go', on_click=lambda: get_joystick(result))

ui.run()

# js.process_events()

'''
    GUI LAYOUT

    JoyStick Buttons 
        drop down for hotkey, inline bash, or path to script 
            re path to script, see if nicegui has an option for opening files 
        

'''
