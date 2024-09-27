from typing import Union
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame

class RainbowCircleArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "RainbowCircleArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.delay: int = controller_interface.init_values_rainbow_circle[0]

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Rainbow cycle", font=self.label_font2)
        self.frame_label.pack(side=TOP, fill=BOTH, anchor=CENTER)
        #self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_delay: Label = Label(self, text = "Delay (ms): ", font=self.label_font)
        #self.lab_delay.grid(row=1, column=1)
        self.lab_delay.pack(side=LEFT, fill=BOTH, anchor=CENTER)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay))) 
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        #self.txtin_delay.grid(row=1, column=2)
        self.txtin_delay.pack(side=LEFT, fill=BOTH, anchor=CENTER)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.delay = self.txtin_delay.get()
        self.send_update()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Rainbow circle", self.LED_array_id, [self.delay])
        else:
            self.controller_interface.change_animation("Rainbow circle", array_idx, [self.delay])
