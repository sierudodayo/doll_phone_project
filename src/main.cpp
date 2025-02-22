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
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

// Memory Library
#include <SPIFFS.h>
#include <FS.h>

// Image Library
#include <BITMAPDecoder.h>
BITMAPDecoder bitmap = BITMAPDecoder();

#define FORMAT_SPIFFS_IF_FAILED true
#define BLE_DEVICE_NAME "SmartPhoneForDolls"
#define SERVICE_UUID "b65ac29b-346b-4c47-a64c-c6785bb7800b"
#define CHARACTERISTIC_UUID "9b3f623b-9c48-4fca-840e-b43e6b4fc8e4"
#define LED_PIN 2

String imageFilePath = "/sample.bmp";

bool spiffsResponse = true;

// image file reception
bool receptImageFile() {
  bool receptImage = false;
    if (SerialBT.available() > 0) {
    int i = SerialBT.read();
    if (i == 'f') {
      File fp;
      fp = SPIFFS.open(imageFilePath, "r");
      bitmap.checkFile( fp );
      for (int Y=0; Y<bitmap.height(); Y++) {
        for(int X=0; X<bitmap.width(); X++) {
          PIXEL p = bitmap.readPixel( fp, X, Y);
          int hex = p.r * 65535 + p.g * 256 + p.b;
          SerialBT.println(hex, HEX);
          for (char i; i!= 'r';) {
            i = SerialBT.read();
          }
        }
      }
      receptImage = true;
      fp.close();
    }
  }

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

void printNoImageOnDisplay() {
  lcd.fillScreen(TFT_WHITE);
  lcd.setCursor(10,25);
  lcd.setFont(&fonts::Font0);
  lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  lcd.setTextSize(3);
  lcd.print("No Image");
}


void setup()
{
  // Serial Port init
  Serial.begin(19200);

  // spiffs init
  spiffsResponse = SPIFFS.begin(true);

  // display init
  lcd.init();
  lcd.setRotation(3);
}

void loop() {

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  Serial.println("Test Serial Message");

  bool result = receptImageFile();
  if (result) {
    printImageOnDisplay();
  } else {
    printNoImageOnDisplay();
  }
}