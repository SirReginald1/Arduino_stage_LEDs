#ifndef SD_manager.h
#define SD_manager.h

    #include "FS.h"
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

        static void readBinFile(fs::FS &fs, const char *path);

        static void writeTxtFile(fs::FS &fs, const char *path, const char *message);

        static void appendTxtFile(fs::FS &fs, const char *path, const char *message);

        static void renameFile(fs::FS &fs, const char *path1, const char *path2);

        static void deleteFile(fs::FS &fs, const char *path);

        static void testFileIO(fs::FS &fs, const char *path);
    };
    
#endif