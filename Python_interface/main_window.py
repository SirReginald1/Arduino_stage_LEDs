from tkinter import *
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from typing import Tuple, List
from menu_bar import CustomMenuBar
from custom_widgets import SlidePanel
from led_array_frame import LedArrayFrame
from controller_interface import ControllerInterface


label_font = ("Helvetica", 18)

class MainWindow(ttk.Window):
    """"""

    tab_font: Tuple[str, int] = ("Helvetica", 12)
    """Tab Font. Tuple containing (font_style, font_size)"""

    array_frame_label_font: Tuple[str, int] = ("Helvetica", 18)
    """Array frame label Font. Tuple containing (font_style, font_size)"""

    def __init__(self,
                 app: "App",
                 size: Tuple[int],
                 nb_of_arrays: int) -> "MainWindow":
        """Constructor for the main interface window.
        
        ### Args:
            - app (App): The root application that is using this window.
            - size (tuple): A tuple of size 2 that indicates the dimensions
            of the window on startup.
            - nb_of_arrays (int): The number of independent LED arrays that
            are to be controlled by the app.
        """
        super().__init__(themename="superhero")
        self.app = app
        self.controller_interface: ControllerInterface = app.controller_interface
        self.title("LED Controller")
        #self.iconbitmap()
        self.geometry(f"{size[0]}x{size[1]}")

         ##################### Side Panel ######################
        self.side_bar_menu: SlidePanel = SlidePanel(self,
                                                    1.0,
                                                    0.7,
                                                    self.controller_interface)

        self.menu_bar: CustomMenuBar = CustomMenuBar(self)
        self.menu_bar.pack(side=TOP,
                           fill=X)

        self.scrole_canvas = Canvas(self)
                                        
        self.scrole_canvas.pack(pady=20, side="left", expand=True)

        self.scrole_bar = ttk.Scrollbar(self.scrole_canvas,
                                        orient='vertical',
                                        command=self.scrole_canvas.yview
                                        )
        self.scrole_canvas.configure(yscrollcommand=self.scrole_bar.set)
        self.scrole_bar.pack(side="right",
                             fill=Y)

        ##################### Scrolable canvas ################
        ## === Create Scrollable Canvas ===
        #self.canvas = Canvas(self)
        #self.scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.canvas.yview)
        #self.canvas.configure(yscrollcommand=self.scrollbar.set)
#
        #self.scrollbar.pack(side="right", fill="y")
        #self.canvas.pack(side="left", fill="both", expand=True)
#
        ## === Scrollable Frame inside Canvas ===
        #self.scrollable_frame = ttk.Frame(self.canvas)
        #self.canvas_frame = self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
#
        ## Update scrollregion when scrollable_frame changes size
        #self.scrollable_frame.bind("<Configure>", lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all")))
        #self.canvas.bind("<Configure>", self._resize_canvas)
#
        ## Optional: mousewheel support
        #self.scrollable_frame.bind("<Enter>", self._bind_mousewheel)
        #self.scrollable_frame.bind("<Leave>", self._unbind_mousewheel)
#
       
        #self.side_bar_menu.lift()

        #menu_bar = MenuBar(root)
        

        self.array_frames: List[LedArrayFrame] = []
        """A list that contains the panel menu object for each of the array menu panels."""

        # Create number of frames equal to the number of arrays, each with 2 sections, and stack them at the top of the window
        for i in range(nb_of_arrays):
            self.array_frames.append(LedArrayFrame(self.scrole_canvas,
                                                   i,
                                                   size[1]))
            self.array_frames[i].pack(fill=X,
                                      side="top",
                                      pady = 10
                                      )  # Stack frames at the top
            self.side_bar_menu.lift(self.array_frames[i])

    #def enter_key_press(self, message: str):
    #    """"""
    #    self.arduino_interface.send_message(self.txtin_console.get())
    #    self.txtin_console.delete(0,END)

    #def _resize_canvas(self, event):
    #    self.canvas.itemconfig(self.canvas_frame, width=event.width)
    #
    #def _bind_mousewheel(self, event):
    #    self.canvas.bind_all("<MouseWheel>", self._on_mousewheel)
    #
    #def _unbind_mousewheel(self, event):
    #    self.canvas.unbind_all("<MouseWheel>")
    #
    #def _on_mousewheel(self, event):
    #    self.canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")

    def update_all_animations(self) -> None:
        """Updates all parameter values for all animations."""
        for array_frame in self.array_frames:
            #if type(array_frame.param_frame.winfo_children()[0]) != 'tkinter.ttk.Button': # Change this if update all bugs
            try:
                array_frame.var_frame.content.send_sync_message()
            except:
                print("MainWinddow.update_all_animations: send_sync_message function not found!")
                pass
        for array_frame in self.array_frames:
            #if type(array_frame.param_frame.winfo_children()[0]) != 'tkinter.ttk.Button': # Change this if update all bugs
            try:
                array_frame.param_frame.winfo_children()[0].set_animations()
            except:
                pass
            #print(type(array_frame))
            #for widget in array_frame.param_frame.winfo_children():
            #    print(type(widget))
            #print(type(array_frame.param_frame.winfo_children()[0]))

        # To get currently selected
        # noteBook.index("current")
        # nb.select()
        # '.4299842480.4300630784'
        # nb.index(nb.select())

    def update_console(self) -> None:
        """Methode that is periodically called to update the console window."""
        #print("Calling update!")
        #self.side_bar_menu.update_console_window(self.arduino_interface.read_str())
        print("Calling update!")
        self.console.update_console(self.controller_interface.read_str())
        #self.after(20, self.update_console)
        

#if __name__ == '__main__':
#    root = Tk()
#    root.main_panel = MainPanel((2500,1300), 4)
#    root.main_panel.update()
#    #root.after(2000, print("root 2secs"))
#    #root.mainloop()
#    #main_panel.after(2000, main_panel.update_console)
#    #main_panel.update_console()
#    root.mainloop()