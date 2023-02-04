import logging
import evdev
from evdev import InputDevice, categorize, ecodes


class JoyStick:
	def __init__(self):
		self.devices = [InputDevice(path) for path in evdev.list_devices()]
		for device in self.devices:
			if device.name == 'N/A':
				break
		# Attach to the joystick device
		self.joystick = InputDevice(device.path)
	
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

	def process_events(self):
		for event in self.joystick.read_loop():
			match event.value:
				# if pressed
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
				# If released
				case 0:
					match event.code: 
						case 305:
							# print('A Button was released')
							pass
						# &c 

	def __print_events(self):
		for event in joystick.read_loop():
			print((categorize(event)))
	
	def __print_event_info(self, input_event):
		print(f'Type: {input_event.type}')
		print(f'Code: {input_event.code}')
		print(f'Value: {input_event.value}')
		print('\n')


js = JoyStick()

def hello_world():
	print('Hello World')

js.a_button_pressed = hello_world
js.b_button_pressed = hello_world
js.x_button_pressed = hello_world
js.y_button_pressed = hello_world
js.right_trigger_pressed = hello_world
js.left_trigger_pressed = hello_world
js.start_button_pressed = hello_world
js.select_button_pressed = hello_world

js.process_events()