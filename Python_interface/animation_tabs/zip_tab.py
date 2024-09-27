from typing import Tuple, Union
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class ZipArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "ZipArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.size: int = controller_interface.init_values_zip[0]
        self.start: int = controller_interface.init_values_zip[1]
        self.end: int = controller_interface.init_values_zip[2]
        self.delay: int = controller_interface.init_values_zip[3]
        self.color: Tuple[int] = controller_interface.init_values_zip[4]

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Zip", font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_size: Label = Label(self, text = "Size: ", font=self.label_font)
        self.lab_size.grid(row=1, column=0)
        self.txtin_size = Entry(self, textvariable=StringVar(value=str(self.size))) 
        self.txtin_size.bind("<Return>", self.enter_key_event)
        self.txtin_size.grid(row=1, column=1)

        self.lab_start: Label = Label(self, text = "Start: ", font=self.label_font)
        self.lab_start.grid(row=2, column=0)
        self.txtin_start = Entry(self, textvariable=StringVar(value=str(self.start)))
        self.txtin_start.bind("<Return>", self.enter_key_event)
        self.txtin_start.grid(row=2, column=1)

        self.lab_end: Label = Label(self, text = "End: ", font=self.label_font)
        self.lab_end.grid(row=3, column=0)
        self.txtin_end = Entry(self, textvariable=StringVar(value=str(self.end)))
        self.txtin_end.bind("<Return>", self.enter_key_event)
        self.txtin_end.grid(row=3, column=1)

        self.lab_delay: Label = Label(self, text = "Delay: ", font=self.label_font)
        self.lab_delay.grid(row=4, column=0)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay)))
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=4, column=1)

        self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        self.lab_color.grid(row=5, column=0)
        self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        self.btn_color_chooser.grid(row=5, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.size = self.txtin_size.get()
        self.start = self.txtin_start.get()
        self.end = self.txtin_end.get()
        self.delay = self.txtin_delay.get()
        self.send_update()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Zip", self.LED_array_id, [self.size, self.start, self.end, self.delay, self.color[0], self.color[1], self.color[2]])
        else:
            self.controller_interface.change_animation("Zip", array_idx, [self.size, self.start, self.end, self.delay, self.color[0], self.color[1], self.color[2]])


