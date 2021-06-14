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
              switch(key_char)
                {
                case 27:
                  Serial.println("[ESC]");
                  break;
               case 0x49:
                  Serial.println("[PgUp]");
                  break;
                case 0x51:
                  Serial.println("[PgDn]");
                  break;
                case 0x47:
                  Serial.println("[Home]");
                  break;
                case 0x4F:
                  Serial.println("[End]");
                  break;
                case 0x52:
                  Serial.println("[Ins]");
                  break;
                case 0x53:
                  Serial.println("[Del]");
                  break;
                case 0x3B:
                  Serial.println("[F1]");
                  break;
                case 0x3C:
                  Serial.println("[F2]");
                  break;
                case 0x3D:
                  Serial.println("[F3]");
                  break;
                case 0x3E:
                  Serial.println("[F4]");
                  break;
                case 0x3F:
                  Serial.println("[F5]");
                  break;
                case 0x40:
                  Serial.println("[F6]");
                  break;
                case 0x41:
                  Serial.println("[F7]");
                  break;
                case 0x42:
                  Serial.println("[F8]");
                  break;
                case 0x43:
                  Serial.println("[F9]");
                  break;
                case 0x44:
                  Serial.println("[F10]");
                  break;
                case 0x57:
                  Serial.println("[F11]");
                  break;
                case 0x58:
                  Serial.println("[F12]");
                  break;
                case 0x48:
                  Serial.println("[Up]");
                  break;
                case 0x50:
                  Serial.println("[Down]");
                  break;
                case 0x4B:
                  Serial.println("[Left]");
                  break;
                case 0x4D:
                  Serial.println("[Right]");
                  break;
                case 0x54:
                  Serial.println("[Println]");
                  break;
                case 0x5B:
                  Serial.println("[Windows]");
                  break;
                default:
                  Serial.println("[?]");
                  break;
                }
              flag_esc=0;
            }
          else
            {

              if(key_char==1)
                {
                  Serial.println("Control-A");
                }
              else if(key_char==2)
                {
                  Serial.println("Control-B");
                }
              else if(key_char==3)
                {
                  Serial.println("Control-C");
                }
              else if(key_char==4)
                {
                  Serial.println("Control-D");
                }
              else if(key_char==5)
                {
                  Serial.println("Control-E");
                }
              else if(key_char==6)
                {
                  Serial.println("Control-F");
                }
              else if(key_char==7)
                {
                  Serial.println("Control-G");
                }
              // Dont decode 8 - backspace
              else if(key_char==9)
                {
                  Serial.println("Tab");
                }
              // Dont decode 10 - Line Feed
              else if(key_char==11)
                {
                  Serial.println("Control-K");
                }
              else if(key_char==12)
                {
                  Serial.println("Control-L");
                }
              // Dont decode 13 - Carriage Return
              else if(key_char==14)
                {
                  Serial.println("Control-N");
                }
              else if(key_char==15)
                {
                  Serial.println("Control-O");
                }
              else if(key_char==16)
                {
                  Serial.println("Control-P");
                }
              else if(key_char==17)
                {
                  Serial.println("Control-Q");
                }
              else if(key_char==18)
                {
                  Serial.println("Control-R");
                }
              else if(key_char==19)
                {
                  Serial.println("Control-S");
                }
              else if(key_char==20)
                {
                  Serial.println("Control-T");
                }
              else if(key_char==21)
                {
                  Serial.println("Control-U");
                }
              else if(key_char==22)
                {
                  Serial.println("Control-V");
                }
              else if(key_char==23)
                {
                  Serial.println("Control-W");
                }
              else if(key_char==24)
                {
                  Serial.println("Control-X");
                }
              else if(key_char==25)
                {
                  Serial.println("Control-Y");
                }
              else if(key_char==26)
                {
                  Serial.println("Control-Z");
                }

              else
                {
                  // Its a normal key
                  Serial.write(key_char);
                }
            }
        }
    }
}
