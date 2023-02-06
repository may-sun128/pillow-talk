import pyautogui

class Mouse:
	def __init__(self):
		self.speed = 0.001

	def right_click(self):
		pyautogui.click(button='right')
	
	def left_click(self):
		pyautogui.click(button='left')

	def move_up(self):
		pyautogui.moveRel(0, -50, 0.01)
		# for i in range(10):
		# 	pyautogui.dragRel(1, 0)
		# 	time.sleep(self.speed)
	def move_down(self):
		pyautogui.move(0, 50, 0.1)

	def move_right(self):
		pyautogui.moveRel(50, 0)

	def move_left(self):
		pyautogui.moveRel(-50, 0)
