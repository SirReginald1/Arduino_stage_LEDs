from typing import Tuple, Union
from tkinter import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class FlashToBeatFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 controller_interface: ControllerInterface
                 ) -> "FlashToBeatFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)
        
        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)
        
        self.freq_bin_low: int = controller_interface.init_values_flash_to_beat[2]
        self.freq_bin_high: int = controller_interface.init_values_flash_to_beat[3]
        self.time_on: int = controller_interface.init_values_flash_to_beat[1]
        self.color: Tuple[int] = controller_interface.init_values_flash_to_beat[0]

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Flash to beat", font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_freq_range: Label = Label(self, text = "Frequency range: ", font=self.label_font)
        self.lab_freq_range.grid(row=1, column=0)
        self.txtin_freq_bin_low = Entry(self, textvariable=StringVar(value=str(self.freq_bin_low))) 
        self.txtin_freq_bin_low.bind("<Return>", self.enter_key_event)
        self.txtin_freq_bin_low.grid(row=1, column=1)
        self.txtin_freq_bin_high = Entry(self, textvariable=StringVar(value=str(self.freq_bin_high))) 
        self.txtin_freq_bin_high.bind("<Return>", self.enter_key_event)
        self.txtin_freq_bin_high.grid(row=1, column=2)

        self.lab_time_on: Label = Label(self, text = "Time on: ", font=self.label_font)
        self.lab_time_on.grid(row=2, column=0)
        self.txtin_time_on = Entry(self, textvariable=StringVar(value=str(self.time_on))) 
        self.txtin_time_on.bind("<Return>", self.enter_key_event)
        self.txtin_time_on.grid(row=2, column=1)

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
        self.freq_bin_low = self.txtin_freq_bin_low.get()
        self.freq_bin_high = self.txtin_freq_bin_high.get()
        self.send_update()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Flash to beat", self.LED_array_id, [self.color[0], self.color[1], self.color[2], self.time_on])#, self.freq_bin_low, self.freq_bin_high])
        else:
            self.controller_interface.change_animation("Flash to beat", array_idx, [self.color[0], self.color[1], self.color[2], self.time_on])#, self.freq_bin_low, self.freq_bin_high])


