/*
 * Read the values in the ID card of Universidad de Sevilla
 * ----------------------------------------------------------------------------
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

byte buffer[18];
byte block;
byte waarde[64][16];
MFRC522::StatusCode status;

MFRC522::MIFARE_Key key;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#define NR_KEYS 2

byte knownKeys[NR_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} //Last sector
};

void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card
    Serial.println(F("**********************************************************************"));
    Serial.println(F("**************************Reading US ID CARD**************************"));
    Serial.println(F("**********************************************************************"));


}

void getData(MFRC522::Uid uid){
  //Prepare the first key
  byte sectors = 0;
  for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
    key.keyByte[i] = knownKeys[0][i];
  }
  
  for(sectors = 0; sectors < 15; sectors++){
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(uid),&(key),sectors);
  }

  for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
    key.keyByte[i] = knownKeys[1][i];
  }
  
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(uid),&(key),sectors);
  
}

void loop() {
  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
    Serial.println("Found card, trying to gather data");
    getData(mfrc522.uid);
  }
}
