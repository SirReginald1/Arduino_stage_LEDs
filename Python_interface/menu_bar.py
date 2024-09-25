from typing import Callable
import tkinter as tk
from tkinter import Button
import ttkbootstrap as ttk
from tkinter import Misc, Event
from custom_widgets import SlidePanel

class MenuBar(tk.Menu):

    def __init__(self,
                 parent: Misc) -> "MenuBar":
        super().__init__(parent)

        self.file_menu = tk.Menu(parent, tearoff=0)
        # Add "File" menu with sub-items
        self.file_menu = tk.Menu(parent, tearoff=0)
        self.file_menu.add_command(label="New")
        self.file_menu.add_command(label="Open")
        self.file_menu.add_command(label="Save")
        self.file_menu.add_separator()  # Adds a separator line
        self.file_menu.add_command(label="Exit", command=lambda: print("Menu bar file menu quit!"))

        # Add "Edit" menu with sub-items
        self.edit_menu = tk.Menu(parent, tearoff=0)
        self.edit_menu.add_command(label="Undo")
        self.edit_menu.add_command(label="Redo")
        self.edit_menu.add_separator()
        self.edit_menu.add_command(label="Cut")
        self.edit_menu.add_command(label="Copy")
        self.edit_menu.add_command(label="Paste")

        # Add the menus to the menu bar
        self.add_cascade(label="File", menu=self.file_menu)
        self.add_cascade(label="Edit", menu=self.edit_menu)
        self.add_command(label="Open console", command=lambda: print("Open console."))

        # Helper function to change color on hover
    def on_enter(self, event, widget):
        widget.config(background=hover_bg_color, foreground=hover_fg_color)

    def on_leave(self, event, widget):
        widget.config(background=menu_bg_color, foreground=menu_fg_color)

    # Create individual menu items (as labels)
    def create_menu_item(self, parent, text, command):
        label = tk.Label(parent, text=text, bg=menu_bg_color, fg=menu_fg_color, padx=10, pady=5, font=("Helvetica", 12))
        label.pack(side=tk.LEFT, padx=5)
        label.bind("<Button-1>", lambda e: command())  # Bind left-click to the menu action
        label.bind("<Enter>", lambda e: on_enter(e, label))  # Change color on hover
        label.bind("<Leave>", lambda e: on_leave(e, label))  # Restore color when hover leaves
        return label


class CustomMenuBar(ttk.Frame):

    # Define the color scheme
    menu_bg_color = "#2C3E50"  # Background color for the menu
    menu_fg_color = "#ECF0F1"  # Text color
    hover_bg_color = "#2980B9"  # Hover background color
    hover_fg_color = "#FFFFFF"  # Hover text color


    def __init__(self,
                 parent: Misc) -> "CustomMenuBar":
        # Define the custom menu bar frame
        super().__init__(parent, bootstyle="secondary", padding=(5, 2))  # Colored frame for the menu bar
        #menu_bar.pack(side=tk.TOP, fill=tk.X)
         # Create the "File" menu
        self.file_menu = self.create_menu_item(self, "File", lambda: self.menu_action("File"))
        self.edit_menu = self.create_menu_item(self, "Edit", lambda: self.menu_action("Edit"))
        self.view_menu = self.create_menu_item(self, "View", lambda: self.menu_action("View"))
        self.help_menu = self.create_menu_item(self, "Help", lambda: self.menu_action("Help"))
        self.open_console = self.create_menu_item(self, "Console", parent.side_bar_menu.animate)

    # Function to handle menu item clicks
    def menu_action(self, action_name: str) -> None:
        print(f"{action_name} selected")

    # Helper function to change color on hover
    def on_enter(self, event: Event, widget: Misc) -> None:
        widget.config(background=self.hover_bg_color, foreground=self.hover_fg_color)

    def on_leave(self, event: Event, widget: Misc) -> None:
        widget.config(background=self.menu_bg_color, foreground=self.menu_fg_color)

    # Create individual menu items (as labels)
    def create_menu_item(self, 
                         parent: Misc, 
                         text: str, 
                         command: Callable) -> tk.Label:
        label = tk.Label(parent, text=text, bg=self.menu_bg_color, fg=self.menu_fg_color, padx=10, pady=5, font=("Helvetica", 12))
        label.pack(side=tk.LEFT, padx=5)
        label.bind("<Button-1>", lambda e: command())  # Bind left-click to the menu action
        label.bind("<Enter>", lambda e: self.on_enter(e, label))  # Change color on hover
        label.bind("<Leave>", lambda e: self.on_leave(e, label))  # Restore color when hover leaves
        return label


class CustomMenuBarTest(ttk.Frame):

    def __init__(self,
                 parent: Misc) -> "CustomMenuBar":
        # Function to handle menu actions

        # Create a custom menu bar using a frame to control its color
        super().__init__(parent, bootstyle="dark", padding=(5, 5))
        self.pack(side=tk.TOP, fill=tk.X)

        # Create the actual Tkinter Menu (inside the frame)
        self.menu_bar = tk.Menu(self, tearoff=0)

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