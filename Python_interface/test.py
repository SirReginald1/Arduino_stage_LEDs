import serial
import time
import ttkbootstrap as ttk

# Connecting to the arduino
# Other ports are:
# /dev/cu.usbmodem14101
# /dev/ttyACM0
try:
    arduino = serial.Serial(port='COM3', baudrate=9600, timeout=0)
except:
    print("Connection error! Check that that the port is correct.")

def send_str(string: str) -> None:
    """Sends the givent string to the arduino.
    ### Args:
    - string (str): The string to be passed to the arduino.
    """
    arduino.write(bytes(string,   'utf-8'))

def read_str() -> str:
    """Reads the string sent the arduino.
    ### Returns:
    - str: The string being sent from the arduino.
    """
    return str(arduino.readline(),"utf-8").strip("'\r\n")[2:]


while True:
    num = input("Enter a number: ")
    send_str(num)
    value = read_str()
    print(value)
