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

void setup() {
  Serial.begin(9600);     // initialize the comunicacion for monitor serie at 9600 bps
  SPI.begin();        // initialize SPI bus 
  mfrc522.PCD_Init();     // initialize the Module
  Serial.print("Program Runing");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) // if no card is detected
    return;         
  
  if ( ! mfrc522.PICC_ReadCardSerial())   // if cannot detect any data from the card
    return;         
    
  Serial.print("UID:");       // print "UID: " text in the serial monitor
  for (byte i = 0; i < mfrc522.uid.size; i++) { // loop through each byte of the UID
    Serial.print(mfrc522.uid.uidByte[i], HEX);  // print the byte in hex-mode
  } 
  Serial.println();       
  mfrc522.PICC_HaltA();     //end communication with the card              
}
