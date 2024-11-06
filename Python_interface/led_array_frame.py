from typing import Dict, Callable, Union, List, Any
from tkinter import *
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from animation_tabs.strob_tab import StrobeArrayFrame
from animation_tabs.sparkle_tab import SparkleArrayFrame
from animation_tabs.shooting_star_tab import ShootingStarArrayFrame
from animation_tabs.twinkle_pixel_tab import TwinklePixelArrayFrame
from animation_tabs.zip_tab import ZipArrayFrame
from animation_tabs.rainbow_circle import RainbowCircleArrayFrame
from animation_tabs.fade_tab import FadeArrayFrame
from animation_tabs.fire_tab import FireArrayFrame
from animation_tabs.flash_to_beat_tab import FlashToBeatFrame

class LedArrayFrame(Frame):

    animation_parameter_frames: Dict[str, Callable] = {"Rainbow circle" : RainbowCircleArrayFrame,
                                                       "Fade" : FadeArrayFrame,
                                                       "Sparkle" : SparkleArrayFrame, 
                                                       "Fire" : FireArrayFrame, 
                                                       "Shooting star" : ShootingStarArrayFrame, 
                                                       "Twinkle pixel" : TwinklePixelArrayFrame, 
                                                       "Strobe" : StrobeArrayFrame, 
                                                       "Zip" : ZipArrayFrame, 
                                                       "Flash to beat" : FlashToBeatFrame}


    def __init__(self,
                 parent: Misc,
                 led_array_idx: int,
                 parent_window_height: int):
        super().__init__(parent, 
                         height=parent_window_height, 
                         borderwidth=3, 
                         relief="solid", 
                         highlightcolor="black", 
                         highlightbackground="black",
                         pady=10)

        self.led_array_idx: int = led_array_idx

        # Configure the grid inside the parent frame to center the three frames
        self.grid_rowconfigure(0, weight=1)  # Top spacer row
        self.grid_rowconfigure(1, weight=1)  # Row with frames
        self.grid_rowconfigure(2, weight=1)  # Bottom spacer row

        self.grid_columnconfigure(0, weight=1)  # Left spacer column
        self.grid_columnconfigure(1, weight=2)  # First frame column
        self.grid_columnconfigure(2, weight=2)  # Second frame column
        self.grid_columnconfigure(3, weight=2)  # Third frame column
        self.grid_columnconfigure(4, weight=1)  # Right spacer column

        self.title_label: ttk.Label = ttk.Label(self, text=f"Array {led_array_idx}", font=parent.array_frame_label_font, anchor="w")
        self.title_label.grid(row=0, column=2)

        # Define two sections within each frame using grid
        self.param_frame: ttk.Frame = ttk.Frame(self)
        self.param_frame.grid(row=1, column=1, sticky="nsew")

        self.var_frame: ttk.Frame = ttk.Frame(self)
        self.var_frame.grid(row=1, column=3, sticky="nsew")
        
        # Create a style object
        style = ttk.Style()
        style.configure("Custom.TSeparator", background="black")
        # Create a vertical separator
        self.separator: ttk.Separator = ttk.Separator(self, orient="vertical", style="Custom.TSeparator")
        self.separator.grid(row=1, column=2, sticky="ns", rowspan=2, pady = 5)  # 'ns' means the separator will stretch vertically

        self.animation_parameter_frame_parameters: Dict[str, Dict[str, Union[int, float]]] = {
            anim : {
                    "parent" : self.param_frame, 
                    "array_id" : led_array_idx, 
                    "controller_interface" : self.master.controller_interface
                    } for anim in parent.controller_interface.animation_codes.keys()
            }
        # Populate the left section with 10 buttons in a grid layout
        self.create_buttons(self.param_frame, self.var_frame)


    # Function to replace frame contents with new labels and buttons
    def switch_frame_content(self, param_frame: Frame, var_frame: Frame, animation_label: str) -> None:
        # Clear the frame
        for widget in param_frame.winfo_children():
            widget.destroy()
        # Create a red "exit" button with a cross
        exit_button = ttk.Button(param_frame, text="✖", bootstyle=DANGER, command=lambda: self.create_buttons(param_frame, var_frame), width=5)
        exit_button.place(x=10, y=10)  # Position in the top-left corner
        # Create a new label inside the frame
        content = self.animation_parameter_frames[animation_label](**self.animation_parameter_frame_parameters[animation_label])
        #content.grid(row=0, column=0,sticky="nsew")
        content.pack(expand=True)
        exit_button.lift(content)
        wiget_lab_lists = self.master.controller_interface.animation_var_array_dict[animation_label]
        var_frame.content = VariableFrame(var_frame, wiget_lab_lists[0], wiget_lab_lists[1],wiget_lab_lists[2])
        var_frame.content.pack()

    # Function to create ten buttons in a grid layout (5 rows, 2 columns) in the left section of each frame
    def create_buttons(self, param_frame: ttk.Frame, var_frame: ttk.Frame) -> None:
        # Clear the left section first (to allow for back-and-forth switch)
        for widget in param_frame.winfo_children():
            widget.destroy()
        for widget in var_frame.winfo_children():
            widget.destroy()

        # Create 10 buttons in a grid (2 rows, 5 columns)
        for btn_idx, animation_label in zip(range(len(self.master.controller_interface.animation_codes)), self.master.controller_interface.animation_codes):
            button = ttk.Button(param_frame, 
                                text=f"{animation_label}", 
                                width = 15,
                                bootstyle=PRIMARY, 
                                command=lambda sec=param_frame, widg=var_frame, btn=animation_label: self.switch_frame_content(sec, widg, btn))
            button.grid(row=btn_idx//5, column=btn_idx%5, padx=5, pady=5, sticky="ew")
        
        # Ensure that the buttons expand evenly in the grid
        for col in range(5):
            param_frame.grid_columnconfigure(col, weight=0)
        for row in range(2):
            param_frame.grid_rowconfigure(row, weight=1)

class VariableFrame(ttk.Frame):
    """The object containing the widgets for synching animation."""

    def __init__(self, 
                 parent: Misc, 
                 labels: List[str]=[], 
                 entries: List[str]=[], 
                 checkboxes: List[str]=[], 
                 *args, 
                 **kwargs):
        super().__init__(parent, *args, **kwargs)

        # Create a canvas and a scrollbar
        self.canvas = Canvas(self, borderwidth=0, height=150, width=700)
        #self.canvas.pack_propagate(False)
        self.scrollbar = ttk.Scrollbar(self, orient="horizontal", command=self.canvas.xview)
        self.scrollable_frame = ttk.Frame(self.canvas)

        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )

        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.canvas.configure(xscrollcommand=self.scrollbar.set)

        # Pack the canvas and scrollbar
        self.canvas.pack(side="top", fill="x", expand=False, anchor=CENTER)
        self.scrollbar.pack(side="bottom", fill="x")

        # Send message button
        self.btn_synch: ttk.Button = ttk.Button(self, 
                                                text="Sync", 
                                                width = 15,
                                                bootstyle=PRIMARY, 
                                                command=lambda self=self: send_sync_message(self))
        self.btn_synch.pack()

        # Add widgets dynamically based on the arrays
        self.label_vars: StringVar = []
        self.entry_vars: StringVar = []
        self.checkbox_vars: BooleanVar = []

        def send_sync_message(self: Any) -> None:
            """Sends the synching message to controller."""
            message = f"@3<{self.master.master.led_array_idx},"
            for i in range(len(self.label_vars)):
                if self.checkbox_vars[i].get():
                    message = f"{message}{i},{self.master.master.master.controller_interface.animation_codes[self.master.master.param_frame.winfo_children()[0].animation_label]},{self.entry_vars[i].get()},"
            self.master.master.master.controller_interface.send_message(message[:-1] + ">")
        # Horizontal layout for labels, entries, and checkboxes
        #print(labels)
        nb_test = 4
        row = 0
        for i, label_text in enumerate(labels):
            label_var = StringVar(value=label_text)
            label = ttk.Label(self.scrollable_frame, textvariable=label_var)
            label.grid(row=row, column=i%nb_test, padx=10, pady=5)
            self.label_vars.append(label_var)
            if i != 0 and i%nb_test == 0:
                row += 3

        row = 1
        for i, entry_default in enumerate(entries):
            entry_var = StringVar(value=entry_default)
            entry = ttk.Entry(self.scrollable_frame, textvariable=entry_var, width = 10)
            entry.grid(row=row, column=i%nb_test, padx=10, pady=5)
            self.entry_vars.append(entry_var)
            if i != 0 and i%nb_test == 0:
                row += 3

        row = 2
        for i, checkbox_text in enumerate(checkboxes):
            checkbox_var = BooleanVar()
            checkbutton = ttk.Checkbutton(self.scrollable_frame, text=checkbox_text, variable=checkbox_var)
            checkbutton.grid(row=row, column=i%nb_test, padx=10, pady=5)
            self.checkbox_vars.append(checkbox_var)
            if i != 0 and i%nb_test == 0:
                row += 3

    def create_variable_widgets(self, parent: ttk.Frame) -> None:
        """This function builds the appropriate widget selction in the var_frame for animation variable selecticon
        when an animation button is pressed.

        ### Args:
            - The frame in which the widgets will be placed.
        """

    def get_values(self):
        """
        Retrieve the current values of the widgets.
        Returns a dictionary with the values of labels, entries, and checkboxes.
        """
        label_values = [var.get() for var in self.label_vars]
        entry_values = [var.get() for var in self.entry_vars]
        checkbox_values = [var.get() for var in self.checkbox_vars]

        return {
            'labels': label_values,
            'entries': entry_values,
            'checkboxes': checkbox_values
        }

"""
# Define some example data arrays
labels = [f"Label {i+1}" for i in range(10)]
entries = [f"Entry {i+1}" for i in range(10)]
checkboxes = [f"Checkbox {i+1}" for i in range(10)]

# Create a HorizontalScrollableFrame with the arrays
scrollable_frame = HorizontalScrollableFrame(root, labels=labels, entries=entries, checkboxes=checkboxes)
scrollable_frame.pack(fill="both", expand=True, padx=20, pady=20)

# Function to display the values of the widgets
def show_values():
    values = scrollable_frame.get_values()
    print("Labels:", values['labels'])
    print("Entries:", values['entries'])
    print("Checkboxes:", values['checkboxes'])

# Button to trigger value display
show_values_button = ttk.Button(root, text="Show Values", command=show_values)
show_values_button.pack(pady=10)

# Start the main Tkinter event loop
root.mainloop()
"""
