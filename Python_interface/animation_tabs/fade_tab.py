from typing import Tuple, Union
from tkinter import *
from controller_interface import ControllerInterface
from custom_widgets import Switch
from animation_tabs.parameters_frame import ParametersFrame


class FadeArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "FadeArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)

        self.color: Tuple[int] = controller_interface.init_values_fade[0]
        self.random_colors: int = controller_interface.init_values_fade[1]
        
        ##################### Widgets ######################
        self.frame_label = Label(self, text="Fade", font=self.label_font2)
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


    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Fade", self.LED_array_id, [self.color[0], self.color[1], self.color[2], self.random_colors])
        else:
            self.controller_interface.change_animation("Fade", array_idx, [self.color[0], self.color[1], self.color[2], self.random_colors])

