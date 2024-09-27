from typing import Tuple, Union
from tkinter import *
from controller_interface import ControllerInterface
from animation_tabs.parameters_frame import ParametersFrame


class ShootingStarArrayFrame(ParametersFrame):
    """The Frame containing all the controles for a single array."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 #label: str, 
                 controller_interface: ControllerInterface
                 ) -> "ShootingStarArrayFrame":
        """"""
        super().__init__(parent, array_id, controller_interface)

        self.nb_rows: int = 10
        self.nb_cols: int = 2
        self.rowconfigure(tuple(range(self.nb_rows)), weight=1)
        self.columnconfigure(tuple(range(self.nb_cols)), weight=1)
        
        self.tail_length: int = controller_interface.init_values_shooting_star[1]
        self.delay: int = controller_interface.init_values_shooting_star[2]
        self.interval: int = controller_interface.init_values_shooting_star[3]
        self.direction: int = controller_interface.init_values_shooting_star[4]
        self.color: Tuple[int] = controller_interface.init_values_shooting_star[0]

        ##################### Widgets ######################
        self.frame_label = Label(self, text="Shooting Star", font=self.label_font2)
        self.frame_label.grid(row=0, column=0, columnspan=2)

        self.lab_tail_length: Label = Label(self, text = "Tail length: ", font=self.label_font)
        self.lab_tail_length.grid(row=1, column=0)
        self.txtin_tail_length = Entry(self, textvariable=StringVar(value=str(self.tail_length))) 
        self.txtin_tail_length.bind("<Return>", self.enter_key_event)
        self.txtin_tail_length.grid(row=1, column=1)

        self.lab_delay: Label = Label(self, text = "Delay (ms): ", font=self.label_font)
        self.lab_delay.grid(row=2, column=0)
        self.txtin_delay = Entry(self, textvariable=StringVar(value=str(self.delay)))
        self.txtin_delay.bind("<Return>", self.enter_key_event)
        self.txtin_delay.grid(row=2, column=1)

        self.lab_interval: Label = Label(self, text = "Interval: ", font=self.label_font)
        self.lab_interval.grid(row=3, column=0)
        self.txtin_interval = Entry(self, textvariable=StringVar(value=str(self.interval)))
        self.txtin_interval.bind("<Return>", self.enter_key_event)
        self.txtin_interval.grid(row=3, column=1)

        self.lab_direction: Label = Label(self, text = "Interval: ", font=self.label_font)
        self.lab_direction.grid(row=4, column=0)
        self.switch_direction = Entry(self, textvariable=StringVar(value=str(self.direction)))
        self.switch_direction.bind("<Return>", self.enter_key_event)
        self.switch_direction.grid(row=4, column=1)

        self.lab_color: Label = Label(self, text = "Color : ", font=self.label_font)
        self.lab_color.grid(row=5, column=0)
        self.btn_color_chooser: Button = Button(self, text="Choos color", command=self.choos_color)
        self.btn_color_chooser.grid(row=5, column=1)

    def enter_key_event(self, event: Event) -> None:
        """The action that is performed when the enter key is pressed.
        
        ### Args:
            - event (Event): The key pess event.
        """
        self.delay_duration = self.txtin_delay.get()
        self.tail_length = self.txtin_tail_length.get()
        self.interval = self.txtin_interval.get()
        self.direction = self.switch_direction.get()
        self.send_update()

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        if not array_idx:
            self.controller_interface.change_animation("Shooting star", self.LED_array_id, [self.color[0], self.color[1], self.color[2], self.tail_length, self.delay, self.interval, self.direction])
        else:
            self.controller_interface.change_animation("Shooting star", array_idx, [self.color[0], self.color[1], self.color[2], self.tail_length, self.delay, self.interval, self.direction])


