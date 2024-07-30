from typing import Tuple
from tkinter import *
from arduino_interface import ArduinoInterface
from animation_tabs.array_frame import ArrayFrame


class StrobeArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str, 
                 arduino_int: ArduinoInterface
                 ) -> "StrobeArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)
        """The id of the LED array that this command panel controls."""
        self.time_on: int = arduino_int.init_values_strobe[0]
        self.time_off: int = arduino_int.init_values_strobe[1]
        self.color: Tuple[int] = arduino_int.init_values_strobe[2]

        self.nb_rows: int = 10
        self.nb_cols: int = 10
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_time_on: Label = Label(self, text = "Time spent on (ms): ", font=self.label_font)
        self.lab_time_on.grid(row=1, column=0)
        self.txtin_time_on = Entry(self, textvariable = StringVar(value=str(self.time_on)))
        self.txtin_time_on.bind("<Return>", self.enter_key_event)
        self.txtin_time_on.grid(row=1, column=1)

        self.lab_time_off: Label = Label(self, text = "Time spent off (ms): ", font=self.label_font)
        self.lab_time_off.grid(row=2, column=0)
        self.txtin_time_off = Entry(self, textvariable = StringVar(value=str(self.time_off)))
        self.txtin_time_off.bind("<Return>", self.enter_key_event)
        self.txtin_time_off.grid(row=2, column=1)

        self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        self.lab_color.grid(row=3, column=0)
        self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        self.btn_color_chooser.grid(row=3, column=1)        

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.time_on = self.txtin_time_on.get()
        self.time_off = self.txtin_time_off.get()
        self.send_update()

    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.send_command("strobe", self.LED_array_id, [self.time_on, self.time_off, self.color[0], self.color[1], self.color[2]])