from typing import Tuple, Union, List, Any
from tkinter import *
import ttkbootstrap as ttk
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface


class ArrayFrame(Frame):
    """The Frame containing all the controles for a single array."""

    label_font: Tuple[str, int] = ("Helvetica", 18)
    """The font for the all lebels on strobe tab."""

    label_font2: Tuple[str, int] = ("Helvetica", 9)
    """The font for the all lebels on strobe tab."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 arduino_int: ControllerInterface
                 ) -> "ArrayFrame":
        """"""
        super().__init__(parent)
        self.LED_array_id: int = array_id
        """The id of the LED array that this command panel controls."""
        self.arduino_int: ControllerInterface = arduino_int
        self.color: Tuple[int] = None
        self.nb_rows: int = 10
        self.nb_cols: int = 10
        
    def choos_color(self) -> None:
        """"""
        color_chooser: ColorChooserDialog = ColorChooserDialog()
        color_chooser.show()
        res = color_chooser.result
        self.btn_color_chooser.configure(bg=res.hex)
        self.color = res.rgb
        self.send_update()

    def enter_key_event(self, event: Event) -> None:
        """"""
        raise NotImplementedError

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        raise NotImplementedError