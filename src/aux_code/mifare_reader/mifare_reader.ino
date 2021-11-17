/**************************************************************************/
/*!
    @file     readMifare.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.

    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:

    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then read any of the
      4 blocks in that sector (though only block 4 is read here)

    If the card has a 7-byte UID it is probably a Mifare
    Ultralight card, and the 4 byte pages can be read directly.
    Page 4 is read by default since this is the first 'general-
    purpose' page on the tags.


  This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
  This library works with the Adafruit NFC breakout
  ----> https://www.adafruit.com/products/364

  Check out the links above for our tutorials and wiring diagrams
  These chips use SPI or I2C to communicate.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <EEPROM.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a software SPI connection (recommended):
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);


void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  nfc.begin();

  // configure board to read RFID tags
  nfc.SAMConfig();

  bool OVERWRITE = true;

  if (OVERWRITE) {
      int addr = 0;
  
      /*  CFG DETERMINES FLAGS STORED IN MEMORY:
        cfg is the decimal representation of the flag bits.
        Flag descriptions in project, summarized here:
        [0]  (0b0000), {"exit", "main"},
        [1]  (0b0001), {"exit", "fishbowl"},
        [2]  (0b0010), {"exit", "reception"},
        [3]  (0b0011), {"exit", "staircase"},
        [4]  (0b0100), {"enter", "main"},
        [5]  (0b0101), {"enter", "fishbowl"},
        [6]  (0b0110), {"enter", "reception"},
        [8]  (0b1000), {"pop"},
        [14] (0b1110), {"error"},
        [15] (0b1111), {"default"}
        (otherwise) INVALID
      */
 
     // CHANGE HERE
     int cfg = 8;

     ////////////////////////////////////////////////////////////////////////////////////
     // invalid flag, set to error
     if (cfg < 0 || cfg > 15) {
       cfg = 14;
     }

     // load memory
     EEPROM.write(addr, cfg); 
  }
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t flags;                            // Stores the flags read from EEPROM
  uint8_t addr = 0;                         // Address where flag byte is

  // Read flags
  flags = EEPROM.read(addr);

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  
  Serial.flush();
  if (success) {

    // Print bits
    // Force uidLength to 4 bytes (a bit hacky)
    uidLength = 4;
    for (int i = 0; i < uidLength; i++) {
      // Keep leading zeroes
      if (uid[i] < 2) Serial.print(B0);
      if (uid[i] < 4) Serial.print(B0);
      if (uid[i] < 8) Serial.print(B0);
      if (uid[i] < 16) Serial.print(B0);
      if (uid[i] < 32) Serial.print(B0);
      if (uid[i] < 64) Serial.print(B0);
      if (uid[i] < 128) Serial.print(B0);

      Serial.print(uid[i], BIN);
    }
    // Keep leading zeroes
    if (flags < 2) Serial.print(B0);
    if (flags < 4) Serial.print(B0);
    if (flags < 8) Serial.print(B0);
    Serial.print(flags, BIN);
  }

  delay(1000);
  Serial.flush();
}
