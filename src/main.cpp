// Arduino Library
#include <Arduino.h>

// Memory Library
#include <SPIFFS.h>
#include <FS.h>
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

String imageFilePath = "/sample.jpg";
String binaryBmp = "";
String textMessage = "";
bool spiffsResponse = true;
bool changeStatus = true;

uint8_t receivedData[512];
uint32_t totalLength = 0;
uint32_t receivedLength = 0;

// display info
const uint16_t dsp_width = 80;
const uint16_t dsp_height = 160;

// Image file array
const uint8_t bmp_data_buf = {};
bool feastCallBack = true;

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
  return SPIFFS.exists(imageFilePath);
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
    lcd.fillScreen(TFT_BLACK);
    lcd.drawJpgFile(SPIFFS, "/sample.jpg", 0, 0);
  }
  return true;
}

// No Image
void printNoImageOnDisplay() {
  lcd.fillScreen(TFT_WHITE);
  lcd.setCursor(10,25);
  lcd.setFont(&fonts::Font0);
  lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  lcd.setTextSize(2);
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

// create image file (JPEG)
bool createJpgFile(std::string output) {
  bool result = true;
  try {
    File fp = SPIFFS.open(imageFilePath, FILE_WRITE);

    if (!fp) {
      result = false;
    } else {
      fp.write((const uint8_t*)output.c_str(), output.length());
      fp.close();
    }
  } catch (char *arg) {
    Serial.println(arg);
  }

  return result;
}

// add data image file (JPEG)
bool addDataJpgFile(std::string output) {
  File fp = SPIFFS.open(imageFilePath, FILE_APPEND);
  bool result = true;
  try {
    if (!fp) {
      result = false;
    } else {
      fp.write((const uint8_t*)output.c_str(), output.length());
      fp.close();
    }
  } catch (char *arg) {
    Serial.println(arg);
  }
  return result;
}

// callback class
class BLECallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    textMessage = pCharacteristic->getValue().c_str();
    printTextOnDisplay(binaryBmp);
  }
};

// imagefile size (end check)
int beforeSize = 0;
// image Callback class
class ImageCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *imageCharacteristic) override {
    std::string value = imageCharacteristic->getValue();
    bool result = true;
    if (value.length() > 0) {
      // memcpy(receivedData + receivedLength, value.c_str(), value.length());
      receivedLength += value.length();
      Serial.print("Receibed: ");
      Serial.print(value.length());
      Serial.print(" - bytes, Total: ");
      Serial.println(receivedLength);
      Serial.print("value data ");
      Serial.println(value.c_str());
    
      if (beforeSize > value.length()) {
        result = addDataJpgFile(value);
        Serial.println("file receive end!!");
        beforeSize = 0;
        feastCallBack = true;
        changeStatus = true;
      } else {
        beforeSize = value.length();
        if (feastCallBack) {
          result = createJpgFile(value);
          printTextOnDisplay("receiving now");
        } else {
          result = addDataJpgFile(value);
        }
        Serial.println(result);
        feastCallBack = false;
      }
    
    }
  }
};


void setup()
{
  // Serial Port init
  Serial.begin(115200);

  // spiffs init
  spiffsResponse = SPIFFS.begin(true);

  // display init
  lcd.init();
  lcd.setRotation(2);
  lcd.setBrightness(50);

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

  // Serial.println("Test Serial Message");

  bool result = false;
  result = fileExistanceCheck();
  if (result) {
    if (changeStatus) {
      printImageOnDisplay();
    }
    changeStatus = false;

    Serial.println("exist file.");
  } else {
    if (changeStatus) {
      printNoImageOnDisplay();
    }
    changeStatus = false;
    Serial.println("not exist file.");
  }
}