from typing import Tuple
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.array_frame import ArrayFrame

class RainbowCircleArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str, 
                 arduino_int: ControllerInterface
                 ) -> "RainbowCircleArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.delay: int = arduino_int.init_values_rainbow_circle[0]

        ##################### Widgets ######################
        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_delay: Label = Label(self, text = "Delay (ms): ", font=self.label_font)
        self.lab_delay.grid(row=1, column=0)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay))) 
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=1, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.delay = self.txtin_delay.get()
        self.send_update()

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.change_animation("rainbow_circle", self.LED_array_id, [self.delay])

