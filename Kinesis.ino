#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial softSerial(A0, A1); // RX, TX

char  hidReport[23];                // 00-00-00-00-00-00-00-00
byte inByte;
byte byteArray[8];
int   byteIndex;
bool  skipSeg = false;

const int  adc_address=41;   // I2C Address

char keyboard_data[80];   // Array to store keyboard values

/**
 * Raw Keycodes
 */
uint8_t modifier;
uint8_t key0;
uint8_t key1;
uint8_t key2;
uint8_t key3;
uint8_t key4;
uint8_t key5;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Wire.begin();
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
}

void loop() // run over and over//
{
  int keyboard_chars;

  Wire.beginTransmission(adc_address); // transmit to device
  Wire.write(1);                       // ask how many characters are in buffer
  Wire.endTransmission();              // stop transmitting
  Wire.requestFrom(adc_address, 1);    // request 1 bytes from slave device
  while(Wire.available())
    {
      keyboard_chars = Wire.read();
    }

  Wire.beginTransmission(adc_address); // transmit to device
  Wire.write(0);                       // get keyboard characters
  Wire.endTransmission();              // stop transmitting
  Wire.requestFrom(adc_address, keyboard_chars);    // request characters from slave device

  while(Wire.available())
    {
      keyboard_chars = Wire.read();
      Serial.println((int) keyboard_chars);
    }
}
