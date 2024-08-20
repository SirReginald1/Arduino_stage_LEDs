from threading import Thread
from time import time
from tkinter import Event
from main_window import MainWindow


class App():

    def __init__(self) -> "App":
        self.main_window = MainWindow((2500,1300), 4)
        self.main_window.bind("<r>", self.resend_last_message)

    def update_console(self):
        """This is the function called by the thread that reads the serial buffer from the controller."""
        while True:
            #self.main_window.console.update_console()
            self.main_window.side_bar_menu.console.update_console()

    def resend_last_message(self, event: Event):
        """Resends the last message sent by the controller interface to the controller."""
        self.main_window.controller_interface.send_message(self.main_window.controller_interface.last_message_sent)

if __name__ == '__main__':
    app = App()
    thread_reading_serial = Thread(target = app.update_console)
    thread_reading_serial.daemon = True
    thread_reading_serial.start()
    app.main_window.mainloop()