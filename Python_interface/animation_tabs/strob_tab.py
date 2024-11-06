from typing import Tuple, Union
from tkinter import *
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class StrobeArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "StrobeArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)
        """The id of the LED array that this command panel controls."""
        self.time_on: int = controller_interface.init_values_strobe[0]
        self.time_off: int = controller_interface.init_values_strobe[1]
        self.color: Tuple[int] = controller_interface.init_values_strobe[2]

        self.animation_label: str = "Strobe"
        """The str value that represents the animation"""

        self.nb_rows: int = 10
        self.nb_cols: int = 10
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.frame_label = Label(self, text="Strobe", font=self.label_font2)
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

    def update_parameter_values(self) -> None:
        "Updates all the animation parameter values in the interface those present in the entry boxes."
        self.time_on = self.txtin_time_on.get()
        self.time_off = self.txtin_time_off.get()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if array_idx == None:
            self.controller_interface.change_animation("Strobe", self.LED_array_id, [self.time_on, self.time_off, self.color[0], self.color[1], self.color[2]])
        else:
            self.controller_interface.change_animation("Strobe", array_idx, [self.time_on, self.time_off, self.color[0], self.color[1], self.color[2]])