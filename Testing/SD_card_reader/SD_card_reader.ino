#include "driver/uart.h"
#include "SD_manager.h"
#include "FS.h"
#include "SD_MMC.h"


void setup(){
  uart_set_baudrate(UART_NUM_0, 1000000);
  Serial.begin(1000000);
  //setCpuFrequencyMhz(240);

  SDManager::setup();

  SDManager::listDir(SD_MMC, "/", 0);
  SDManager::createDir(SD_MMC, "/mydir");
  SDManager::listDir(SD_MMC, "/", 0);
  SDManager::removeDir(SD_MMC, "/mydir");
  SDManager::listDir(SD_MMC, "/", 2);
  SDManager::writeTxtFile(SD_MMC, "/hello.txt", "Hello ");
  SDManager::appendTxtFile(SD_MMC, "/hello.txt", "World!\n");
  SDManager::readTxtFile(SD_MMC, "/hello.txt");
  SDManager::deleteFile(SD_MMC, "/foo.txt");
  SDManager::renameFile(SD_MMC, "/hello.txt", "/foo.txt");
  SDManager::readTxtFile(SD_MMC, "/foo.txt");
  SDManager::testFileIO(SD_MMC, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

}

void loop(){

}