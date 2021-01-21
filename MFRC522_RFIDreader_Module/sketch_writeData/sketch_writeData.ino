 /*
 *             MFRC522      Arduino       Arduino    
 *             Reader/PCD   Uno/101       Nano v3    
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             D9  
 * SPI SS      SDA(SS)      10            D10
 * SPI MOSI    MOSI         11            D11
 * SPI MISO    MISO         12            D12
 * SPI SCK     SCK          13            D13
 */

 
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          10          

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

String pass = "";
String key = "";
String vigenereCoded="";
void setup() {
  Serial.begin(9600);
  SPI.begin();               
  mfrc522.PCD_Init();        
}

char vigenere(char a, char key){
    char arr[]={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    int posA=-1;
    int posKey=-1;
    for(int i=0; i<(sizeof(arr)); i++){
        if(a==arr[i]){
            posA=i;
        }
        if(key==arr[i]){
            posKey=i;
        }
        if(posKey!=-1 && posA!=-1){
            break;
        }
    }
    return arr[(posA+posKey)%(sizeof(arr))];
}


boolean getInput(){
  
  Serial.println("start now? Press 1");
  
  //Section 1: getting the desired key and pass code  
  
  while(Serial.available() == 0){}
  String input = Serial.readString();  
  if(input=="1\n"){
    vigenereCoded="";
    Serial.println("Please enter the key?");
    while(Serial.available() == 0){}    
    key = Serial.readString();
    Serial.println("Please enter the pass? (less than 33 chars)");
    while(Serial.available() == 0){}  
    pass = Serial.readString();
    //in arduino .length() function returns the size of the string +1
    //length() in cpp   =      length() in arduino - 1
    //why? cause the function readString() also reads the \n in the end of the input
    for(int i=0; i<pass.length()-1; i++){
        vigenereCoded += vigenere(pass[i], key[i%(key.length()-1)]);
    }
    Serial.println("The Code:"+vigenereCoded);
    return true;
  }
  return false;
}
boolean flag = false;
void loop() {
  //Section 2: write passcode into the rfid cards/tags
  if(!flag){
    flag = getInput();
  }else{
      // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
  
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
  
    byte buffer[34];
    byte block;
    MFRC522::StatusCode status;
    vigenereCoded.getBytes(buffer, 34);
    block = 1;
    //Authenticate
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));
  
    // Write block
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));
  
    block = 2;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  
    // Write block
    status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    flag = false;
  }
  
}
