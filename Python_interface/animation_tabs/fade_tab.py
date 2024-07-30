from typing import Tuple
from tkinter import *
from arduino_interface import ArduinoInterface
from custom_widgets import Switch
from animation_tabs.array_frame import ArrayFrame


class FadeArrayFrame(ArrayFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 label: str, 
                 arduino_int: ArduinoInterface
                 ) -> "FadeArrayFrame":
        """"""
        super().__init__(parent, array_id, arduino_int)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.color: Tuple[int] = arduino_int.init_values_fade[0]
        self.random_colors: int = arduino_int.init_values_fade[1]
        
        ##################### Widgets ######################
        self.frame_label = Label(self, text=label, font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_color: Label = Label(self, text = "Use random colors : ", font=self.label_font)
        self.lab_color.grid(row=1, column=0)
        self.switch_random: Switch = Switch(self, 
                                            self.random_switch_event,
                                            "Random")
        self.switch_random.grid(row=1, column=1)

        self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        self.lab_color.grid(row=2, column=0)
        self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        self.btn_color_chooser.grid(row=2, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        raise NotImplementedError
    
    def random_switch_event(self):
        """"""
        self.random_colors = self.switch_random.my_switch.get()
        self.send_update()


    def send_update(self) -> None:
        """This function sends the curent values for the parameters to the arduino"""
        self.arduino_int.send_command("fade", self.LED_array_id, [self.color[0], self.color[1], self.color[2], self.random_colors])

