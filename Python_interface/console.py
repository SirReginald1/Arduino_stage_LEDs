from typing import List
from tkinter import *
from controller_interface import ControllerInterface

class Console(Frame):
    """Class representing the console for comunication with the contromer."""

    def __init__(self,
                 parent: Misc,
                 controller_int: ControllerInterface) -> "Console":
        """The constructor for the Console object."""
        super().__init__(parent, 
                         borderwidth=1, 
                         highlightbackground="black", 
                         highlightthickness=2, 
                         #background="white",
                         relief="raised")
        self.controller_interface: ControllerInterface = controller_int
        """The arduino interface object used to connect to the arduino."""
        
        self.nb_rows: int = 2
        self.nb_cols: int = 1
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1, pad=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.console_output_text_box = Text(self, pady=500)
        """The textbox object in which the output text is shown."""
        #self.console_output_text_box.pack(fill="y")
        self.console_output_text_box.grid(row=0, column=0, pady=(2.5,5), padx=5)
        self.console_output_text_box.configure(state="normal")

        self.console_input_text_box = Entry(self, width=81)
        """The textbox object in which the input text is entered."""
        self.console_input_text_box.grid(row=1, column=0, pady=(5,2.5), padx=5)
        # Send message key command
        self.console_input_text_box.bind("<Return>", self.enter_key_press)
        # Go up in sent command history key command
        self.console_input_text_box.bind("<Up>", self.up_key_press)
        # Go up in sent command history key command
        self.console_input_text_box.bind("<Down>", self.down_key_press)
        #self.console_input_text_box.pack(fill="y")

        self.input_history: List[str] = []
        """The history of all the command sent through the console"""

        self.input_history_idx: int = -1
        """The index that is used to navigate through the console input history."""

        self.input_history_nb: int = 0
        """The total number of messages that that have been sent from this console"""

    def enter_key_press(self, event: Event):
        """Function called when the enter key is pressed and the console_input_text_box is selected."""
        message = self.console_input_text_box.get()
        if message != "":
            self.controller_interface.send_message(message)
            self.input_history.append(message) # Add to input history
            self.input_history_nb += 1
            self.console_input_text_box.delete(0,END)
        self.input_history_idx = self.input_history_nb # Increment current position in console history
        self.write_to_console(f"SENT> {message}")

    def up_key_press(self, event: Event):
        """Function that is called when the up arrow is pressed and the console_input_text_box is selected."""
        if self.input_history_idx > 0:
            self.input_history_idx -= 1
            #print(f"idx: {self.input_history_idx}, value: {self.input_history[self.input_history_idx]}")
            self.console_input_text_box.delete(0,END)
            self.console_input_text_box.insert(INSERT, self.input_history[self.input_history_idx])
    
    def down_key_press(self, event: Event):
        """Function that is called when the down arrow is pressed and the console_input_text_box is selected."""
        self.console_input_text_box.delete(0,END)
        if self.input_history_idx < self.input_history_nb-1:
            self.input_history_idx += 1
            #print(f"idx: {self.input_history_idx}, value: {self.input_history[self.input_history_idx]}")
            self.console_input_text_box.insert(INSERT, self.input_history[self.input_history_idx])

    def write_to_console(self, message: str) -> None:
        """Writes the provided message to the console."""
        self.console_output_text_box.configure(state="normal")
        self.console_output_text_box.insert(END, f"{message}\n")
        self.console_output_text_box.configure(state="disabled")

    def update_console(self) -> None:
        """Updates the console window."""
        message = self.controller_interface.read_str()
        if message:
            self.write_to_console(message)
            self.console_output_text_box.yview('end')
            print(message)