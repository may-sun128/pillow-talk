from nicegui import ui
import joystick
import os
import json 

# TODO figure out how to import joystick module from another directory 


# write button dictionary to ./config.json
def write_config(config: dict):
    json_config = json.dumps(config, indent = 4)
    with open('config.json', 'w') as f:
        f.write(json_config)

# non-dpad buttons 
ui.markdown('## Bind Buttons')

# A Button 

button_dict = {
        'a button': {
            'type': '', 
            'value': '',
            'event': ''
        }
    }

# hard code event 
button_dict.update({'event': 'pressed'})
# label
ui.label('A Button')
# get type (bash, script, hotkey, mouse action)
a_button_drp_dwn = ui.select(['Bash', 'Script', 'HotKey', 'Mouse Action'], value='Initial', on_change=lambda: button_dict.update({'type': a_button_drp_dwn.value}))
# get value ('echo hello world', '~/scripts/hello-world.sh', hotkey, &c)
a_button_txt_bx = ui.input(label='Text', placeholder='Enter bash code...', on_change=lambda: button_dict.update({'value': a_button_txt_bx.value}))
# write dict to file 
ui.button('Save', on_click=lambda: write_config(button_dict))
# start 
ui.run(dark=True, show=False)






'''
    GUI LAYOUT

    JoyStick Buttons 
        drop down for hotkey, inline bash, or path to script 
            re path to script, see if nicegui has an option for opening files 
        

    d: dict = {
        'type': 'bash' or 'script' or 'hotkey' or 'mouse action'
        'value': 
    }

    inline_bash = {
        'a button': {
            'type': 'bash', 
            'value': 'echo hello world',
            'button': 'a', 
            'event': 'pressed'
        }
    }

'''
