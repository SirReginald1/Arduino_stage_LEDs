//#include "driver/uart.h" // driver used to change the baudrate
#include "esp_task_wdt.h"
#include "esp_system.h"
#include "freertos/queue.h"
#include <Arduino.h>
#include <FastLED.h>
#include <arduinoFFT.h>
//#include <TimerOne.h>
#include "Globals.h"
#include "Animations.h"
#include "Com_interface.h"
#include "SD_manager.h"
#include "Beat_detector.h"
#include "Microphone.h"
#ifdef USE_BLUETOOTH
  #include "Bluetooth.h"
#endif


/*Indicated if the program should be run by the real time animation interface or just animation numbers*/
//#define USE_INTERFACE
/*Indecates if the the microphone should be used*/
//#define USE_MIC
// Setting LED pins
#define LED_PIN_0 19
#define LED_PIN_1 18
#define LED_PIN_2 17
#define LED_PIN_3 16
// Setting button pins
#define BTN_PIN 2

#ifdef USE_BLUETOOTH
  /* The bluetooth serial object */
  extern BluetoothSerial SerialBT;
#endif

// Functioning modes
/** The value indicating if the animation selection mode is runing. (Is the default startup mode) */
bool appModeAnimSelect = true;
/** The value indicating if the preprepared animation mode is runing. must be. */
bool appModePrePrepAnim = false;
/** 
* The value indicating if the mic and FFT loop is runing on core0. This value is used so as to not have to share resources between cores.
* Is set at the same time as the task notification is sent in ConInterface pars function.
*/
bool appModeMicFFTOnCore1 = false;


/* Array of all LED arrays. */
CRGB led_arrays[NB_ARRAYS][NUM_LEDS];

/** The array of structs that contain*/
animParamRef animParamRefs[NB_ARRAYS];

//esp_task_wdt_in

// ############################# Time variables ##############################
// Millisecond counter
unsigned long millisecs;

// ############################# Interupt ##############################
/**
The code to be executed during a hardware interupt.
*/
/*
void hardware_interrupt(){
  int next_value = (animation+1) % 9;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}
*/
/**
Toggle value for a button.
*/
//volatile bool toggleState = false;
/**
Function that checks if a switch is pressed and if so starts an interupt.
*/
/*
void interupt_switch(){
  Serial.println("Entered iterupt.");
  Serial.print("toggle value:"); Serial.println(toggleState);
  if(digitalRead(BTN_PIN) == LOW){
    toggleState = !toggleState;
  }
  //digitalWrite(LED1_PIN, !toggleState);
  hardware_interrupt();
}
*/

/**
The code to be executed during an software interupt.
*/
/*
void software_interrupt(){
  int next_value = (animation+1) % 7;
  animation = next_value == 0 ? 1 : next_value;
  FastLED.clear();
  Serial.print("iterupt animation value: ");Serial.print(animation);Serial.println();
}
*/
/** Variables used for preprepared animations */ 
float* timings;
int timingsLength = 0;

/** The task handler for the FFT task runing on core 0 */
TaskHandle_t mainCore0Handle;
/** The notification queue used by core0 to signal core 1 */
QueueHandle_t core1NotifQueue;
/** Value indicating what audio feature hase been detected. If none detected will be set to NO_AUDIO_FEATURE_DETECTED. */
extern u_int16_t FFTAudioFeatureDetected;


void setup() {
  // #########################################################
  // ###################### HARDWARE #########################
  // #########################################################
  // ################## PIN SETUP ####################
  // Set btn pin
  //pinMode(BTN_PIN, INPUT_PULLUP);

  // following three lines to speed up ADC (that is analogRead() )
  //bitClear(ADCSRA,ADPS0);
  //bitSet(ADCSRA,ADPS1);
  //bitClear(ADCSRA,ADPS2);
  // ################ INTERUPT SETUP ##################
  // Setting hardwar interupt. Executes the interupt function when stat of pin 2 changes
  //attachInterrupt(digitalPinToInterrupt(2), interupt_switch, FALLING);
  // Initializing sofware iterupt. (in microseconds)
  //TimerOne.initialize(1000000);
  // Setting software interupt
  //TimerOne.attachInterrupt(software_interrupt);
  // ################ IN OUT CONSOLE ##################
  // Setting up console output.
  //uart_set_baudrate(UART_NUM_0, 155200);
  Serial.begin(115200);
  // Setting the time the program will wait for input.
  Serial.setTimeout(50);
  // #########################################################
  // ###################### SD card reader ###################
  // #########################################################
  SDManager::setup();
  SDManager::listDir("/", 1);

  // #########################################################
  // ###################### Beat detection ###################
  // #########################################################
  beatDetectionSetup();
  xTaskCreatePinnedToCore(mainCore0, "mainCore0", 10000, NULL, 1, &mainCore0Handle, 0); // Setting up main code to run on second core.
  if(mainCore0Handle == NULL){
    Serial.println("The task on the second core was not created!");
  }
  core1NotifQueue = xQueueCreate(10, // Queue length
                            sizeof(u_int16_t));
  // #########################################################
  // ######################### LED ###########################
  // #########################################################

  // Initialising led arrays
  FastLED.addLeds<WS2812B, LED_PIN_0, RGB>(led_arrays[0], NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_1, RGB>(led_arrays[1], NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_2, RGB>(led_arrays[2], NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_3, RGB>(led_arrays[3], NUM_LEDS);

  // Set power limit of LED strip to 5V, 1500mA
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  // Make sure all lights are off
  FastLED.clear();
  // Setup array IDs
  for(int i=0;i<NB_ARRAYS;i++){
    animParamRefs[i].arrayId = i;
  }
  // #########################################################
  // ################## Set variables ########################
  // #########################################################
  // Load preprepared animation timings array from SD card
  timings = SDManager::readTimingBinFile("/Vibe Chemistry & HARLEE - Same Old Song_wav.bin", &timingsLength);
}

void loop() {
  millisecs = millis();
  // ############# LED CODE ##############
  if(Serial.available() > 0){
    ComInterface::readInput();
    FastLED.clear(); // !!!!!!!!!!!!!! Flickering at animation switch probably comes from here !!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  #ifdef USE_BLUETOOTH
    else if(bluetoothOn && SerialBT.available() > 0){
      Serial.println("Detected bluetooth input!");
      Serial.println(SerialBT.read());
    }
  #endif
  if(appModeMicFFTOnCore1){
    readBeatQueue();
    Animations::runAnimations(led_arrays, animParamRefs);
    resetBeatDetectionVariables();
  }
  else{
    Animations::runAnimations(led_arrays, animParamRefs);
  }
}

void mainCore0(void* parameter){
  // Variable to store the notification value.
  uint32_t notificationValue;
  /** The value indicating if the microphone reading and FFT task should run */
  bool appModeMicFFTOn = false;

  /** Used by core 0 as a pointer to send signal to core 1 if an audio feature is detected */
  u_int16_t detectedAudioFeature = NO_AUDIO_FEATURE_DETECTED;

  unsigned long start; // Variable used for timing print
  for(;;){
    // This part of the code deals with chacking if signals are recived from the main loop and processing them accordingly.
    // This checks to see if there is a notification pending returns true if there was one, if timer exceded send false and move on.
    if (xTaskNotifyWait(0x00, // Clear no bites in notificationValue before reading
                        ULONG_MAX, // Clear all the bites in notificationValue after reading it.
                        &notificationValue, // The variable to check
                        (TickType_t)1) /*The time in ticks*/ == pdTRUE) {
      switch(notificationValue){
        case MODE_MIC_FFT_ON:
          appModeMicFFTOn = !appModeMicFFTOn;
          Serial.print("FFTOn:");
          Serial.println(appModeMicFFTOn);
          break;
        default:
          Serial.print("Core 0 notif switch default");
      }
    }

  
    if(appModeMicFFTOn){
      //Serial.println("In FFT loop");
      //start = millis();
      getFFT(); // Takes about 20 ms to execute
      
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      if(detectKick(5)){
        // Send notification to the main core 1 through the queue
        detectedAudioFeature = KICK_DETECTED;
        xQueueSend(core1NotifQueue, &detectedAudioFeature, 0);
        //Serial.println("Core 0 kick!");
        //detectedAudioFeature = NO_AUDIO_FEATURE_DETECTED; // Probably don't need this
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      
    }

    // Delay this task
    //vTaskDelay(pdMS_TO_TICKS(500));  // 500ms delay
  }
}