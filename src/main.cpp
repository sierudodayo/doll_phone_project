// Arduino Library
#include <Arduino.h>

// LGFX Library
#include <LGFX.cpp>
#include <SPIFFS.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// LGFX Static
static LGFX lcd;
static LGFX lcdIn;
static LGFX_Sprite sprite[10];

// Bluetooth Library
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Memory Library
#include <SPIFFS.h>
#include <FS.h>

// Image Library
#include <BITMAPDecoder.h>
BITMAPDecoder bitmap = BITMAPDecoder();

//Other Library
#include <ios>

#define FORMAT_SPIFFS_IF_FAILED true
#define BLE_DEVICE_NAME "SmartPhoneForDolls"
#define SERVICE_UUID "b65ac29b-346b-4c47-a64c-c6785bb7800b"
#define MESSAGE_CHARACTERISTIC_UUID "9b3f623b-9c48-4fca-840e-b43e6b4fc8e4"
#define IMAGE_CHARACTERISTIC_UUID "9b3f623b-9c48-4fca-840e-b43e6b4fc8e5"
#define LED_PIN 2

String imageFilePath = "/sample.bmp";
String binaryBmp = "";
String textMessage = "";
bool spiffsResponse = true;
bool changeStatus = true;

// display info
const uint16_t dsp_width = 80;
const uint16_t dsp_height = 160;

// Image file array
const uint8_t bmp_data_buf = {};

BLECharacteristic *pCharacteristic;
BLECharacteristic *imageCharacteristic;

// image file reception
bool receptImageFile() {
  bool receptImage = false;
  return receptImage;
}


/**
 * fileExistanceCheck
 * 
 * return bool; // existed = true;not exist = false;
*/
bool fileExistanceCheck() {
  File file = SPIFFS.open(imageFilePath, "r");
  if (!file) {
    return false;
  }
  return true;
}

/**
 * printImageOnDisplay
 * 
 * Display image files on the display
*/
bool printImageOnDisplay()
{
  if (fileExistanceCheck()) {
    File file = SPIFFS.open(imageFilePath, "r");
    if (!file) {
      return false;
    }
    //lcd.drawJpgFile(file, 0, 0);
  }
  return true;
}

// No Image
void printNoImageOnDisplay() {
  lcd.fillScreen(TFT_WHITE);
  lcd.setCursor(10,25);
  lcd.setFont(&fonts::Font0);
  lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  lcd.setTextSize(3);
  lcd.print("No Image");
}

// existed image
void printTextOnDisplay(String text) {
  lcd.fillScreen(TFT_WHITE);
  lcd.setCursor(10,25);
  lcd.setFont(&fonts::Font0);
  lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  lcd.setTextSize(3);
  lcd.print(text);
}

// save image on spiffs
void saveBmpImage(uint8_t buf) {

  int rowSize = (2 * dsp_width + 3) & ~ 3;

  File saveFile = SPIFFS.open(imageFilePath, "w");

  if (saveFile) {
  }
}

// callback class
class BLECallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    textMessage = pCharacteristic->getValue().c_str();
    printTextOnDisplay(binaryBmp);
  }
};

// image Callback class
class ImageCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *imageCharacteristic) override {
    binaryBmp = imageCharacteristic->getValue().c_str();
    printTextOnDisplay(binaryBmp);
  }
};


void setup()
{
  // Serial Port init
  Serial.begin(19200);

  // spiffs init
  spiffsResponse = SPIFFS.begin(true);

  // display init
  lcd.init();
  lcd.setRotation(3);

  // BLE init
  BLEDevice::init(BLE_DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();

  // serice Create
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // characteristic create
  pCharacteristic = pService->createCharacteristic(
                              MESSAGE_CHARACTERISTIC_UUID,
                              BLECharacteristic::PROPERTY_READ | 
                              BLECharacteristic::PROPERTY_WRITE
                              );
  imageCharacteristic = pService->createCharacteristic(
                              IMAGE_CHARACTERISTIC_UUID,
                              BLECharacteristic::PROPERTY_READ |
                              BLECharacteristic::PROPERTY_WRITE
                              );
  
  pCharacteristic->setCallbacks(new BLECallbacks());
  imageCharacteristic->setCallbacks(new ImageCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
}

void loop() {

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  Serial.println("Test Serial Message");

  bool result = false;
  result = receptImageFile();
  if (result) {
    if (changeStatus) {
      printImageOnDisplay();
    }
    changeStatus = false;
  } else {
    if (changeStatus) {
      printNoImageOnDisplay();
    }
    changeStatus = false;
  }
}