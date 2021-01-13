

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7,8); // CNS, CE
const byte address[6] = "00010";
void setup(){
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(115200);
}
void loop(){
  const char message[]= "Hello World!";
  radio.write(&message, sizeof(message));
  delay(1000);
  Serial.println("done");
}
