/*
 * pin 1 - D2                |  Micro SD card     |
 * pin 2 - D3                |                   /
 * pin 3 - CMD               |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - CLK               | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - D0                | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - D1                |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * ESP32            |  4   2  GND  14 3V3 GND  15 13  12  |
 * Pin name         | D1  D0  VSS CLK VDD VSS CMD D3  D2  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 * WARNING: ALL data pins must be pulled up to 3.3V with an external 10k Ohm resistor!
 * Note to ESP32 pin 2 (D0): Add a 1K Ohm pull-up resistor to 3.3V after flashing
 *
 * SD Card | ESP32
 *    D2       12
 *    D3       13
 *    CMD      15
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      14
 *    VSS      GND
 *    D0       2  (add 1K pull up after flashing)
 *    D1       4
 *
 *    For more info see file README.md in this library or on URL:
 *    https://github.com/espressif/arduino-esp32/tree/master/libraries/SD_MMC
 */
#include "FS.h"
#include "SD_MMC.h"

/** This value indicates if the ONE_BIT_MODE is used. Set to false for slightly faster comunication. SD card pin 1 and 9 must be pluged if set to false.  */
#define ONE_BIT_MODE true
/* Pins are in order from left to right --> top to bottom.*/
#define D1_PIN 4
#define D0_PIN 2
//#define CLK_PIN 14
#define CMD_PIN 15
/* Only used if ONE_BIT_MODE == false */
#define D3_PIN 13
#define D2_PIN 12


/**
 * Class used to manage an SD card reader.
 */
class SDManager{
    public:

    static fs::FS errorType;

    static uint8_t cardType;

    static void setup();

    static void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

    static void createDir(fs::FS &fs, const char *path);

    static void removeDir(fs::FS &fs, const char *path);

    static void readTxtFile(fs::FS &fs, const char *path);

    static void writeTxtFile(fs::FS &fs, const char *path, const char *message);

    static void appendTxtFile(fs::FS &fs, const char *path, const char *message);

    static void renameFile(fs::FS &fs, const char *path1, const char *path2);

    static void deleteFile(fs::FS &fs, const char *path);

    static void testFileIO(fs::FS &fs, const char *path);
};

// ##############################################################################
// ########################## STATIC VARIABLES DEFINITIONS ######################
// ##############################################################################

fs::FS SDManager::errorType = SD_MMC;

uint8_t SDManager::cardType = 0;

// ##############################################################################
// ########################## STATIC FUNCTION DEFINITIONS #######################
// ##############################################################################

/**
 * Setts all the pinModes for the SD card manager.
 */
void SDManager::setup(){
    pinMode(D0_PIN,INPUT_PULLUP);
    // Not sure if next 4 pinMode calls are needed remove if not working
    pinMode(D1_PIN,INPUT_PULLUP);
    pinMode(CMD_PIN,INPUT_PULLUP);
    pinMode(D3_PIN,INPUT_PULLUP);
    pinMode(D2_PIN,INPUT_PULLUP);

    if (!SD_MMC.begin("/sdcard", ONE_BIT_MODE)) {
        Serial.println("Card Mount Failed");
    }

    SDManager::cardType = SD_MMC.cardType();

    Serial.print("SD_MMC Card Type: ");
    switch(SDManager::cardType){
        case CARD_NONE:
          Serial.println("No SD_MMC card attached");
          break;
        case CARD_MMC:
            Serial.println("MMC");
            break;
        case CARD_SD:
            Serial.println("SDSC");
            break;
        case CARD_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
}

/**
 * Prints the list the contents of a given directory.
 * 
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param dirname The name of the directory to list.
 * @param levels Indicates the depth of directories to list.
 */
void SDManager::listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
      Serial.println("Failed to open directory");
      return;
    }
    if (!root.isDirectory()) {
      Serial.println("Not a directory");
      return;
    }

    File file = root.openNextFile();
    while (file) {
      if (file.isDirectory()) {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if (levels) {
          listDir(fs, file.path(), levels - 1);
        }
      } else {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
      }
      file = root.openNextFile();
    }
}

/**
 * Creates a directory at the specified path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the directory to be created.
 */
void  SDManager::createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

/**
 * Deletes the directory at the specified path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the directory to be deleted.
 */
void SDManager::removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

/**
 * Reads the file at the specifide path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the txt file to be read.
 */
void SDManager::readTxtFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
}

/**
 * Writes the given message to a new file at the specified path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the txt file to be read.
 * @param message The message to be writen to the file.
 */
void SDManager::writeTxtFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

/**
 * Appends the given message to the specified the file at the specifide path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the txt file to be read.
 * @param message The message to be writen to be appended to the file.
 */
void SDManager::appendTxtFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}

/**
 * Renames or moves a file to from the first path to the second path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path1 The path to the original file.
 * @param path2 The path to the new file.
 */
void SDManager::renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

/**
 * Delets the file at the specifide path.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the file to be deleted.
 */
void SDManager::deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

/**
 * Measures how long it takes to read a file and prints out the result allong with its size.
 * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
 * @param path The path to the file to be tested.
 */
void SDManager::testFileIO(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %lu ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %lu ms\n", 2048 * 512, end);
  file.close();
}