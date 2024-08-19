from typing import Tuple
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.array_frame import ArrayFrame
  

class SparkleArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str,
                 arduino_int: ControllerInterface
                 ) -> "SparkleArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)

        self.nb_rows: int = 10
        self.nb_cols: int = 10
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.color: Tuple[int] = arduino_int.init_values_sparkle[0]
        self.delay_duration: int = arduino_int.init_values_sparkle[1]
        
        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_delay: Label = Label(self, text = "Delay duration (ms): ", font=self.label_font)
        self.lab_delay.grid(row=1, column=0)
        self.txtin_delay = Entry(self, textvariable= StringVar(value=str(self.delay_duration)))
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=1, column=1)

        self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        self.lab_color.grid(row=2, column=0)
        self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        self.btn_color_chooser.grid(row=2, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.delay_duration = self.txtin_delay.get()
        self.send_update()

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.change_animation("sparkle", self.LED_array_id, [self.color[0], self.color[1], self.color[2], self.delay_duration])


