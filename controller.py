class Button:
	def __init__(self, key, name):
		self.key = key
		self.name = name


class Controller:
	# Buttons A and B are mapped to mouse clicks 
	start 		  = Button('1', 'start')
	y 			  = Button('2', 'y')
	select 		  = Button('3', 'select')
	x 			  = Button('4', 'x')
	right_trigger = Button('5', 'right trigger')
	left_trigger  = Button('6', 'left trigger')

	def display_controls(self):
		print(self.start.name + ' : ' + self.start.key)

# NES = Controller()
# NES.display_controls()










