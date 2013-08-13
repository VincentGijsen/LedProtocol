/**
*  This sketch prints-out the data received from Chinese led-controllers
*/

#include <LedProtocol.h>

#define RECEIVER P1_4

volatile byte *data;
volatile bool FLAGDATAREADY = false;

int counter;
void setup() {
  Serial.begin(9600);

  LedProtocol::init(RECEIVER, showRaw);
  Serial.println("started");
  attachInterrupt(RECEIVER, interruptHandler, RISING);
  
}

void interruptHandler(){
  //make rising/falling work alas "CHANGE"
  uint8_t bit = digitalPinToBitMask(RECEIVER);
  P1IES ^= bit;

  LedProtocol::interruptHandler();
}


void loop() {
  // Empty! However, you can do other stuff here if you like.
  keepAlive();
  delay(10);
  if (FLAGDATAREADY){
    FLAGDATAREADY=false;
    LedProtocol::disable();
    Serial.print(" GOT: ");
    Serial.print(data[0],BIN);
    Serial.print(" ");
    Serial.print(data[1],BIN);
    Serial.print(" ");
    Serial.print(data[2],BIN);
    Serial.print(" "); 
    Serial.print(data[3],BIN);
    Serial.println(' ');
    
    LedProtocol::enable();
    }
}

void keepAlive(){
 counter++;
if (counter == 0xFF){
   counter = 0;
  Serial.println("Mark"); 
}
  
}

void showRaw(byte *d) {
 data = d;
 FLAGDATAREADY = true;
}


