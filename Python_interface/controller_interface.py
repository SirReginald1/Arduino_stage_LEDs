from typing import Union, Dict, Tuple, List, Callable
from serial import Serial, SerialException
import sys
import glob
from time import time
from struct import unpack


class ControllerInterface():
    """Class that deals with interfacing with the arduino."""
    
    init_values_strobe: List[Union[int, Tuple]] = [20, 55, (255, 255, 255)]
    """Parameter init values for the strobe animation. {time_on, time_off, (R, G, B)}"""

    init_values_zip: List[int] = [2,10,122,0,(0, 0, 255)]
    """Parameter init values for the zip animation."""

    init_values_twinkle_pixel: List[int] = [200,50,50,100,50]
    """Parameter init values for the pixel animation."""

    init_values_fire: List[int] = [50,50,25,1.]
    """Parameter init values for the fire animation. {flame_height, sparks, delay, intensity}"""

    init_values_shooting_star: List[int] = [(150, 0, 150), 20, 10, 2000, 1]
    """Parameter init values for the shooting star animation.
    {(R, G, B), tail_length, delay_duration, interval, direction}
    """

    init_values_sparkle: List[int] = [(200, 0, 100), 0]
    """Parameter init values for the sparkle animation."""

    init_values_fade: List[int] = [(255, 255, 255), 100]
    """Parameter init values for the fade animation."""

    init_values_rainbow_circle: List[int] = [0]
    """Parameter init values for the rainbow wheel animation. {delay}"""

    init_values_flash_to_beat: List[int] = [(255, 255, 255), 50, 80, 180]
    """Parameter init values for the flash to beat animation. {freq_bin_low, freq_bin_high, time_on}"""

    init_labels_strobe: List[str] = ["time_on", "time_off", "color"]
    """Parameter init labels for the strobe animation."""

    init_labels_zip: List[str] = ["size", "start", "end", "delay"]
    """Parameter init labels for the zip animation."""

    init_labels_twinkle_pixel: List[str] = ["color", "color_saturation", "pixel_volume", "fade_amount", "delay_duration"]
    """Parameter init labels for the twinkle pixel animation."""

    init_labels_fire: List[str] = ["flame_hight", "sparks", "delay", "intensity"]
    """Parameter init labels for the fire animation."""

    init_labels_shooting_star: List[str] = ["red", "green", "blue", "tail_lenght", "delay", "interval"]
    """Parameter init labels for the shooting star animation."""

    init_labels_sparkle: List[str] = ["red", "green", "blue", "delay"]
    """Parameter init labels for the sparkle animation."""

    init_labels_fade: List[str] = ["red", "green", "blue"]
    """Parameter init labels for the fade animation."""

    init_labels_rainbow_wheel: List[str] = ["delay"]
    """Parameter init labels for the rainbow wheel animation."""

    animation_codes = {"Rainbow circle" : 0,
                       "Fade" : 1,
                       "Sparkle" : 2, 
                       "Fire" : 3, 
                       "Shooting star" : 4, 
                       "Twinkle pixel" : 5, 
                       "Strobe" : 6, 
                       "Zip" : 7, 
                       "Flash to beat" : 8}
    """Dictionary of the codes that reference each animation in the arduino code."""
    
    animation_labels = ["zip", 
                       "strobe", 
                       "volume_bar", 
                       "twinkle_pixel", 
                       "shooting_star", 
                       "fire", 
                       "sparkle", 
                       "fade", 
                       "rainbow_circle"]
    """List of all the animation names."""

    NB_ARRAYS = 4

    # Send bytes representing the animation code
    # If bytes long then with parameters else just change animation

    def __init__(self, 
                 port: str = "COM7", 
                 baudrate: int = 115200, 
                 error_fun_call: Callable = None) -> "ControllerInterface":
        """Constructor for arduino interface.
        
        ### Args:
            - port (str, Optional): The port to which the interface will try to connect.
            - baudrate (int, Optional): The baudrate used to comunicate with the controller.
            - error_fun_call (Callable, Optional): The function that is called if the ControllerInterface is unable to make a serial connection or comunication.
            This function must take a string as it's first parameter.
            - use_smooth_coom (bool, Optional): Value indicating if the controller interface should use tchniques to make communication smoother.
              Such as automaticaly resending the last command if the controller interface receves a resend message from the controller.

        ### Returns:
            - "ControllerInterface": A new ControllerInterface object. 
        """ 
        
        self.port: str = port
        """The port to which the interface will try to connect."""

        self.baudrate: int = baudrate
        """The baudrate used to connect to the controller. (Comunication speed with the controler.)"""

        self.error_fun_call: Callable = error_fun_call
        """The function that is called if the ControllerInterface is unable to make a serial connection."""

        self.last_message_sent: str = ""
        """The last message sent by the controller interface"""

        self.last_message_receved: str = ""
        """The last message receved by the controller interface"""

        self.resend_message: bool = False
        """Boolean indicateing if interface should atempt to resend message."""

        self.nb_messages_resent: int = 0
        """Counts the nuber of times a message is resent so as to stop the resending of messages after a certain amount. 
        (Stops infinit loops if an unrecognised message is sent)
        """

        self.time_of_last_message_receved = float(0)
        """The time the last message was receved."""

        self.time_of_last_message_sent = float(0)
        """The time the last message was sent."""

        try:
            self.controller = Serial(port=self.port, baudrate=self.baudrate, timeout=1)
            """The arduino connection port."""
        except:
            #raise RuntimeError("Connection error! Check that that the port is correct!")
            self.controller = None
            if error_fun_call:
                error_fun_call("Connection error! Check that that the port is correct!")
            print("Connection error! Check that that the port is correct!")

        self.param_values_strobe: Dict[int, Tuple[int]] = {label : value for label, value in zip(self.init_labels_strobe, self.init_values_strobe)}
        """Parameter values for the strobe animation."""

        self.param_values_zip: Dict[int] = {label : value for label, value in zip(self.init_labels_zip, self.init_values_zip)}
        """Parameter values for the zip animation."""

        self.param_values_twinkle_pixel: Dict[int] = {label : value for label, value in zip(self.init_labels_twinkle_pixel, self.init_values_twinkle_pixel)}
        """Parameter values for the twinkle pixel animation."""

        self.param_values_fire: Dict[int, float] = {label : value for label, value in zip(self.init_labels_fire, self.init_values_fire)}
        """Parameter values for the fire animation."""

        self.param_values_shooting_star: Dict[int] = {label : value for label, value in zip(self.init_labels_shooting_star, self.init_values_shooting_star)}
        """Parameter values for the shooting star animation."""

        self.param_values_sparkle: Dict[int] = {label : value for label, value in zip(self.init_labels_sparkle, self.init_values_sparkle)}
        """Parameter values for the sparkle animation."""

        self.param_values_fade: Dict[int] = {label : value for label, value in zip(self.init_labels_fade, self.init_values_fade)}
        """Parameter values for the fade animation."""

        self.param_values_rainbow_circle: Dict[int] = {label : value for label, value in zip(self.init_labels_rainbow_wheel, self.init_values_rainbow_circle)}
        """Parameter values for the rainbow circle animation."""
 
        self.animation_var_array_dict: Dict[str, List[List[str]]] = {"Rainbow circle" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]],
                                                                     "Fade" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]],
                                                                     "Sparkle" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Fire" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Shooting star" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Twinkle pixel" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Strobe" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Zip" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]], 
                                                                     "Flash to beat" : [["PosCounter offset" for i in range(self.NB_ARRAYS)], 
                                                                                         ["0" for i in range(self.NB_ARRAYS)], 
                                                                                         [f"array {i}" for i in range(self.NB_ARRAYS)]]}
        """The dictionary is used fill all the widgets in the array synching frames.
        For each animation it return an array containing:
        - The top labes
        - The content of the entry boxes
        - The label in front of the checkboxes
        """

    def change_animation(self, 
                         animation: str, 
                         LED_array_id: int,
                         values: Union[Tuple[Union[str, int, float, bytes, bool]], List[Union[str, int, float, bytes, bool]]]) -> None:
        """Send a command to the arduino to change.
        
        ### Args:
            - animation (str): Name of the animation. The folowing values are accepted: 
            {"zip", "strobe", "volume_bar", "twinkle_pixel", "fire", "shooting_star", "sparkle", "fade", "rainbow_wheel"}
            - LED_array_id (int): The id of the array that will have it's animation chaged
            - values (tuple, list): A list or tuple of parameter values to pass to the animation function.
        """
        params = ",".join([str(v) for v in values])
        message = f"<{LED_array_id},{self.animation_codes[animation]},{params}>"
        print(message) # Print message to cmd terminal
        self.last_message_sent = message
        try:
            self.controller.write(bytes(message, 'utf-8'))
            self.last_message_sent = time()
        except AttributeError:
            print("No connection to arduino.")

    def send_message(self, message: str) -> str:
        """Sends the provided text to the arduino."""
        print(message) # Print message to cmd terminal
        try:
            self.controller.write(bytes(message, 'utf-8'))
            self.time_of_last_message_sent = time()
            self.last_message_sent = message
        except AttributeError:
            print("No connection to arduino.")

    def read_str(self) -> Union[str, None]:
        """Reads the string sent from the arduino.

        ### Returns:
            - str: The string being sent from the arduino.
        """
        if not self.controller: # Checks that interface is connected to a controller
            print("No connection to controller. Can't send message.")
            self.error_fun_call("No connection to controller. Can't send message.")
            return "No connection to controller. Can't send message."
        #print(self.controller.readline().decode())
        if self.controller.in_waiting > 0:
            try:
                #message = self.controller.readline().decode()
                message = self.controller.read_until("@").decode().replace("@","")
                if message and message != "":
                    self.time_of_last_message_receved = time()
                    self.last_message_receved = message
                return message
            except:
                return "Error when reading message from serial! Try again \n"
        return None
    
    def read_measurments(self) -> Union[float, None]:
        """Reads measurments sent from the controller. """
        try:
            return float(str(self.controller.readline())[2:][:-5])
            #if self.controller.in_waiting > 1:
            #    temp = self.controller.read(2)
            #    print(temp)
            #    out = unpack("e",temp)[0]
            #    print(f"out: {out}, type: {type(out)}")
            return out
            
        except:
            return None

    def try_to_connect(self) -> str:
        """Attempt to connect to the arduino port.
        
        ### Returns:
            - str: User message on connection status.
        """
        try:
            self.controller = Serial(port=self.port, baudrate=self.baudrate, timeout=10)
            return "Connection to arduino established."
        except:
            self.error_fun_call("Failed to connect to controller! Check that that the port is correct!")
            return "Failed to connect to controller! Check that that the port is correct!"
        
    def serial_ports():
        """ Lists serial port names. !!! Try to use serial.tools.list_ports in stead !!!

            ### Returns:
                - list: A list of the serial ports available on the system.
            ### Raises: 
                - EnvironmentError: On unsupported or unknown platforms.
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = Serial(port)
                s.close()
                result.append(port)
            except (OSError, SerialException):
                pass
        return result