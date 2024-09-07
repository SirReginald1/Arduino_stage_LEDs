from threading import Thread
from time import time
from tkinter import Event
from main_window import MainWindow


class App():

    def __init__(self) -> "App":
        self.main_window = MainWindow((2500,1300), 4)

    def update_console(self):
        """This is the function called by the thread that reads the serial buffer from the controller."""
        while True:
            #self.main_window.console.update_console()
            self.main_window.side_bar_menu.console.update_console()

if __name__ == '__main__':
    app = App()
    thread_reading_serial = Thread(target = app.update_console)
    thread_reading_serial.daemon = True
    thread_reading_serial.start()
    app.main_window.mainloop()