import tkinter as tk
from tkinter import Misc
import ttkbootstrap as ttk
from ttkbootstrap.constants import *

class CustomMenuBar(ttk.Frame):

    def __init__(self,
                 parent: Misc) -> "CustomMenuBar":
        # Function to handle menu actions

        # Create a custom menu bar using a frame to control its color
        super().__init__(parent, bootstyle="dark", padding=(5, 5))
        #self.pack(side=tk.TOP, fill=tk.X)

        # Create the actual Tkinter Menu (inside the frame)
        self.menu_bar = tk.Menu(self, tearoff=0)
        #self.menu_bar.pack()

        # Add "File" menu with sub-items
        self.file_menu = tk.Menu(self.menu_bar, tearoff=0)
        self.file_menu.add_command(label="New", command=lambda: self.menu_action("New"))
        self.file_menu.add_command(label="Open", command=lambda: self.menu_action("Open"))
        self.file_menu.add_command(label="Save", command=lambda: self.menu_action("Save"))
        self.file_menu.add_separator()
        self.file_menu.add_command(label="Exit", command=lambda: self.menu_action("Pressed quit!"))

        # Add "Edit" menu with sub-items
        self.edit_menu = tk.Menu(self.menu_bar, tearoff=0)
        self.edit_menu.add_command(label="Undo", command=lambda: self.menu_action("Undo"))
        self.edit_menu.add_command(label="Redo", command=lambda: self.menu_action("Redo"))
        self.edit_menu.add_separator()
        self.edit_menu.add_command(label="Cut", command=lambda: self.menu_action("Cut"))
        self.edit_menu.add_command(label="Copy", command=lambda: self.menu_action("Copy"))
        self.edit_menu.add_command(label="Paste", command=lambda: self.menu_action("Paste"))

        # Add the menus to the menu bar
        self.menu_bar.add_cascade(label="File", menu=self.file_menu)
        self.menu_bar.add_cascade(label="Edit", menu=self.edit_menu)

    # Function to handle menu actions
    def menu_action(self, action_name):
        print(f"{action_name} selected")

   
# Create the main window
root = ttk.Window(themename="superhero")
root.title("Custom Menu Bar with Color")

menu_bar = CustomMenuBar(root)
menu_bar.pack(side=tk.TOP, fill=tk.X)

# Add a section below to simulate the main content
main_content = ttk.Frame(root, padding=20)
main_content.pack(fill=tk.BOTH, expand=True)

# Sample label for main content
sample_label = ttk.Label(main_content, text="Main Content Goes Here", font=("Helvetica", 18))
sample_label.pack(pady=20)

# Start the main loop
root.mainloop()