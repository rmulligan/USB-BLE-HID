#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <Wire.h>

#define KEY_PRESS_PIN 7

const int  adc_address=41;   // I2C Address
int flag_esc = 0;

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
  int key_depressed = digitalRead(KEY_PRESS_PIN);

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

  ///////////////////////////////////////////////////
  //
  //  We have a keystroke. Let's do something!
  //
  ///////////////////////////////////////////////////

  while(Wire.available())
    {
      key_char = Wire.read();
      if(key_char == 27 && flag_esc == 0)
        {
          flag_esc = 1;
        }
      else
        {
          if(flag_esc==1)
            {
              // Previous char was ESC - Decode all the escaped keys

              // Send Keypress
              //
              //  ble.keyPress();
              //


              flag_esc=0;
            }
          else
            {
             // Skip 10 & 13

             // Send keypress
            }
        }
    }
}
