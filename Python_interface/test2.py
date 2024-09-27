import tkinter as tk
from tkinter import ttk

class HorizontalScrollableFrame(ttk.Frame):
    def __init__(self, parent, labels=[], entries=[], checkboxes=[], *args, **kwargs):
        super().__init__(parent, *args, **kwargs)

        # Create a canvas and a scrollbar
        self.canvas = tk.Canvas(self, borderwidth=0)
        self.scrollbar = ttk.Scrollbar(self, orient="horizontal", command=self.canvas.xview)
        self.scrollable_frame = ttk.Frame(self.canvas)

        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )

        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.canvas.configure(xscrollcommand=self.scrollbar.set)

        # Pack the canvas and scrollbar
        self.canvas.pack(side="top", fill="both", expand=True)
        self.scrollbar.pack(side="bottom", fill="x")

        # Add widgets dynamically based on the arrays
        self.label_vars = []
        self.entry_vars = []
        self.checkbox_vars = []

        # Horizontal layout for labels, entries, and checkboxes
        for i, label_text in enumerate(labels):
            label_var = tk.StringVar(value=label_text)
            label = ttk.Label(self.scrollable_frame, textvariable=label_var)
            label.grid(row=0, column=i, padx=10, pady=5)
            self.label_vars.append(label_var)

        for i, entry_default in enumerate(entries):
            entry_var = tk.StringVar(value=entry_default)
            entry = ttk.Entry(self.scrollable_frame, textvariable=entry_var)
            entry.grid(row=1, column=i, padx=10, pady=5)
            self.entry_vars.append(entry_var)

        for i, checkbox_text in enumerate(checkboxes):
            checkbox_var = tk.BooleanVar()
            checkbutton = ttk.Checkbutton(self.scrollable_frame, text=checkbox_text, variable=checkbox_var)
            checkbutton.grid(row=2, column=i, padx=10, pady=5)
            self.checkbox_vars.append(checkbox_var)

    def get_values(self):
        """
        Retrieve the current values of the widgets.
        Returns a dictionary with the values of labels, entries, and checkboxes.
        """
        label_values = [var.get() for var in self.label_vars]
        entry_values = [var.get() for var in self.entry_vars]
        checkbox_values = [var.get() for var in self.checkbox_vars]

        return {
            'labels': label_values,
            'entries': entry_values,
            'checkboxes': checkbox_values
        }

# Main application window
root = tk.Tk()
root.title("Horizontal Scrollable Frame Example")

# Define some example data arrays
labels = [f"Label {i+1}" for i in range(10)]
entries = [f"Entry {i+1}" for i in range(10)]
checkboxes = [f"Checkbox {i+1}" for i in range(10)]

# Create a HorizontalScrollableFrame with the arrays
scrollable_frame = HorizontalScrollableFrame(root, labels=labels, entries=entries, checkboxes=checkboxes)
scrollable_frame.pack(fill="both", expand=True, padx=20, pady=20)

# Function to display the values of the widgets
def show_values():
    values = scrollable_frame.get_values()
    print("Labels:", values['labels'])
    print("Entries:", values['entries'])
    print("Checkboxes:", values['checkboxes'])

# Button to trigger value display
show_values_button = ttk.Button(root, text="Show Values", command=show_values)
show_values_button.pack(pady=10)

# Start the main Tkinter event loop
root.mainloop()
