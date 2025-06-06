from tcp_server import ThreadedTCPServer
from threading import Thread
from time import time, sleep
from tkinter import Event
from main_window import MainWindow
from controller_interface import ControllerInterface


class App():

    NB_ARRAYS = 8
    """The number of arrays conected to the controller"""

    def __init__(self) -> "App":
        self.controller_interface = ControllerInterface(baudrate=115200)
        self.server: ThreadedTCPServer = ThreadedTCPServer(self)
        self.main_window = MainWindow(self, (2500,1300), self.NB_ARRAYS)

    def update_console(self):
        """This is the function called by the thread that reads the serial buffer from the controller."""
        while True:
            #self.main_window.console.update_console()
            self.main_window.side_bar_menu.console.update_console()
            sleep(0.1)

if __name__ == '__main__':
    app = App()
    thread_reading_serial = Thread(target = app.update_console)
    thread_reading_serial.daemon = True
    try:
        thread_reading_serial.start()
    except RuntimeError as e:
        print("An error occurred on main serial interface read thread startup!")
    app.main_window.mainloop()
