from typing import Union, Dict, Tuple, List
from serial import Serial
from numpy.random import randint


class ArduinoInterface():
    """Class that deals with interfacing with the arduino."""
    
    init_values_strobe: List[Union[int, Tuple]] = [20, 60, (255, 255, 255)]
    """Parameter init values for the strobe animation. {time_on, time_off, (R, G, B)}"""

    init_values_zip: List[int] = [2,10,122,0,(0, 0, 255)]
    """Parameter init values for the zip animation."""

    init_values_twinkle_pixel: List[int] = [200,50,50,20,100]
    """Parameter init values for the pixel animation."""

    init_values_fire: List[int] = [50,50,0,0.01]
    """Parameter init values for the fire animation. {flame_height, sparks, delay, intensity}"""

    init_values_shooting_star: List[int] = [(150, 0, 150), 20, 10, 2000, 1]
    """Parameter init values for the shooting star animation.
    {(R, G, B), tail_length, delay_duration, interval, direction}
    """

    init_values_sparkle: List[int] = [(0, 255, 100), 0]
    """Parameter init values for the sparkle animation."""

    init_values_fade: List[int] = [(255, 255, 255), 0]
    """Parameter init values for the fade animation."""

    init_values_rainbow_circle: List[int] = [2000]
    """Parameter init values for the rainbow wheel animation. {delay}"""

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

    animation_codes = {"zip" : 9, 
                       "strobe" : 8, 
                       "volume_bar" : 8, 
                       "twinkle_pixel" : 6, 
                       "fire" : 5, 
                       "shooting_star" : 4, 
                       "sparkle" : 3, 
                       "fade" : 2, 
                       "rainbow_circle" : 1}
    """Dictionary of the codes that reference each animation in the arduino code."""
    
    animation_labels = ["zip", 
                       "strobe", 
                       "volume_bar", 
                       "twinkle_pixel", 
                       "fire", 
                       "shooting_star", 
                       "sparkle", 
                       "fade", 
                       "rainbow_circle"]
    """List of all the animation names."""

    # Send bytes representing the animation code
    # If bytes long then with parameters else just change animation

    def __init__(self) -> None:
        """Constructor for arduino interface."""

        try:
            self.arduino = Serial(port='COM3', baudrate=9600, timeout=20000)
            """The arduino connection port."""
        except:
            #raise RuntimeError("Connection error! Check that that the port is correct!")
            print("Connection error! Check that that the port is correct!")
            self.arduino = None

        #self.arduino = arduino

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
 
    def send_command(self, 
                     animation: str, 
                     LED_array_id: int,
                     values: Union[Tuple[Union[str, int, float, bytes, bool]], List[Union[str, int, float, bytes, bool]]]) -> None:
        """Send a message to the to the arduino.
        
        ### Args:
            - animation (str): Name of the animation. The folowing values are accepted: 
            {"zip", "strobe", "volume_bar", "twinkle_pixel", "fire", "shooting_star", "sparkle", "fade", "rainbow_wheel"}
            - values (tuple, list): A list or tuple of parameter values to pass to the animation function.
        """
        params = ",".join([str(v) for v in values])
        message = f"{LED_array_id}:{self.animation_codes[animation]}:{params}"
        print(message)
        try:
            self.arduino.write(bytes(message, 'utf-8'))
        except AttributeError:
            print("No connection to arduino.")
        finally:
            print("Can't send message.")

    def send_message(self, message: str) -> None:
        """Sends the provided text to the arduino."""
        print(message)
        try:
            self.arduino.write(bytes(message, 'utf-8'))
        except AttributeError:
            print("No connection to arduino.")
        finally:
            print("Can't send message.")

    def read_str(self) -> str:
        """Reads the string sent from the arduino.

        ### Returns:
            - str: The string being sent from the arduino.
        """
        try:
            return str(self.arduino.readline(),"utf-8").strip("'\r\n")[2:]
        except AttributeError:
            print("No connection to arduino. Can't send message.")
            return "Arduino connection error!"
        
    def try_to_connect(self) -> str:
        """Attempt to connect to the arduino port.
        
        ### Returns:
            - str: User message on connection status.
        """
        try:
            self.arduino = Serial(port='COM3', baudrate=9600, timeout=100)
            return "Connection to arduino established."
        except:
            return "Failed to connect to arduino! Check that that the port is correct!"