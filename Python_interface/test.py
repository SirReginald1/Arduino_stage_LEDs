import tkinter as tk
import ttkbootstrap as ttk
from typing import Callable
from ttkbootstrap.constants import *

class CustomMenuBar(ttk.Frame):

    # Define the color scheme
    menu_bg_color = "#2C3E50"  # Background color for the menu
    menu_fg_color = "#ECF0F1"  # Text color
    hover_bg_color = "#2980B9"  # Hover background color
    hover_fg_color = "#FFFFFF"  # Hover text color

    def __init__(self, parent: tk.Misc) -> None:
        # Define the custom menu bar frame
        super().__init__(parent, bootstyle="secondary", padding=(5, 2))
        self.pack(side=tk.TOP, fill=tk.X)

        # Create dropdown menus for each label
        self.file_menu = self.create_menu_item(self, "File", self.create_file_menu)
        self.edit_menu = self.create_menu_item(self, "Edit", self.create_edit_menu)
        self.view_menu = self.create_menu_item(self, "View", self.create_view_menu)
        self.help_menu = self.create_menu_item(self, "Help", self.create_help_menu)

    # Function to handle menu item clicks
    def menu_action(self, action_name: str) -> None:
        print(f"{action_name} selected")

    # Helper function to change color on hover
    def on_enter(self, event: tk.Event, widget: tk.Misc) -> None:
        widget.config(background=self.hover_bg_color, foreground=self.hover_fg_color)

    def on_leave(self, event: tk.Event, widget: tk.Misc) -> None:
        widget.config(background=self.menu_bg_color, foreground=self.menu_fg_color)

    # Create individual menu items (as labels) with dropdown functionality
    def create_menu_item(self, parent: tk.Misc, text: str, create_menu: Callable) -> tk.Label:
        label = tk.Label(parent, text=text, bg=self.menu_bg_color, fg=self.menu_fg_color, padx=10, pady=5, font=("Helvetica", 12))
        label.pack(side=tk.LEFT, padx=5)

        # Bind hover and click events
        label.bind("<Enter>", lambda e: self.on_enter(e, label))  # Change color on hover
        label.bind("<Leave>", lambda e: self.on_leave(e, label))  # Restore color when hover leaves
        label.bind("<Button-1>", lambda e: self.show_menu(label, create_menu()))  # Show the dropdown menu when clicked
        
        return label

    # Show the dropdown menu
    def show_menu(self, widget: tk.Widget, menu: tk.Menu) -> None:
        x, y, width, height = widget.bbox()
        menu.post(widget.winfo_rootx(), widget.winfo_rooty() + height)

    # Create File menu dropdown
    def create_file_menu(self) -> tk.Menu:
        menu = tk.Menu(self, tearoff=0)
        menu.add_command(label="New", command=lambda: self.menu_action("New"))
        menu.add_command(label="Open", command=lambda: self.menu_action("Open"))
        menu.add_separator()
        menu.add_command(label="Exit", command=lambda: self.menu_action("Exit"))
        return menu

    # Create Edit menu dropdown
    def create_edit_menu(self) -> tk.Menu:
        menu = tk.Menu(self, tearoff=0)
        menu.add_command(label="Undo", command=lambda: self.menu_action("Undo"))
        menu.add_command(label="Redo", command=lambda: self.menu_action("Redo"))
        return menu

    # Create View menu dropdown
    def create_view_menu(self) -> tk.Menu:
        menu = tk.Menu(self, tearoff=0)
        menu.add_command(label="Zoom In", command=lambda: self.menu_action("Zoom In"))
        menu.add_command(label="Zoom Out", command=lambda: self.menu_action("Zoom Out"))
        return menu

    # Create Help menu dropdown
    def create_help_menu(self) -> tk.Menu:
        menu = tk.Menu(self, tearoff=0)
        menu.add_command(label="About", command=lambda: self.menu_action("About"))
        return menu


# Test the CustomMenuBar
if __name__ == "__main__":
    root = ttk.Window(themename="superhero")
    root.title("Custom Menu Bar with Dropdown Menus")

    menu_bar = CustomMenuBar(root)
    root.mainloop()