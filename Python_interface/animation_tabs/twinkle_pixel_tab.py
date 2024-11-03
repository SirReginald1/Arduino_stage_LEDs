from typing import Tuple, Union
from tkinter import *
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class TwinklePixelArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "TwinklePixelArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)
        
        self.animation_label: str = "Twinkle pixel"
        """The str value that represents the animation"""

        self.color: int = controller_interface.init_values_twinkle_pixel[0]
        self.color_saturation: int = controller_interface.init_values_twinkle_pixel[1]
        self.pixel_volume: int = controller_interface.init_values_twinkle_pixel[2]
        self.fade_amount: int = controller_interface.init_values_twinkle_pixel[3]
        self.delay: int = controller_interface.init_values_twinkle_pixel[4]
        #self.color: Tuple[int] = (255, 255, 255)

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Twinkle Pixel", font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_color: Label = Label(self, text = "Color: ", font=self.label_font)
        self.lab_color.grid(row=1, column=0)
        self.txtin_color = Entry(self, textvariable=StringVar(value=str(self.color))) 
        self.txtin_color.bind("<Return>", self.enter_key_event)
        self.txtin_color.grid(row=1, column=1)

        self.lab_color_saturation: Label = Label(self, text = "Color Saturation: ", font=self.label_font)
        self.lab_color_saturation.grid(row=2, column=0)
        self.txtin_color_saturation = Entry(self, textvariable=StringVar(value=str(self.color_saturation)))
        self.txtin_color_saturation.bind("<Return>", self.enter_key_event)
        self.txtin_color_saturation.grid(row=2, column=1)

        self.lab_pixel_volume: Label = Label(self, text = "Pixel volume: ", font=self.label_font)
        self.lab_pixel_volume.grid(row=3, column=0)
        self.txtin_pixel_volume = Entry(self, textvariable=StringVar(value=str(self.pixel_volume)))
        self.txtin_pixel_volume.bind("<Return>", self.enter_key_event)
        self.txtin_pixel_volume.grid(row=3, column=1)

        self.lab_fade_amount: Label = Label(self, text = "Fade Amount: ", font=self.label_font)
        self.lab_fade_amount.grid(row=4, column=0)
        self.txtin_fade_amount = Entry(self, textvariable=StringVar(value=str(self.fade_amount)))
        self.txtin_fade_amount.bind("<Return>", self.enter_key_event)
        self.txtin_fade_amount.grid(row=4, column=1)

        self.lab_delay: Label = Label(self, text = "Delay (ms): ", font=self.label_font)
        self.lab_delay.grid(row=5, column=0)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay)))
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=5, column=1)

        #self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        #self.lab_color.grid(row=6, column=0)
        #self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        #self.btn_color_chooser.grid(row=6, column=1)

    def update_parameter_values(self) -> None:
        "Updates all the animation parameter values in the interface those present in the entry boxes."
        self.color = self.txtin_color.get()
        self.color_saturation = self.txtin_color_saturation.get()
        self.pixel_volume = self.txtin_pixel_volume.get()
        self.fade_amount = self.txtin_fade_amount.get()
        self.delay = self.txtin_delay.get()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if array_idx == None:
            self.controller_interface.change_animation("Twinkle pixel", self.LED_array_id, [self.color, self.color_saturation, self.pixel_volume, self.fade_amount, self.delay])
        else:
            self.controller_interface.change_animation("Twinkle pixel", array_idx, [self.color, self.color_saturation, self.pixel_volume, self.fade_amount, self.delay])


