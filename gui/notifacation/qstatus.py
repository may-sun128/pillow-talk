import datetime
import sys

from PyQt5 import Qt
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QPushButton, QDesktopWidget, QVBoxLayout, QApplication


# To simplify the task it is better to create a Widget for the messages as I show
# below:

class Message(QWidget):
    def __init__(self, title, message, parent=None):
        QWidget.__init__(self, parent)
        self.setLayout(QGridLayout())
        self.titleLabel = QLabel(title, self)
        self.titleLabel.setStyleSheet(
            "font-family: 'Roboto', sans-serif; font-size: 14px; font-weight: bold; padding: 0;")
        self.messageLabel = QLabel(message, self)
        self.messageLabel.setStyleSheet(
            "font-family: 'Roboto', sans-serif; font-size: 12px; font-weight: normal; padding: 0;")
        self.buttonClose = QPushButton(self)
        self.buttonClose.setIcon(QIcon("res/close1.png"))
        self.buttonClose.setFixedSize(14, 14)
        self.layout().addWidget(self.titleLabel, 0, 0)
        self.layout().addWidget(self.messageLabel, 1, 0)
        self.layout().addWidget(self.buttonClose, 0, 1, 2, 1)

# When you want to remove the message first you must remove it from the layout
# with the removeWidget() function of the layout and then you must use
# deleteLater() to delete itself and its child widget. when we send a signal we
# can get the object that emits it through sender(), in our case it will be the
# button, and through this we obtain the parent that is our message widget. Then
# to update the correct size of the layout we use adjustSize(). Even removing the
# entire widget takes a minimum value so it is advisable to close it if there are
# no messages.

class Notification(QWidget):
    signNotifyClose = pyqtSignal(str)
    def __init__(self, parent = None):
        time = datetime.datetime.now()
        currentTime = str(time.hour) + ":" + str(time.minute) + "_"
        self.LOG_TAG = currentTime + self.__class__.__name__ + ": "
        super(QWidget, self).__init__(parent)

        # self.setWindowFlags(Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint)
        resolution = QDesktopWidget().screenGeometry(-1)
        screenWidth = resolution.width()
        screenHeight = resolution.height()
        print(self.LOG_TAG + "width: " + str(resolution.width()) + " height: " + str(resolution.height()))
        self.nMessages = 0
        self.mainLayout = QVBoxLayout(self)
        self.move(screenWidth, 0)

    def setNotify(self, title, message):
        m = Message(title, message, self)
        self.mainLayout.addWidget(m)
        m.buttonClose.clicked.connect(self.onClicked)
        self.nMessages += 1
        self.show()

    def onClicked(self):
        self.mainLayout.removeWidget(self.sender().parent())
        self.sender().parent().deleteLater()
        self.nMessages -= 1
        self.adjustSize()
        if self.nMessages == 0:
            self.close()

# Here is an example of how to use it with the following code:

class Example(QWidget):
    counter = 0

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.setLayout(QVBoxLayout())
        btn = QPushButton("Send Notify", self)
        self.layout().addWidget(btn)

        self.notification = Notification()
        btn.clicked.connect(self.notify)


    def notify(self):
        self.counter += 1
        self.notification.setNotify("Title{}".format(self.counter),
                                    "message{}".format(self.counter))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = Example()
    w.show()
    sys.exit(app.exec_())