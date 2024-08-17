from typing import Tuple, Union, List, Any, TypeVar, Generic
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from arduino_interface import ArduinoInterface


class Tab(Frame):
    """"""
    ArrayFrame = TypeVar("ArrayFrame")

    label_font: Tuple[str, int] = ("Helvetica", 18)
    """The font for the all lebels on strobe tab."""

    def __init__(self, 
                 parent: Misc, 
                 arduino_int: ArduinoInterface, 
                 nb_of_arrays: int,
                 array_frame_type: Generic[ArrayFrame]) -> "Tab":
        """Constructor for the 
        
        ### Args:
            - parameters (list): List of tuples each containing all the parameters for each LED array.
            For strobe the tuple containse
        """
        super().__init__(parent)
        self.arduino_int: ArduinoInterface = arduino_int
        #################### Make scrolable #################### 
        self.canvas = Canvas(self)
        self.scrollbar = Scrollbar(self.canvas, orient="vertical", command=self.canvas.yview)
        self.scrollbar.pack(side="right", fill="y")
        self.frame = Frame(self.canvas)
        self.frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all")))
        self.canvas.create_window((0, 0), window=self.frame, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.canvas.pack(side="left", fill="both", expand=True)
        #################### Setting up array boxes #################### 
        self.nb_cols: int = 1
        self.nb_rows: int = nb_of_arrays
        self.animation_array_frames: List[array_frame_type] = []
        for i in range(self.nb_rows):
            animation_array_frame = array_frame_type(self.frame, i,f"LED array {i+1}", arduino_int)
            animation_array_frame.grid(row=i, column=0, pady=10)
            self.animation_array_frames.append(animation_array_frame)
        
    def set_all_arrays(self) -> None:
        """Updates all the parameter values for the all the arrays for the current animation."""
        for array_frame in self.animation_array_frames:
            array_frame.send_update()