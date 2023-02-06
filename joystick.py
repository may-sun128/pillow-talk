import logging
import evdev
from evdev import InputDevice, categorize, ecodes
import pyautogui
import time
import json
import os


class JoyStick:
	def __init__(self):
		self.devices = [InputDevice(path) for path in evdev.list_devices()]
		for device in self.devices:
			if device.name == 'N/A':
				break
		# Attach to the joystick device
		# TODO change this name; having a field that same as its class name is confusing 
		self.joystick = InputDevice(device.path)
	
	# TODO figure out if I need self here or not?
	def a_button_pressed():
		pass
	def b_button_pressed():
		pass
	def x_button_pressed():
		pass
	def y_button_pressed():
		pass
	def start_button_pressed():
		pass
	def select_button_pressed():
		pass
	def right_trigger_pressed():
		pass
	def left_trigger_pressed():
		pass

	def right_dpad_pressed():
		pass
	def left_dpad_pressed():
		pass
	def up_dpad_pressed():
		pass
	def down_dpad_pressed():
		pass

	def process_events(self):
		for event in self.joystick.read_loop():
			# button or axis
			match event.type:
				# button 
				case 1:
					# pressed or released
					match event.value:
						# pressed
						case 1:
							match event.code:
								case 305:
									self.a_button_pressed()
								case 306:
									self.b_button_pressed()
								case 304:
									self.x_button_pressed()
								case 307:
									self.y_button_pressed()
								case 312:
									self.select_button_pressed()
								case 313:
									self.start_button_pressed()
								case 308:
									self.left_trigger_pressed()
								case 309:
									self.right_trigger_pressed()
						# released
						case 0:
							match event.code: 
								case 305:
									# print('A Button was released')
									pass
								# &c 
				# axis 
				case 3:
					# up/down or left/right
					match event.code:
						# up/down
						case 1:
							# up or down
							match event.value:
								# up 
								case 0:
									self.up_dpad_pressed()
								# down
								case 255:
									self.down_dpad_pressed()
						# left/right
						case 0:
							match event.value:
								# left 
								case 0:
									self.left_dpad_pressed()
								# right
								case 255:
									self.right_dpad_pressed()

	'''
		PROBLEM: For the mouse feature to work, 
		ie moving the mouse continuously while a button is pressed, 
		we need to break out of a while loop that loops on the 
		condition that the button is pressed.

		for event in events
			while event.button_is_pressed()
				move_mouse()
				if event.button_is_released()
					exit while loop 
		
		The trouble is that the event never changes while inside the while loop,
		thus the exit condition is never met. 

		This has been a real head-scratcher for me that chat-gtp, stackoverflow, 
		and the issues github page for evdev have provided no remedy. 

		POSSIBLE SOLUTION: use the get_event() method to return a single event, 
		then in the lazily-named while_event() function, loop on the condition that 
		the returned event is pressed, then call the get_event method *again*, and
		setting it to the looping event. Hopefully, by calling the function again, 
		the event value will finally change and the while loop can be exited.  
	'''

	def get_event(self):
		for event in self.joystick.read_loop():
			return event

	def while_event(self):
		e = self.get_event()
		# self.__print_event_info(e)
		while True:
			print(e.value)
			m = Mouse()
			m.move_up()
			new_event = self.get_event()
			if new_event.value != 589827:
				break

	def __print_events(self):
		for event in joystick.read_loop():
			print((categorize(event)))
	
	def __print_event_info(self, input_event):
		print(f'Type: {input_event.type}')
		print(f'Code: {input_event.code}')
		print(f'Value: {input_event.value}')
		print('\n')


	def load_config(self):
		data: str = ''
		with open('config.json', 'r') as f:
			data = f.read()
		button_dict = json.loads(data)
		for key in button_dict:
			print(key)	
			v = button_dict[key]['value']
			e = button_dict[key]['event']
			t = button_dict[key]['type'] 
			print(v, e, t)
			# if A button 
			if key == 'a button':
					if t == 'Bash':
						self.a_button_pressed = lambda: os.system(v)


# NOTE: these calls are for testing, but NB that they must be commented out for ui to run properly 
# js = JoyStick()
# js.load_config()
# js.process_events()

