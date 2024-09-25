import tkinter as tk
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from controller_interface import ControllerInterface
from menu_bar import MenuBar, CustomMenuBar

animation_codes = {"Rainbow circle" : 0,
                   "Fade" : 1,
                   "Sparle" : 2, 
                   "Fire" : 3, 
                   "Shooting star" : 4, 
                   "Twinkle pixel" : 5, 
                   "Strobe" : 6, 
                   "Zip" : 7, 
                   "Flash to beat" : 8}

# Function to replace frame contents with new labels and buttons
def switch_frame_content(frame):
    # Clear the frame
    for widget in frame.winfo_children():
        widget.destroy()
    # Create a red "exit" button with a cross
    exit_button = ttk.Button(frame, text="✖", bootstyle=WARNING, command=lambda: create_buttons(frame), width=5)
    exit_button.place(x=10, y=10)  # Position in the top-left corner
    # Create a new label inside the frame
    label = ttk.Label(frame, text="New Content", font=("Arial", 20), bootstyle=SECONDARY)
    label.pack(pady=20)

# Function to create ten buttons in a grid layout (5 rows, 2 columns) in the left section of each frame
def create_buttons(left_section):
    # Clear the left section first (to allow for back-and-forth switch)
    for widget in left_section.winfo_children():
        widget.destroy()

    # Create 10 buttons in a grid (2 rows, 5 columns)
    for btn_idx, animation_label in zip(range(len(animation_codes)), animation_codes):
        button = ttk.Button(left_section, 
                            text=f"{animation_label}", 
                            width = 15,
                            bootstyle=PRIMARY, 
                            command=lambda sec=left_section: switch_frame_content(sec))
        button.grid(row=btn_idx//5, column=btn_idx%5, padx=5, pady=5, sticky="nsew")

    # Ensure that the buttons expand evenly in the grid
    for col in range(5):
        left_section.grid_columnconfigure(col, weight=0)
    for row in range(2):
        left_section.grid_rowconfigure(row, weight=1)

# Main window setup
root = ttk.Window(themename="superhero")  # Use ttkbootstrap Window with 'superhero' theme
root.title("4 Frames Interface with Superhero Theme")

# Set the window to take up the full width of the screen
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

# We will limit the total window height so that the frames don't take up the full screen height
window_height = screen_height // 2  # Half the screen height
root.geometry(f"{screen_width}x{window_height}")

# Define number of frames and their relative height
num_frames = 4
frame_height = window_height // num_frames

# Create 4 frames, each with 2 sections, and stack them at the top of the window
for i in range(num_frames):
    array_frame = ttk.Frame(root, height=frame_height)
    array_frame.pack(fill=tk.X, side="top")  # Stack frames at the top

    title_label = ttk.Label(array_frame, text=f"Array {i}", font=("Helvetica", 18), anchor="w")
    title_label.grid()

    # Define two sections within each frame using grid
    left_section = ttk.Frame(array_frame)
    right_section = ttk.Frame(array_frame)

    left_section.grid(row=1, column=0, sticky="nsew")
    right_section.grid(row=1, column=2, sticky="nsew")

    # Create a style object
    style = ttk.Style()
    style.configure("Custom.TSeparator", background="red")  # Change 'red' to any color you prefer
    # Create a vertical separator
    separator = ttk.Separator(array_frame, orient="vertical", style="Custom.TSeparator")
    separator.grid(row=0, column=1, sticky="ns", padx=10, rowspan=2, pady = 5)  # 'ns' means the separator will stretch vertically
    
    # Make sure both sections expand equally
    array_frame.grid_columnconfigure(0, weight=1)
    array_frame.grid_columnconfigure(1, weight=1)

    # Populate the left section with 10 buttons in a grid layout
    create_buttons(left_section)

#menu_bar = MenuBar(root)
menu_bar = CustomMenuBar(root)
menu_bar.pack(side=tk.TOP, fill=tk.X)
# Configure the menu bar on the main window
root.config(menu=menu_bar)



# Use option_add to set colors for the menu (may only work on some platforms like Windows)
#root.option_add('*TearOff', False)
#root.option_add('*Menu.background', '#2C3E50')  # Dark background
#root.option_add('*Menu.foreground', '#ECF0F1')  # Light text
#root.option_add('*Menu.activeBackground', '#2980B9')  # Highlight color when hovering
#root.option_add('*Menu.activeForeground', '#FFFFFF')  # Active text color
# Start the main event loop
root.mainloop()
