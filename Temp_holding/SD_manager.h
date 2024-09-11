#ifndef SD_manager.h
#define SD_manager.h

#include "FS.h"
#include "SD_MMC.h"
/**
 * Class used to manage an SD card reader.
 */
class SDManager{
    public:

      static fs::FS fileSystem;

      static uint8_t cardType;

      static void setup();

      static void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

      static void createDir(fs::FS &fs, const char *path);

      static void removeDir(fs::FS &fs, const char *path);

      static void readTxtFile(fs::FS &fs, const char *path);

      /**
      * Reads a provided txt file containing the timing array for light animations.
      * @typedef T must be of type float of any number of bits.
      * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
      * @param path The path to the binary file to be read.
      * @param numElements The number of elements present in the array. (modifies the passed reference for posible use later)
      * @return Array of type T. Default is float.
      */
      template<typename T> static T* readTimingTxtFile(fs::FS &fs, const char *path, int* numElements){
        File file = fs.open(path, "r");
        if (!file) {
          Serial.println("Opening file to read failed!");
          return nullptr;
        }
        *numElements = 0;
        T* dataArray = nullptr;
        int capacity = 100;
        dataArray = (T*)malloc(capacity * sizeof(T));
        if (!dataArray) {
            Serial.println("Failed to allocate memory for data array!");
            file.close();
            return nullptr;
        }
        T value = -1.0;
        while (value != 0.0) {
          value = static_cast<T>(file.parseFloat());
          if(value == 0.0)
            break;
          // Store the value in the array
          if (*numElements >= capacity) {
              // Resize the array if necessary
              capacity *= 2; // Double the capacity
              dataArray = (T*)realloc(dataArray, capacity * sizeof(T));
              if (!dataArray) {
                  Serial.println("Failed to reallocate memory for data array!");
                  file.close();
                  return nullptr;
              }
          }
          dataArray[*numElements] = value; // Store the value
          (*numElements)++;                // Increment the element count
        }
        /*
          while(true){
            temp = file.parseFloat();
            if(temp == 0.0)
              break;
            Serial.println(temp);
          }
        */
          file.close();
          return dataArray;
}

      /**
      * Reads a provided bin file containing the timing array for light animations.
      * @param fs The DS_MMC obejct being used. (Always use SD_MMC)
      * @param path The path to the binary file to be read.
      * @param numElements The number of elements present in the array. (modifies the passed reference for posible use later)
      * @return Array of type T. Default is float.
      */
      template <typename T = float> static T* readTimingBinFile(fs::FS &fs, const char *path, int* numElements){
          // Open the binary file for reading
          File file = SD_MMC.open(path, FILE_READ);
          if (!file) {
              Serial.println("Failed to open file for reading");
              return NULL;
          }
          // Get the size of the file
          size_t fileSize = file.size();
          // Calculate the number of floats in the file
          *numElements = fileSize / sizeof(T);
          // Allocate memory for the array of floats
          T* floatArray = (T*)malloc(fileSize);
          if (!floatArray) {
              Serial.println("Failed to allocate memory for array!");
              file.close();
              return NULL;
          }
          // Read the binary data from the file into the array
          file.read((uint8_t*)floatArray, fileSize);
          file.close();
          return floatArray;
      }

        static void readTimingTxtFile(fs::FS &fs, const char *path);

        static void writeTxtFile(fs::FS &fs, const char *path, const char *message);

        static void appendTxtFile(fs::FS &fs, const char *path, const char *message);

        static void renameFile(fs::FS &fs, const char *path1, const char *path2);

        static void deleteFile(fs::FS &fs, const char *path);

        static void testFileIO(fs::FS &fs, const char *path);

        //template <typename T> static T readDataChunk(FILE *f, long int offset);
    };
    
#endif