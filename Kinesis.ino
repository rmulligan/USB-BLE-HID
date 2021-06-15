#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include <Wire.h>
#include <bluefruit.h>

#define KEY_PRESS_PIN 7

/**
 * Adafruit BLE classes
 */
BLEDis bledis;
BLEHidAdafruit blehid;

const int  adc_address=41;   // I2C Address
int flag_esc = 0;

bool key_pressed = false;

void setup()
{
  pinMode(KEY_PRESS_PIN, INPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Wire.begin();
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  initBluetooth();
  startAdv();
}

void loop() // run over and over//
{
  char char_count;
  int key_char;
  int pressSensor = digitalRead(KEY_PRESS_PIN);

  if (!key_pressed && pressSensor) {
    // Key Pressed
    key_pressed = true;
  } else if (key_pressed && !pressSensor) {
    // Key Released
    key_pressed = false;
    blehid.keyRelease();
  }

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

              blehid.keyPress(key_char);
              flag_esc=0;
            }
          else
            {
              // Skip 10 & 13
              if (key_char != 10 || key_char != 13) {
                blehid.keyPress(key_char);
              }
            }
        }
    }
}


void initBluetooth() {
  Bluefruit.begin();
  Bluefruit.setName("Kinesis Advantage 2");
  // Set bluetooth power - Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(-4);
  // On-board blue LED: On (true) / Off (false)
  Bluefruit.autoConnLed(true);

  bledis.setManufacturer("Mike Wu");
  bledis.setModel("Kinesis Advantage 2");

  bledis.begin();
  blehid.begin();
}

// BLE Advertising
void startAdv()
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();

  // Apple recommended advertising settings
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}
