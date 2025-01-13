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

// BLE Library
#include <BLEDevice.h>
#include <BLEServer.h>

// Memory Library
#include <SPIFFS.h>

#define FORMAT_SPIFFS_IF_FAILED true
#define BLE_DEVICE_NAME "SmartPhoneForDolls"
#define SERVICE_UUID "b65ac29b-346b-4c47-a64c-c6785bb7800b"
#define CHARACTERISTIC_UUID "9b3f623b-9c48-4fca-840e-b43e6b4fc8e4"
#define LED_PIN 2

// BLE Static
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string rxValue;
std::string txValue;
bool bleOn = false;

void setup()
{
  // Serial Port init
  Serial.begin(19200);

  SPIFFS.begin(true);
  lcd.init();
  lcd.setRotation(3);
  lcd.fillScreen(0xFFFFFFU);
}

void loop() {

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  BLEDevice::init(BLE_DEVICE_NAME);

  // BLE Server setup
  pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // BLE Start
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  Serial.println("Test Serial Message");

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

// SPIFFS init
void writeImageFile(fs::FS &fs, const char *path)
{
  
}