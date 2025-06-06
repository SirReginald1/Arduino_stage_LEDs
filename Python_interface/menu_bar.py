from typing import Callable
import tkinter as tk
from tkinter import Button, Entry, StringVar
import ttkbootstrap as ttk
from tkinter import Misc, Event


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
        self.file_menu = self.create_dropdown_menu_item(self, "File", self.create_file_menu)
        self.edit_menu = self.create_menu_item(self, "Edit", lambda: self.menu_action("Edit"))
        self.view_menu = self.create_menu_item(self, "View", lambda: self.menu_action("View"))
        self.help_menu = self.create_menu_item(self, "Help", lambda: self.menu_action("Help"))
        self.connection_menu = self.create_dropdown_menu_item(self, "Connection", self.create_connection_menu)
        self.open_console = self.create_menu_item(self, "Console", parent.side_bar_menu.animate)
        self.update_all = self.create_menu_item(self, "Update all", lambda: self.master.update_all_animations())
        self.server = self.create_menu_item(self, "Server", self.open_server_window)

        self.global_brightness_slider_memory: int = 0
        """The value used to make the slider only send commands when it switches integer."""

        # Create a slider (scale) at the end of the menu bar
        self.brightness_slider: ttk.Scale = ttk.Scale(self, from_= 0, to = 60, value=255, length=800, orient=tk.HORIZONTAL, bootstyle="primary", command=self.update_brightness)
        self.brightness_slider.pack(side=tk.RIGHT, padx=20, pady=5)

        # Create a label to display the slider's value
        self.slider_value_label: ttk.Label = ttk.Label(self, text="255", bootstyle="secondary", padding=(5, 2))
        self.slider_value_label.pack(side=tk.RIGHT, padx=10, pady=5)

    # Update the label with the current value of the slider
    def update_brightness(self, value: str) -> None:
        #print(f"type: {type(value)}, value: {value}")
        converted_value: int = int(float(value))  
        if converted_value >= 55:
            converted_value = min(converted_value*5, 255)
        if self.global_brightness_slider_memory != converted_value:
            self.slider_value_label.config(text=converted_value)
            self.master.controller_interface.send_message(f"@2<-2,{converted_value}>")
            self.global_brightness_slider_memory = converted_value

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
    
    # Create individual menu items (as labels) with dropdown functionality
    def create_dropdown_menu_item(self, parent: tk.Misc, text: str, create_menu: Callable) -> tk.Label:
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
    
    def create_connection_menu(self) -> tk.Menu:
        """Create connection dropdown menu.
        
        ### Returns:
            - tk.Menu: The connection dropdown menu
        """
        menu = tk.Menu(self, tearoff=0)
        menu.add_command(label="Set connection options", command=lambda: self.open_connection_settings_window())
        menu.add_command(label="Re-connect", command=lambda: self.master.controller_interface.try_to_connect())
        #menu.add_separator()
        #menu.add_command(label="Exit", command=lambda: self.menu_action("Exit"))
        return menu

    def open_connection_settings_window(self) -> None:
        """Function used to open the window connect to set Serial baudrate,
        COM port and start a new connection.
        """
        new_window = ttk.Toplevel(self.master)  # Create a new top-level window
        new_window.title("Serial connection settings")
        new_window.geometry("600x400")

        lab_port = ttk.Label(new_window, text="Port", font=("Helvetica", 16))
        lab_port.pack(pady=20)
        text_in_port = Entry(new_window, textvariable=StringVar(value=str(self.master.controller_interface.port)))
        text_in_port.pack(pady=20)

        # Add some content to the new window
        lab_baudrate = ttk.Label(new_window, text="Baudrate", font=("Helvetica", 16))
        lab_baudrate.pack(pady=20)
        text_in_baudrate = Entry(new_window, textvariable=StringVar(value=str(self.master.controller_interface.baudrate)))
        text_in_baudrate.pack(pady=20)

        def close_window() -> None:
            self.master.controller_interface.port = text_in_port.get()
            self.master.controller_interface.baudrate = int(text_in_baudrate.get())
            #print(f"The interface port: {self.master.controller_interface.port}")
            #print(f"The interface baudrate: {self.master.controller_interface.baudrate}")
            new_window.destroy()

        close_button = ttk.Button(new_window, text="Set variables", command=close_window)
        close_button.pack(pady=10)

    def open_server_window(self) -> None:
        """Opens the window used to start, stop and set parameters for the TCP server."""
        new_window = ttk.Toplevel(self.master)  # Create a new top-level window
        new_window.title("TCP Server settings")
        new_window.geometry("600x400")

        lab_address = ttk.Label(new_window, text="Address", font=("Helvetica", 16))
        lab_address.pack(pady=20)
        text_in_address = Entry(new_window, textvariable=StringVar(value=str(self.master.app.server.host)))
        text_in_address.pack(pady=20)

        # Add some content to the new window
        lab_port = ttk.Label(new_window, text="Port", font=("Helvetica", 16))
        lab_port.pack(pady=20)
        text_in_port = Entry(new_window, textvariable=StringVar(value=str(self.master.app.server.port)))
        text_in_port.pack(pady=20)

        # TODO: Add proper used feedback on button press.
        def set_address_and_port() -> None:
            if self.master.app.server.set_address_and_port(text_in_address.get(), int(text_in_port.get())) == 1:
                print("Can't start server as server is already running")
            #print(f"The server address: {self.master.app.server.host}")
            #print(f"The server port: {self.master.app.server.port}")
            #new_window.destroy()

        #def switch_server_on_off(callable_idx: int) -> int:
        #    server_on_off_btn_cmd[server_on_off_btn_txt_idx]()
        #    return (callable_idx + 1) % 2

        set_parameters_button = ttk.Button(new_window, text="Set variables", command=set_address_and_port)
        set_parameters_button.pack(pady=10)
        # TODO: Make a single button that toggles server on and off
        server_on_off_btn_text = ["Open server", "Close server"]
        #server_on_off_btn_cmd = [self.master.app.server.start,
        #                          self.master.app.server.stop]
        #server_on_off_btn_txt_idx = 0
        server_on_button = ttk.Button(new_window,
                                          text=server_on_off_btn_text[0],
                                          command=self.master.app.server.start)
        server_on_button.pack(pady=10)
        server_off_button = ttk.Button(new_window,
                                          text=server_on_off_btn_text[1],
                                          command=self.master.app.server.stop)
        server_off_button.pack(pady=10)

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