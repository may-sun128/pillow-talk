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


class JoyStick:
    # def __init__(self):
    #     print('_JoyStick (Parent) has been instanciated')
    #     super().__init__()

    def a_button_press(self):
        print('button a is pressed')
        pass
    def b_button_press(self):
        pass
    def x_button_press(self):
        pass
    def y_button_press(self):
        pass
    def start_button_press(self):
        pass
    def select_button_press(self):
        pass
    def right_trigger_press(self):
        pass
    def left_trigger_press(self):
        pass



