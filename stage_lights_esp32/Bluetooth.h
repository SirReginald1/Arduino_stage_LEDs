#ifndef Bluetooth_h
#define Bluetooth_h

#include "BluetoothSerial.h"


extern BluetoothSerial SerialBT;

/* Global variable indicating to the rest of the program that bluetooth has been turned on. */
extern bool bluetoothOn;

/* Global varaible indicating if the receeved massage comes from the bluetooth serial */
extern bool bluetoothReceving;

void StartBluetooth();

void endBluetooth();

#endif