from typing import Dict, Callable, Union
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
                                                       "Sparle" : SparkleArrayFrame, 
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

        self.title_label = ttk.Label(self, text=f"Array {led_array_idx}", font=parent.array_frame_label_font, anchor="w")
        self.title_label.grid()

        # Define two sections within each frame using grid
        self.left_section = ttk.Frame(self)
        self.right_section = ttk.Frame(self)

        self.left_section.grid(row=1, column=0, sticky="nsew")
        self.right_section.grid(row=1, column=2, sticky="nsew")

        self.animation_parameter_frame_parameters: Dict[str, Dict[str, Union[int, float]]] = {anim : {"parent" : self.left_section, "array_id" : led_array_idx, "controller_interface" : self.master.controller_interface} for anim in parent.controller_interface.animation_codes.keys()}

        # Create a style object
        style = ttk.Style()
        style.configure("Custom.TSeparator", background="red")  # Change 'red' to any color you prefer
        # Create a vertical separator
        self.separator = ttk.Separator(self, orient="vertical", style="Custom.TSeparator")
        self.separator.grid(row=0, column=1, sticky="ns", padx=10, rowspan=2, pady = 5)  # 'ns' means the separator will stretch vertically

        # Make sure both sections expand equally
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=1)

        # Populate the left section with 10 buttons in a grid layout
        self.create_buttons(self.left_section)


    # Function to replace frame contents with new labels and buttons
    def switch_frame_content(self, frame: Frame, animation_label: str) -> None:
        # Clear the frame
        for widget in frame.winfo_children():
            widget.destroy()
        # Create a red "exit" button with a cross
        exit_button = ttk.Button(frame, text="✖", bootstyle=DANGER, command=lambda: self.create_buttons(frame), width=5)
        exit_button.place(x=10, y=10)  # Position in the top-left corner
        # Create a new label inside the frame
        content = self.animation_parameter_frames[animation_label](**self.animation_parameter_frame_parameters[animation_label])
        #content.grid(row=0, column=0,sticky="nsew")
        content.pack(expand=True)
        exit_button.lift(content)

    # Function to create ten buttons in a grid layout (5 rows, 2 columns) in the left section of each frame
    def create_buttons(self, parent: ttk.Frame) -> None:
        # Clear the left section first (to allow for back-and-forth switch)
        for widget in parent.winfo_children():
            widget.destroy()

        # Create 10 buttons in a grid (2 rows, 5 columns)
        for btn_idx, animation_label in zip(range(len(self.master.controller_interface.animation_codes)), self.master.controller_interface.animation_codes):
            button = ttk.Button(parent, 
                                text=f"{animation_label}", 
                                width = 15,
                                bootstyle=PRIMARY, 
                                command=lambda sec=parent, btn=animation_label: self.switch_frame_content(sec, btn))
            button.grid(row=btn_idx//5, column=btn_idx%5, padx=5, pady=5, sticky="nsew")

        # Ensure that the buttons expand evenly in the grid
        for col in range(5):
            parent.grid_columnconfigure(col, weight=0)
        for row in range(2):
            parent.grid_rowconfigure(row, weight=1)
