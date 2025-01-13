// Arduino Library
#include <Arduino.h>

// LovyanGFX Library
#include <LGFX.cpp>
#include <SPIFFS.h>

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

// // BLE Library
// #include <BLEDevice.h>
// #include <BLEServer.h>

// // Memory Library
// #include <SPIFFS.h>

// #define FORMAT_SPIFFS_IF_FAILED true
// #define BLE_DEVICE_NAME "SmartPhoneForDolls"
// #define SERVICE_UUID "b65ac29b-346b-4c47-a64c-c6785bb7800b"
// #define CHARACTERISTIC_UUID "9b3f623b-9c48-4fca-840e-b43e6b4fc8e4"
// #define LED_PIN 2

// BLE Static
// BLEServer *pServer = NULL;
// BLECharacteristic *pCharacteristic = NULL;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;
// std::string rxValue;
// std::string txValue;
// bool bleOn = false;

// LovyanGFX define

// // receive Image
// struct receiveImage
// {
//     File Image;
// };

// // SPIFFS READ
// void raedImageFile(fs::FS &fs, const char *path)
// {
//     File file = fs.open(path);

//     while (file.available())
//     {
//         // TODO
//     }
// }

// // LGFX instance set
// LGFX display;
// // setup
// void setup(void)
// {
//     // pinMode(LED_PIN, OUTPUT);
//     // Serial.begin(115200);

//     // BLEDevice::init(BLE_DEVICE_NAME);

//     // // BLE Server setup
//     // pServer = BLEDevice::createServer();

//     // BLEService *pService = pServer->createService(SERVICE_UUID);

//     // // BLE Start
//     // pService->start();
//     // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//     // pAdvertising->addServiceUUID(SERVICE_UUID);
//     // pAdvertising->setScanResponse(false);
//     // pAdvertising->setMinPreferred(0x0);
//     // BLEDevice::startAdvertising();


//     static lgfx::Panel_ST7735S panel;
//     display.setPanel(&panel);

//     // LGFX_config
//     display.init();
//     display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);
//     display.fillScreen(TFT_BLACK);
// }
// // // SPIFFS WRITE
// // void writeImageFile(fs::FS &fs, const char *path)
// // {
// // }

// uint32_t count = ~0;
// // loop
// void loop(void)
// {
//     display.startWrite();
//     display.setRotation(++count & 7);
//     display.setColorDepth((count & 8) ? 16 : 24);

//     display.setTextColor(TFT_WHITE);
//     display.drawNumber(display.getRotation(), 16, 0);

//     display.setTextColor(0xFF0000U);
//     display.drawString("R", 0, 16);
//     display.setTextColor(0x00FF00U);
//     display.drawString("G", 10, 16);
//     display.setTextColor(0x0000FFU);
//     display.drawString("B", 20, 16);

//     display.drawRect(30, 30, display.width() - 60, display.height() - 60, count * 7);
//     display.drawFastHLine(0, 0, 10);

//     display.endWrite();
// }
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

static LGFX lcd;
static LGFX lcdIn;
static LGFX_Sprite sprite[10];

void setup()
{
  SPIFFS.begin(true);
  lcd.init();
  lcd.setRotation(3);
  lcd.fillScreen(0xFFFFFFU);
}

void loop() {
  lcd.fillScreen(TFT_WHITE);
  delay(1000);
  lcd.fillScreen(TFT_RED);
  delay(1000);
  lcd.fillScreen(TFT_GREEN);
  delay(1000);
  lcd.fillScreen(TFT_BLUE);
  delay(1000);
  lcd.fillScreen(TFT_BLACK);
  delay(1000);
}

void createFile() {

}