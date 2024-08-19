# Project description
This project seeks to program controlers for LED arrays allong with perifiral utilities such as a computer and smartphone interfaces to modifie the animations in real time,

# Before compiling 
- Set the compiler constants in the Globals.h file
- Set the controler pin variables in the main sketch (stage_lights.ino)
- Set the correct FastLED.addLeds functions as they cannot be defined dinamicaly as they as set at compile time.

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