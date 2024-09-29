#include "Bluetooth.h"
#include "Com_interface.h"

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

#define START_MARKER '<'
#define END_MARKER '>'
/** The flag used to signal that the following integer is to be interpreted as mode comunication mode change.*/
#define MODE_CHANGE_FLAG '@'

/**The mode used to determin how receved data is parsed. In sync with the curently used functionality.*/
//extern int ComInterface::dataParsingMode;

BluetoothSerial SerialBT;

/* The blurtooth device name */
const String bltDeviceName = "LED controller";

/* Global variable indicating to the rest of the program that bluetooth has been turned on. */
bool bluetoothOn = false;

/* Global varaible indicating if the receeved massage comes from the bluetooth serial */
bool bluetoothReceving = false;


// ################### Reading data code ##################
/*The maximum number of bytes that the buffer can recive.*/
const byte numChars = 32;

/*The buffer in which receved strings stored. Has a terminating character (\0).*/
char receivedCharsBT[numChars];

/*Temporary array for use when parsing*/
char tempCharsBT[numChars];        

/**
-Boolean indicating if there is data being proccesed.
If true will not read and parse from the buffer. Is set back to false once all data reved data hase been delt with.
*/
static boolean newDataBT;

/**The mode used to determin how receved data is parsed. In sync with the curently used functionality.*/
static int dataParsingMode;

void StartBluetooth(){
    SerialBT.begin(bltDeviceName);  //Bluetooth device name
    bluetoothOn = true;
    Serial.printf("Bluetooth statrted!\nName: \"%s\".\n", bltDeviceName.c_str());
}

void endBluetooth(){
    SerialBT.end();
    bluetoothOn = false;
    Serial.print("Bluetooth ended!");
}

/*This function is requiered for boards like the arduino that have trouble receving data over serial*/
void recvWithStartEndMarkersBluetooth() {
  static byte ndx = 0;
  char rc;
  rc = SerialBT.read();
  // Checks the first character which is the flag that indicates which parsing mode will be used
  // The second character is the parsing mode to be used
  switch(rc){
    case START_MARKER:
      break;
    case MODE_CHANGE_FLAG:
      try{
        ComInterface::dataParsingMode = SerialBT.read()-48; //Serial.parseInt();
        SerialBT.read(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IF PROBLEMS WHEN PARSING COMMAND MIGHT BE THIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      catch(...){
        // Do nothing
      }
      break;
    default:
      break;
  }
  while (SerialBT.available() > 0 && newDataBT == false) {
      rc = SerialBT.read();
      if (rc != END_MARKER) {
          receivedCharsBT[ndx] = rc;
          ndx++;
          if (ndx >= numChars) {
              ndx = numChars - 1;
          }
      }
      else {
          receivedCharsBT[ndx] = '\0'; // terminate the string
          //recvInProgress = false;
          ndx = 0;
          newDataBT = true;
      }
  }
}
