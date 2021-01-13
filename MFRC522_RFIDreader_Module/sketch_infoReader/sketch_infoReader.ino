/*
  MFRC522       Arduino Nano      Arduino Uno
  3.3 v         3.3v              3.3v
  GND           GND               GND 
  IRQ           -                 -           // this pin won't be used
  RST           9                 D9
  SPI SS        10                D10        
  SPI MOSI      11                D11        
  SPI MISO      12                D12        
  SPI SCK       13                D13        
*/



#include <SPI.h>      //include SPI library
#include <MFRC522.h>      // include the MFRC522, link in the info.txt file

#define RST_PIN  9       
#define SS_PIN  10      

MFRC522 mfrc522(SS_PIN, RST_PIN); //init mfrc522 objet 
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);     // initialize the comunicacion for monitor serie at 9600 bps
  SPI.begin();        // initialize SPI bus 
  mfrc522.PCD_Init();     // initialize the Module


  // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    //if you have changed your card keys, you want to change the value of variable key to your specific key
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

}

void loop() {   
  if ( ! mfrc522.PICC_IsNewCardPresent()) // if no card is detected
    return;         
  
  if ( ! mfrc522.PICC_ReadCardSerial())   // if cannot detect any data from the card
    return;         


  // Show some details of the PICC (that is: the tag/card)
  
  Serial.println(F("        THE BASIC INFO"));
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));


  Serial.println(F("      TRYING TO READ CONTAINING INFO"));
  
  readUsingSector();
}


void readUsingSector(){
  // Show the whole sector as it currently is
  for ( int i =0; i<16; i++){
    Serial.print(F("Current data in sector "));
    Serial.print(i);
    Serial.println(":");
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, i);
    Serial.println();
  }
  mfrc522.PICC_HaltA();     //end communication with the card  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();     
  
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
