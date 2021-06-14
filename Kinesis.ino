#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <Wire.h>

#define KEY_PRESS_PIN 7

const int  adc_address=41;   // I2C Address

void setup()
{
  pinMode(KEY_PRESS_PIN, INPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Wire.begin();
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
}

void loop() // run over and over//
{
  char char_count;
  int key_char;

  Wire.beginTransmission(adc_address); // transmit to device
  Wire.write(1);                       // ask how many characters are in buffer
  Wire.endTransmission();              // stop transmitting
  Wire.requestFrom(adc_address, 1);    // request 1 bytes from slave device
  while(Wire.available())
    {
      char_count = Wire.read();
    }

  Wire.beginTransmission(adc_address); // transmit to device
  Wire.write(0);                       // get keyboard characters
  Wire.endTransmission();              // stop transmitting
  Wire.requestFrom(adc_address, char_count);    // request characters from slave device

  Serial.println(digitalRead(KEY_PRESS_PIN));

  while(Wire.available())
    {
      key_char = Wire.read();
      Serial.println(key_char);
    }
}
