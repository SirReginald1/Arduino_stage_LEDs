#from serial import Serial
from tkinter import *
import ttkbootstrap as ttk
from typing import Tuple
from custom_widgets import SlidePanel
from controller_interface import ControllerInterface
from animation_tabs.strob_tab import StrobeArrayFrame
from animation_tabs.sparkle_tab import SparkleArrayFrame
from animation_tabs.shooting_star_tab import ShootingStarArrayFrame
from animation_tabs.twinkle_pixel_tab import TwinklePixelArrayFrame
from animation_tabs.zip_tab import ZipArrayFrame
from animation_tabs.rainbow_circle import RainbowCircleArrayFrame
from animation_tabs.fade_tab import FadeArrayFrame
from animation_tabs.fire_tab import FireArrayFrame
from animation_tabs.volume_bar_tab import VolumeBarArrayFrame
from animation_tabs.tab import Tab
from console import Console


label_font = ("Helvetica", 18)

class MainWindow(ttk.Window):
    """"""

    tab_font: Tuple[str, int] = ("Helvetica", 12)
    """Tab Font. Tuple containing (font_style, font_size)"""

    def __init__(self, 
                 size: Tuple[int], 
                 nb_of_arrays: int):
        """"""
        super().__init__(themename="superhero")
        #self.menu_bar = Menu(self)
        #self.menu_test = Menu(self.menu_bar, tearoff=0)
        #self.menu_test.add_command(label="test", command=lambda : 10)
        #self.config(menu=self.menu_bar)

        self.controller_interface = ControllerInterface()

        self.title("The title")
        #self.iconbitmap()
        self.geometry(f"{size[0]}x{size[1]}")

        self.tab_pane = ttk.Notebook(self, bootstyle="dark")#, width=size[0]-20, height=size[1]-20)
        self.tab_pane.pack(pady=20, padx=20, expand=True, fill="both")
        self.tabs = []

        ###################################################
        ###################### TABS #######################
        ###################################################

        ##################### Strobe ######################
        self.strobe_tab = Tab(self.tab_pane,
                              self.controller_interface,
                              nb_of_arrays,
                              StrobeArrayFrame)
        self.tab_pane.add(self.strobe_tab, text="Strobe")
        self.tabs.append(self.strobe_tab)

        ##################### Sparkle ######################
        self.sparkle_tab = Tab(self.tab_pane,
                               self.controller_interface,
                               nb_of_arrays,
                               SparkleArrayFrame)
        self.tab_pane.add(self.sparkle_tab, text="Sparkle")
        self.tabs.append(self.sparkle_tab)

        ##################### Shooting star ######################
        self.shooting_star_tab = Tab(self.tab_pane,
                                     self.controller_interface,
                                     nb_of_arrays,
                                     ShootingStarArrayFrame)
        self.tab_pane.add(self.shooting_star_tab, text="Shooting Star")
        self.tabs.append(self.shooting_star_tab)

        ##################### Twinkle Pixel ######################
        self.twinkle_pixel_tab = Tab(self.tab_pane,
                                     self.controller_interface,
                                     nb_of_arrays,
                                     TwinklePixelArrayFrame)
        self.tab_pane.add(self.twinkle_pixel_tab, text="Twinkle Pixel")
        self.tabs.append(self.twinkle_pixel_tab)

        ##################### Zip ######################
        self.zip_tab = Tab(self.tab_pane,
                           self.controller_interface,
                           nb_of_arrays,
                           ZipArrayFrame)
        self.tab_pane.add(self.zip_tab, text="Zip")
        self.tabs.append(self.zip_tab)

        ##################### Rainbow Circle ######################
        self.rainbow_circle = Tab(self.tab_pane,
                                  self.controller_interface,
                                  nb_of_arrays,
                                  RainbowCircleArrayFrame)
        self.tab_pane.add(self.rainbow_circle, text="Rainbow Circle")
        self.tabs.append(self.rainbow_circle)
        
        ##################### Fade ######################
        self.fade_tab = Tab(self.tab_pane,
                               self.controller_interface,
                               nb_of_arrays,
                               FadeArrayFrame)
        self.tab_pane.add(self.fade_tab, text="Fade")
        self.tabs.append(self.fade_tab)

        ##################### Fire ######################
        self.fire_tab = Tab(self.tab_pane,
                               self.controller_interface,
                               nb_of_arrays,
                               FireArrayFrame)
        self.tab_pane.add(self.fire_tab, text="Fire")
        self.tabs.append(self.fire_tab)

        ##################### Volume Bar ######################
        self.volume_bar_tab = Tab(self.tab_pane,
                               self.controller_interface,
                               nb_of_arrays,
                               VolumeBarArrayFrame)
        self.tab_pane.add(self.volume_bar_tab, text="Volume Bar")
        self.tabs.append(self.volume_bar_tab)

        # Setting tab font
        ttk.Style().configure("TNotebook.Tab", font=self.tab_font, anchor=CENTER)

        ##################### Side Panel ######################
        self.side_bar_menu = SlidePanel(self, 1.0, 0.7, self.controller_interface)
        self.button = Button(self.tab_pane, text = 'toggle sidebar', command = self.side_bar_menu.animate)
        self.button.pack(pady=0, padx=20, expand=False, fill="none")

        ##################### Backup Console #####################
        #self.txtin_console = Entry(self, textvariable=StringVar(value=str("")))
        #self.txtin_console.place(x=1000, y=200)
        #self.txtin_console.bind("<Return>", self.enter_key_press)
        #self.console = Console(self, self.controller_interface)
        #self.console.place(x=1000, y=200)


    #def enter_key_press(self, message: str):
    #    """"""
    #    self.arduino_interface.send_message(self.txtin_console.get())
    #    self.txtin_console.delete(0,END)

    def update_all_animations(self) -> None:
        """Updates all parameter values for all animations."""
        for tab in self.tabs:
            tab.set_all_arrays()

        # To get currently selected
        # noteBook.index("current")
        # nb.select()
        # '.4299842480.4300630784'
        # nb.index(nb.select())

    def update_console(self):
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