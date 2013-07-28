/*
 * This sketch receives and decodes data from a 433MHz thermo weather sensor.
 *
 * Setup:
 * - Connect digital output of a 433MHz receiver to digital pin 2 of Arduino
 * - Enable the serial monitor at 115200 baud. 
 *
 */

#include <SpaceLen.h>

void setup() {
  Serial.begin(115200);

  // Init the receiver on interrupt pin 0 (digital pin 2).
  // Set the callback to function "showTempHumi", which is called
  // whenever valid sensor data has been received.
  SpaceLen::init(0, showRaw);
}

void loop() {
  // Empty! However, you can do other stuff here if you like.
  delay(30000);
  Serial.println("--mark--" );
}


void showRaw(byte *data) {
  SpaceLen::disable();
  if ( (data[3]) == 0xF0 ) {							
    int channel=((data[1]&0x30)>>4)+1;
    byte high=((data[1]&0x08)?0xf0:0)+(data[1]&0x0f);
    byte low=data[2];
    int temp10= high*256+low;
    Serial.print("Channel: ");
    Serial.print(channel);

    Serial.print(" Temp: ");
    Serial.print(temp10/10);
    Serial.print(".");
    Serial.print(abs((temp10)%10));
    Serial.print(" BYTE0: ");
    Serial.println(data[0],BIN);
  } else {
    if (false) {
      Serial.print(" B0: ");
      Serial.print(data[0],BIN);
      Serial.print(" ");
      Serial.print(data[1],BIN);
      Serial.print(" ");
      Serial.print(data[2],BIN);
      Serial.print(" "); 
      Serial.print(data[3],BIN);
      Serial.println(' ');
    }
  }
  SpaceLen::enable();
}


