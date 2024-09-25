# Project description
This project seeks to program controlers for LED arrays allong with perifiral utilities such as a computer and smartphone interfaces to modifie the animations in real time.

# Features:
Most feature are buggy and not completely polished as focus was on implementing all wanted features as fast as possible to then be able to polish them sporadically.

- Animation and animation parameter selection in real time both from the console and from a python GUI interface.
- Real time plotting of sound using an I2S microphone. As well as ability to save the recorded sound.
- Automated pipeline for beat detection. Place mp3 or wav file in Python_interface/Beat_detection/Sound_files, and launch the convert_sound_files.bat to convert sound files to .bin files. The bin files contain the arrays of all the timestamps of the music beats in milliseconds. This bin file can be read by the controller so as to execute animations in sync with music.
- Realtime beat detection using an I2S compatible microphone so as to be able to launch animations in time with the music in real time.

**Currently supported boards:**
- arduino UNO (Not developed much)
- esp32

# Before compiling 
- Set the compiler constants in the Globals.h file
- Set the controler pin variables in the main sketch (stage_lights.ino)
- Set the correct FastLED.addLeds functions as they cannot be defined dinamicaly as they as set at compile time.

# Using console interface

The controler can be controled by sending commands through a serial console. The syntax of the comands works as follows:

- @[data parsing method]<param1, param2, param3, ....>

The "@[data parsing method]" part of the command can be omited and the parameters will be parssed using the currently active data parsing mode. The syntax is akin to @[function name]<function parameters>.

**Examples:** 
- @1<> :This will turn on the beat detection features.
- <0,1,20,50> : This will use the currently selected data parsing mode to parse the given parameters. The default mode is the animation selection mode. In this case it will set array 0 to animation 1 and set the first parameter of that animation to 20 and the second parameter of that animation to 50.

## Data parsing modes

### Animation selection

<nb_array, nb_animation, list_of_parameters, ....>

- nb_annay: The index of the array in the led_arrays array.
    + If = -1: Will set all the parameters of the animation for the given array but will not change the curently playing animation.
    + If = -2: Will set all array to the specified animation and given parameters.
- nb_animation: The index of the animation function in the animations array.

If there are less parameters in the command than animation parameters only the ones present in the command will be changed.
If there are more parameters in the command than animation parameters the controller will crash.


# Using the python interface
The python interface is located in the folder of the same name. Simply double click the launch.bat (no linux support yet) to start the interface.
The cmd console will remane open and is used as a debuging terminal.

>[WARNING]
>
>The arduino IDE or any other application that hase made a serial connection to the controller must be closed before launching the
>interface otherwise the python interface will be unable to connect to the controller.

# Adding animations
When adding an annimation make sure you update:
- The `animations` function array with the memory address of the new animation function.
- Add the appropriate arrays and animation variables to the `animParamRef` struct that references all the parameters of that animation.
- The `getParameters***` and `setParameters***` functions switches to allow for dynamic animation change.

# Ideas for additions
- Add a sync option that toggles between animation function types
- Make sur that the data parsing checks that selected array is not larger than NB_ARRAYS. Same for animations.
- Correct python interface message sending from boxes