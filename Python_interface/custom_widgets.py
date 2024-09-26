from typing import Callable
import customtkinter as ctk
import tkinter as tk
from random import choice
from controller_interface import ControllerInterface
from console import Console

class SlidePanel(ctk.CTkFrame):
	def __init__(self, parent, start_pos, end_pos, controller_interface: ControllerInterface):
		super().__init__(master = parent)
		# Console attributes
		self.console_intput_str = ""
		"""The last string that was inputed to the arduino."""
		self.console_output_str = ""
		"""The string that is outputed from the arduino."""
		self.controller_interface = controller_interface

		# general attributes 
		self.start_pos = start_pos + 0.04
		self.end_pos = end_pos - 0.03
		self.width = abs(start_pos - end_pos)

		# animation logic
		self.pos = self.start_pos
		self.in_start_pos = True

		# layout
		self.place(relx = self.start_pos, rely = 0.05, relwidth = self.width, relheight = 0.9)

		self.mic_on_lab = ctk.CTkLabel(self, text = f'Microphone: off')
		self.mic_on_lab.pack(expand = True, fill = 'both', padx = 2, pady = 10)
		self.label2 = ctk.CTkLabel(self, text = 'Label 2')
		self.label2.pack(expand = True, fill = 'both', padx = 2, pady = 10)
		self.button1 = ctk.CTkButton(self, text = 'Turn on mic', corner_radius = 0, 
							   		 command=lambda: controller_interface.send_message("@1<>"))
		self.button1.pack(expand = True, fill = 'both', pady = 10)
		self.console = Console(self,self.controller_interface)
		self.console.pack(expand = True, fill = 'both', padx = 2, pady = 10)

		#self.create_widgets()
		
	def create_widgets(self):
		ctk.CTkLabel(self, text = 'Label 1').pack(expand = True, fill = 'both', padx = 2, pady = 10)
		ctk.CTkLabel(self, text = 'Label 2').pack(expand = True, fill = 'both', padx = 2, pady = 10)
		ctk.CTkButton(self, text = 'Turn on mic', corner_radius = 0).pack(expand = True, fill = 'both', pady = 10)

		#self.console_out = ctk.CTkTextbox(self, fg_color = ('#dbdbdb','#2b2b2b'))
		#self.console_out.insert(tk.INSERT,text=self.console_output_str)
		#self.console_out.pack(expand = True, fill = 'both', pady = 10)
		#self.console_in = tk.Entry(self, textvariable=tk.StringVar(value=str(self.console_intput_str)))
		#self.console_in.bind("<Return>", self.enter_key_event)
		#self.console_in.pack(expand = True, fill = 'both', pady = 10)

	def animate(self):
		if self.in_start_pos:
			self.animate_forward()
		else:
			self.animate_backwards()

	def animate_forward(self):
		if self.pos > self.end_pos:
			self.pos -= 0.008
			self.place(relx = self.pos, rely = 0.05, relwidth = self.width, relheight = 0.9)
			self.after(10, self.animate_forward)
		else:
			self.in_start_pos = False

	def animate_backwards(self):
		if self.pos < self.start_pos:
			self.pos += 0.008
			self.place(relx = self.pos, rely = 0.05, relwidth = self.width, relheight = 0.9)
			self.after(10, self.animate_backwards)
		else:
			self.in_start_pos = True

	def update_console_window(self, mesage: str) -> None:
		"""Updates the console window."""
		self.console_out.insert(tk.INSERT, f"{mesage}\n")

	def enter_key_event(self, event: tk.Event):
		""""""
		self.console_intput_str = self.console_in.get()
		self.controller_interface.send_message(self.console_intput_str)
		self.console_in.delete(0,tk.END)

class Switch(tk.Frame):
	"""Switch widget."""
	
	def __init__(self, 
			  	 parent: tk.Misc,
				 command: Callable,
				 text: str) -> None:
		super().__init__(parent)
		# Create Switch
		self.my_switch = ctk.CTkSwitch(self, text=text, command=command,
			variable=ctk.StringVar(value="on"), onvalue=1, offvalue=0,
			#width=200,
			#height=100,
			switch_width=60,
			switch_height=25,
			#corner_radius=10,
			border_color="orange",
			border_width=5,
			fg_color="red",
			progress_color="green",
			button_color="pink",
			button_hover_color="yellow",
			font=("Helvetica", 24),
			text_color="blue",
			state="normal",
		)
		self.my_switch.pack()

	


# exercise
# 1. animate the button and move it to the right side of the window
# 2. update the panel so it move in from the right
"""
def move_btn():
	global button_x
	button_x += 0.001
	button.place(relx = button_x, rely = 0.5, anchor = 'center')
	
	if button_x < 0.9:
		window.after(10, move_btn)

	# configure 
	# colors = ['red', 'yellow', 'pink', 'green', 'blue', 'black', 'white']
	# color = choice(colors)
	# button.configure(fg_color = color)

def infinite_print():
	global button_x
	button_x += 0.5
	if button_x < 10:
		print('infinite')
		print(button_x)
		window.after(100, infinite_print)
"""
"""
# window 
window = ctk.CTk()
window.title('Animated Widgets')
window.geometry('600x400')

# animated widget
animated_panel = SlidePanel(window, 1.0, 0.7)
ctk.CTkLabel(animated_panel, text = 'Label 1').pack(expand = True, fill = 'both', padx = 2, pady = 10)
ctk.CTkLabel(animated_panel, text = 'Label 2').pack(expand = True, fill = 'both', padx = 2, pady = 10)
ctk.CTkButton(animated_panel, text = 'Button', corner_radius = 0).pack(expand = True, fill = 'both', pady = 10)
ctk.CTkTextbox(animated_panel, fg_color = ('#dbdbdb','#2b2b2b')).pack(expand = True, fill = 'both', pady = 10)

# button
button_x = 0.5
button = ctk.CTkButton(window, text = 'toggle sidebar', command = animated_panel.animate)
button.place(relx = button_x, rely = 0.5, anchor = 'center')

# run
window.mainloop()
"""