from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from typing import Any, List
import tkinter as tk
from time import time, sleep
from tkinter import Frame,Label,Entry,Button
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from controller_interface import ControllerInterface
from threading import Thread

class PlotWindow(Frame):

    def __init__(self, 
                 controller: ControllerInterface,
                 master: Any = None) -> None:
        Frame.__init__(self, master)
        self.master: Any = master

        self.controller: ControllerInterface = controller

        self.start_time: float = time()

        self.x_values: List[int] = []

        self.y_values: List[int] = []

        self.x_lim: List[int] = [0,100]

        self.y_lim: List[int] = [-10000, 10000]

        self.x_lim_step: int = 100

        def animate(i: int):
           """Function called by """
           #self.line.set_ydata(self.yaxis[i])  # update the data
           self.line.set_data(self.x_values, self.y_values)
           self.ax.set_xlim(self.x_lim[0], self.x_lim[1])
           #print(f"len x: {len(self.xaxis)}")
           #print(f"len y: {len(self.yaxis)}")
           return self.line,

        self.master.title("Controler readings")
        self.pack(fill='both', expand=1)     

#Create the controls, note use of grid

        self.buttonClear = Button(self,text="Clear",command=self.Clear,width=12)
        self.buttonClear.grid(row=2,column=2)

#        self.buttonClear.bind(lambda e:self.Plot)
        self.buttonClear.bind(lambda e:self.Clear)

        tk.Label(self,text="SHM Simulation").grid(column=0, row=3)

        self.fig = plt.Figure()

        #self.x = np.arange(0, 2*np.pi, 0.01)        # x-array

        #self.xaxis = range(629)

        self.ax = self.fig.add_subplot(111)
        self.line, = self.ax.plot(self.x_values, self.y_values)        
        self.ax.set_xlim(self.x_lim[0], self.x_lim[1])
        self.ax.set_ylim(self.y_lim[0], self.y_lim[1])

        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().grid(column=0,row=4)

        self.animation = animation.FuncAnimation(self.fig, animate, np.arange(1, 1000000000), interval=1/1000, blit=False)

    def read_measurment(self) -> None:
        """Function used in own thead to read values from the controller and append them to the xaxis array."""
        while True:
           try:
               reading = self.controller.read_measurments()
               if reading:
                   self.y_values.append(reading)
                   self.x_values.append(time()-self.start_time)
               #print(f"Read: {reading}, time: {time()-self.start_time}")
           except:
              break
           
    def check_gaph_changes(self) -> None:
        """Function used in thread to update graph limits."""
        while True:
           if len(self.x_values) > 0 and self.x_values[-1] > self.x_lim[1]:
               self.x_lim[0] = self.x_lim[1]
               self.x_lim[1] = self.x_lim[1] + self.x_lim_step
           sleep(0.1)

    def Clear(self):
        self.x_values = []
        self.y_values = []
        self.x_lim = [0, 100]
        self.y_lim = [-10000, 10000]

controller = ControllerInterface(port="COM7", baudrate=115200)

root = tk.Tk()
root.geometry("1200x800")
app = PlotWindow(root, controller)
tread_read_measurment = Thread(target=app.read_measurment)
tread_read_measurment.daemon = True
tread_read_measurment.start()
tread_read_check_graph = Thread(target=app.check_gaph_changes)
tread_read_check_graph.daemon = True
tread_read_check_graph.start()
tk.mainloop()
