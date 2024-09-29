from typing import Tuple, Union, List, Any
from tkinter import *
import ttkbootstrap as ttk
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class FireArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "FireArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)
        
        self.flame_height: int = controller_interface.init_values_fire[0]
        self.sparks: int = controller_interface.init_values_fire[1]
        self.delay: int = controller_interface.init_values_fire[2]
        self.intensity: float = controller_interface.init_values_fire[3]

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Fire", font=self.label_font2)
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

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Fire", self.LED_array_id, [self.flame_height, self.sparks, self.delay, self.intensity])
        else:
            self.controller_interface.change_animation("Fire", array_idx, [self.flame_height, self.sparks, self.delay, self.intensity])


