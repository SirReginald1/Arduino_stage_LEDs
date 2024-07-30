from typing import Tuple
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from arduino_interface import ArduinoInterface
from animation_tabs.array_frame import ArrayFrame


class VolumeBarArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str, 
                 arduino_int: ArduinoInterface
                 ) -> "VolumeBarArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)
        
        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        ##################### Widgets ######################
        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)


    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.send_update()

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.send_command("volume_bar", self.LED_array_id, [])


