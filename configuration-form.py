from nicegui import ui
import joystick
import os
import json 
import flatten_dict


# TODO figure out how to import joystick module from another directory 

### Callback Functions ###

# write button dictionary to ./config.json
def write_config(config: dict):
    json_config = json.dumps(config, indent = 4)
    with open('config.json', 'w') as f:
        f.write(json_config)

### Dictionary Objects ###

button_dict = {
        'a-button': {
            'type': '', 
            'value': '',
            'event': ''
        }
    }

# flatten dictionary so it's easier to access 
flat_dict = flatten_dict.flatten(button_dict, reducer='underscore')
# print(flat_dict)

# hard code event 
flat_dict.update({'a-button_event': 'pressed'})

### Form ###  

# non-dpad buttons 
ui.markdown('## Bind Buttons')

## A Button ##
with ui.column():
    ui.markdown('**A Button**')
    # get type (bash, script, hotkey, mouse action)
    a_btn_drp_dwn = ui.select(['Bash', 'Script', 'Hot Key', 'Mouse Action'], value='Bash', on_change=lambda: flat_dict.update({'a-button_type': a_button_drp_dwn.value}))
    with ui.row():
        a_btn_bash_txt_bx = ui.input(label='Bash', placeholder='echo "hello world"', on_change=lambda: flat_dict.update({'a-button_value': a_btn_type_txt_bx.value}))
        a_btn_hotkey_txt_bx = ui.input(label='Hot Key', placeholder='ctrl r', on_change=lambda: flat_dict.update({'a-button_value': a_button_txt_bx.value}))
        a_btn_script_upload = ui.upload(label='Upload Script', on_upload=lambda e: ui.notify(f'Uploaded {e.name}')).classes('max-w-full')

## B Button ## 

    ui.markdown('**B Button**')
    # get type (bash, script, hotkey, mouse action)
    a_btn_drp_dwn = ui.select(['Bash', 'Script', 'Hot Key', 'Mouse Action'], value='Bash', on_change=lambda: a_btn_drp_dwn.update())
    with ui.row():
        a_btn_bash_txt_bx = ui.input(label='Bash', placeholder='echo "hello world"', on_change=lambda: flat_dict.update({'b-button_value': a_btn_type_txt_bx.value}))
        a_btn_hotkey_txt_bx = ui.input(label='Hot Key', placeholder='ctrl r', on_change=lambda: flat_dict.update({'b-button_value': a_button_txt_bx.value}))
        a_btn_script_upload = ui.upload(label='Upload Script', on_upload=lambda e: ui.notify(f'Uploaded {e.name}')).classes('max-w-full')



# write dict to file 
ui.button('Save', on_click=lambda: write_config(flatten_dict.unflatten(flat_dict, splitter='underscore')))

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
