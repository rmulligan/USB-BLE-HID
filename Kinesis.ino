#include <Arduino.h>
#include <bluefruit.h>

/**
 * Adafruit BLE classes
 */
BLEDis bledis;
BLEHidAdafruit blehid;

byte inByte;
char hidReport[23];
int  byteIndex;
bool skipSeg = false;

/**
 * Raw Keycodes
 */
uint8_t modifier;
uint8_t keys[6];

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);
  initBluetooth();
  startAdv();
}

void loop() // run over and over//
{
  if (Serial1.available() > 0) {
    inByte = Serial1.read();

    Serial.write(inByte);

    if ((int) inByte == 10) {
      skipSeg = false;
    }

    // The USB Host sends line breaks which are 10 & 13 in decimal
    if ((int) inByte != 10 && (int) inByte != 13 && skipSeg != true) {
      // Only care about first 23 bytes
      if (byteIndex < 23) {
        hidReport[byteIndex] = (char) inByte;
      }

      byteIndex ++;

      if (byteIndex == 23) {
        assignKeyCodes(hidReport);
        sendReport(modifier, keys);
        // Reset and move on to next key segment
        byteIndex = 0;
        memset(hidReport, 0, 23);
        skipSeg = true;
      }
    }
  }
  __WFI();
}

/**
 * Takes a hex code in string format
 * and converts it into a raw byte
 * of the same hex value
 */
uint8_t stringToHex(char* keyCode) {
  uint8_t hexByte = (uint8_t) strtoul(keyCode, NULL ,16);
  return hexByte;
}

/**
 * Parse HID Report array and assigns
 * respective hex bytes to variables
 */
void assignKeyCodes(char* hidReport) {

  char keyCode[2];

  // Modifier
  keyCode[0] = hidReport[0];
  keyCode[1] = hidReport[1];
  modifier = stringToHex(keyCode);

  //Key0
  keyCode[0] = hidReport[6];
  keyCode[1] = hidReport[7];
  keys[0] = stringToHex(keyCode);

  //Key1
  keyCode[0] = hidReport[9];
  keyCode[1] = hidReport[10];
  keys[1] = stringToHex(keyCode);

  //Key2
  keyCode[0] = hidReport[12];
  keyCode[1] = hidReport[13];
  keys[2] = stringToHex(keyCode);

  //Key3
  keyCode[0] = hidReport[15];
  keyCode[1] = hidReport[16];
  keys[3] = stringToHex(keyCode);

  //Key4
  keyCode[0] = hidReport[18];
  keyCode[1] = hidReport[19];
  keys[4] = stringToHex(keyCode);

  //Key5
  keyCode[0] = hidReport[21];
  keyCode[1] = hidReport[22];
  keys[5] = stringToHex(keyCode);
}

void sendReport(uint8_t modifier, uint8_t* keys){
  blehid.keyboardReport(modifier, keys);
}

void initBluetooth() {
  Bluefruit.begin();
  Bluefruit.setName("Kinesis Advantage 2");
  // Set bluetooth power - Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(-4);
  // On-board blue LED: On (true) / Off (false)
  Bluefruit.autoConnLed(true);

  bledis.setManufacturer("Ryan Mulligan");
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
