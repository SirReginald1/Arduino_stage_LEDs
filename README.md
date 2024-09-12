# Project description
This project seeks to program controlers for LED arrays allong with perifiral utilities such as a computer and smartphone interfaces to modifie the animations in real time.

# Features:
Most feature are buggy and not completely polished as focus was on implementing all wanted features as fast as possible to then be able to polish them sporadically.

- Animation and animation parameter selection in real time both from the console and from a python GUI interface.
- Real time plotting of sound using an I2S microphone. As well as ability to save the recorded sound.
- Automated pipeline for beat detection. Place mp3 or wav file in Python_interface/Beat_detection/Sound_files, and launch the convert_sound_files.bat to convert sound files to .bin files. The bin files contain the arrays of all the timestamps of the music beats in milliseconds. This bin file can be read by the controller so as to execute animations in sync with music.
- Realtime beat detection using an I2S compatible microphone so as to be able to launch animations in time with the music in real time.

**Currently supported boards:**
- arduino UNO
- esp32

# Before compiling 
- Set the compiler constants in the Globals.h file
- Set the controler pin variables in the main sketch (stage_lights.ino)
- Set the correct FastLED.addLeds functions as they cannot be defined dinamicaly as they as set at compile time.

# Using the python interface
The python interface is located in the folder of the same name. Simply double click the launch.bat (no linux support yet) to start the interface.
The cmd console will remane open and is used as a debuging terminal.

>[WARNING]
>
>The arduino IDE or any other application that hase made a serial connection to the controller must be closed before launching the
>interface otherwise the python interface will be unable to connect to the controller.

# Adding animations
When adding an annimation make sure you update:
- The `runAnimations` function switch to allow the transition to that animation.
- Add the appropriate arrays to the `animParamRef` struct that reference all the parameters of that animation.
- The `getParameters***` and `setParameters***` functions switches to allow for dynamic animation change.
- And possibly a code reference dictionay thing if it is implemented.

# Ideas for additions
- Add a sync option that toggles between animation function types
- Make sur that the data parsing checks that selected array is not larger than NB_ARRAYS. Same for animations.
- Correct python interface message sending from boxes