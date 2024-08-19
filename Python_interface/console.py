from typing import List
from tkinter import *
from arduino_interface import ArduinoInterface

class Console(Frame):
    """Class representing the console for comunication with the contromer."""

    def __init__(self,
                 parent: Misc,
                 arduino_int: ArduinoInterface) -> "Console":
        """The constructor for the Console object."""
        super().__init__(parent)
        self.arduino_interface: ArduinoInterface = arduino_int
        """The arduino interface object used to connect to the arduino."""
        self.console_output_text_box = Text(self)
        """The textbox object in which the output text is shown."""
        self.console_output_text_box.pack(fill="y")
        self.console_output_text: str = ""
        """The string shown in the console output."""
        self.console_input_text_box = Entry(self, textvariable=StringVar(value=str(self.console_output_text)))
        """The textbox object in which the input text is entered."""
        self.console_input_text_box.bind("<Return>", self.enter_key_press)
        self.console_input_text_box.configure(state="disabled")
        self.console_input_text_box.pack(fill="y")
        self.console_history: List[str] = []
        """The history of all the command sent through the console"""

    def enter_key_press(self):
        """Function called when the enter key is pressed."""
        self.arduino_interface.send_message(self.console_input_text_box.get())
        self.console_input_text_box.delete(0,END)

    def update_console(self, mesage: str) -> None:
        """Updates the console window."""
        self.console_output_text_box.insert(INSERT, f"{mesage}\n")