from typing import Tuple, Union, List, Any
from tkinter import Misc, Event, IntVar, Frame
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from ttkbootstrap.dialogs.colorchooser import ColorChooserDialog
from controller_interface import ControllerInterface


class ParametersFrame(Frame):
    """The Frame containing all the controles for a single array."""

    label_font: Tuple[str, int] = ("Helvetica", 18)
    """The font for the all lebels on strobe tab."""

    label_font2: Tuple[str, int] = ("Helvetica", 9)
    """The font for the all lebels on strobe tab."""

    def __init__(self, 
                 parent: Misc,
                 array_id: int,
                 controller_interface: ControllerInterface
                 ) -> "ParametersFrame":
        """"""
        super().__init__(parent) #, borderwidth=3, relief="ridge"
        self.LED_array_id: int = array_id
        """The id of the LED array that this command panel controls."""
        self.arduino_int: ControllerInterface = controller_interface
        self.color: Tuple[int] = None
        self.nb_rows: int = 10
        self.nb_cols: int = 10
        self.array_selection = CheckboxFrame(parent, self.master.master.master.app.NB_ARRAYS)
        self.array_selection.pack(side=RIGHT)
        
    def choos_color(self) -> None:
        """"""
        color_chooser: ColorChooserDialog = ColorChooserDialog()
        color_chooser.show()
        res = color_chooser.result
        self.btn_color_chooser.configure(bg=res.hex)
        self.color = res.rgb
        self.send_update()

    def enter_key_event(self, event: Event) -> None:
        """"""
        raise NotImplementedError

    def send_update(self, array_idx: Union[int, None] = None) -> None:
        """This function sends the curent values for the parameters to the controller.
        
        ### Args:
            - array_idx (int, None; Optional): The index of the array that will be set to the selected parameters.
            Used for synching multiple arrays using the checkboxes.
        """
        raise NotImplementedError
    
class CheckboxFrame(ttk.Frame):

    def __init__(self, parent: Misc,
                 n: int,
                 *args: Any,
                 **kwargs: Any) -> None:
        super().__init__(parent, *args, **kwargs)
        
        self.check_vars = []  # To store the IntVars for the checkboxes
        
        # Create n+1 checkboxes dynamically
        for i in range(n+1):
            var = IntVar()  # Create an IntVar for each checkbox
            if i == 0:
                checkbox = ttk.Checkbutton(self, text=f"All", variable=var, command=lambda idx=i: self.check_checkbox(idx))
            else:    
                checkbox = ttk.Checkbutton(self, text=f"Array {i}", variable=var, command=lambda idx=i: self.check_checkbox(idx))
            checkbox.grid(row=i, column=0, sticky="w", padx=10, pady=5)
            self.check_vars.append(var)  # Store the variable

        # Button to print the state of each checkbox
        print_button = ttk.Button(self, text="Set arrays", command=lambda: print(f"Parent: {type(parent)}")) #parent.send_update
        print_button.grid(row=n+1, column=0, pady=10)

    def check_checkbox(self, idx: int) -> None:
        """Function called when a checkbox is ticked.
        
        ### Args:
            - idx (int): The index of the checkbox making the call.
        """
        #print(f"idx: {idx}, value: {self.check_vars[idx].get()}")
        self.update_checkboxes(idx)

    # Function to handle checkbox selection behavior
    def update_checkboxes(self, idx: int) -> None:
        """Function used to update the checkboxe selection."""
        if idx == 0:  # If the first checkbox is selected
            if self.check_vars[0].get() == 1:  # If it's checked, uncheck all others
                for var in self.check_vars[1:]:
                    var.set(0)
        else:  # If any of the other checkboxes is selected
            if self.check_vars[idx].get() == 1:  # If the current one is checked
                self.check_vars[0].set(0)  # Uncheck the first one
    
    def send_message_to_arrays(self) -> None:
        """Function that sends the appropriate update message to the controller."""
        message = ""
        #for i in self.check_vars:

        print(f"Message sent: {message}")

    # Function to print the current state of all checkboxes
    def print_states(self) -> None:
        """Print the stats of the checkboxes"""
        states = [var.get() for var in self.check_vars]  # Get the value of each IntVar
        print("Checkbox States:", states)