from typing import Tuple, Union, List, Any
from tkinter import *
import ttkbootstrap as ttk
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.array_frame import ArrayFrame


class FireArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str, 
                 arduino_int: ControllerInterface
                 ) -> "FireArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)
        
        self.flame_height: int = arduino_int.init_values_fire[0]
        self.sparks: int = arduino_int.init_values_fire[1]
        self.delay: int = arduino_int.init_values_fire[2]
        self.intensity: float = arduino_int.init_values_fire[3]

        ##################### Widgets ######################
        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_flame_height: Label = Label(self, text = "Flame Height: ", font=self.label_font)
        self.lab_flame_height.grid(row=1, column=0)
        self.txtin_flame_height = Entry(self, textvariable=StringVar(value=str(self.flame_height))) 
        self.txtin_flame_height.bind("<Return>", self.enter_key_event)
        self.txtin_flame_height.grid(row=1, column=1)

        self.lab_sparks: Label = Label(self, text = "Sparks: ", font=self.label_font)
        self.lab_sparks.grid(row=2, column=0)
        self.txtin_sparks = Entry(self, textvariable=StringVar(value=str(self.sparks))) 
        self.txtin_sparks.bind("<Return>", self.enter_key_event)
        self.txtin_sparks.grid(row=2, column=1)

        self.lab_delay: Label = Label(self, text = "Delay (ms): ", font=self.label_font)
        self.lab_delay.grid(row=3, column=0)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay))) 
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=3, column=1)

        self.lab_intensity: Label = Label(self, text = "Intensity: ", font=self.label_font)
        self.lab_intensity.grid(row=4, column=0)
        self.txtin_intensity = Entry(self, textvariable=StringVar(value=str(self.intensity))) 
        self.txtin_intensity.bind("<Return>", self.enter_key_event)
        self.txtin_intensity.grid(row=4, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.delay = self.txtin_delay.get()
        self.flame_height = self.txtin_flame_height.get()
        self.sparks = self.txtin_sparks.get()
        self.intensity = self.txtin_intensity.get()
        self.send_update()

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.change_animation("fire", self.LED_array_id, [self.flame_height, self.sparks, self.delay, self.intensity])


