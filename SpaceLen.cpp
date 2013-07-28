/*
 * RemoteSensor v1.0.1 (20120213)
 *
 * This library encodes, encrypts en transmits data to
 * remote weather stations made by Hideki Electronics..
 * 
 * Copyright 2011 by Randy Simons http://randysimons.nl/
 *
 * Parts of this code based on Oopsje's CrestaProtocol.pdf, for which
 * I thank him very much!
 * 
 * License: GPLv3. See license.txt
 */

#include <SpaceLen.h>

byte SpaceLen::halfBit = 0;
word SpaceLen::clockTime;
boolean SpaceLen::isOne;
unsigned long SpaceLen::lastChange=0;
SpaceLenCallback SpaceLen::callback;
byte SpaceLen::data[14];
word SpaceLen::duration;
boolean SpaceLen::enabled;
int SpaceLen::markerState;

boolean SpaceLen::packagestart;

void SpaceLen::init(short int interrupt, SpaceLenCallback callbackIn) {
  callback = callbackIn;
  enable();
  
  if (interrupt >= 0) {
	attachInterrupt(interrupt, interruptHandler, CHANGE);
  }  
}

void SpaceLen::interruptHandler() {	
	if (!enabled) {
		return;
	}

	/* 
	The bits are encoded in the spacelength.
	The package starts with a space of minimal 5ms.
	the markers are around 350us.
	After every marker a space follows of 990us for 0 and 1900 for 1.
	the fluctuation was terrible because some '1's were 1200.
	*/
	unsigned long currentTime=micros();
	duration=currentTime-lastChange; // Duration = Time between edges
	lastChange=currentTime;

	int state=digitalRead(2);

	// A package starts with a long space but it's detected afterwards. that's why the state is the markerstate.	
	if ( duration > 4000 ) {
		packagestart=true;
		halfBit=0;
		markerState=state;
		clockTime=0;
	} else {
		if ( packagestart ) {
			if ( duration > ( 4000 )) {
				//error
				packagestart=true;
				clockTime=0;
				halfBit=0;
				//Serial.println('.');
				return;
			}

			if ( state != markerState) {
				//capture the first package for the clockpulselength
				if ( clockTime==0 ) {
					// the first pulse will be a clock pulse
					clockTime=duration;
					if ( clockTime < 300 or clockTime > 1300 ) {
						reset();
						return;
					}
				} 
				if ( duration > ( 2000 )) {
					// should be the same as all markers
					reset();
					//Serial.print(duration);
					//Serial.println('!');
					return;						
				}  
			} else { 
				// if the pulse is near 1x the clock is't a 0
				isOne=( duration < ( clockTime * 4 ))?false:true; //1060
				//Serial.print(isOne?'1':'0');
				byte currentByte = halfBit / 8;
				byte currentBit = 7-(halfBit  % 8); 
				if (currentBit < 8) {
					if (isOne) {
						// Set current bit of current byte
						data[currentByte] |= 1 << currentBit;
					} 
						else {
					  // Reset current bit of current byte
					  data[currentByte] &= ~(1 << currentBit);
					}
				} 
				// the weatherstation package is 32 bits
				if ( halfBit==31 ) { 	
					//Serial.println("Complete:");
					//Serial.print("Clock: ");
					//Serial.print(clockTime);
					(callback)(data);	
					reset();
					return;
				}
				// we don't need longer packages
				if ( halfBit > 32 ) {
					reset();
					return;
				}
				halfBit++;
			}		
		}
	}
	return;
}

void SpaceLen::reset() {
	packagestart=false;
	halfBit=0;
}


void SpaceLen::enable() {
	halfBit = 0;
	packagestart=false;
	enabled = true;
}

void SpaceLen::disable() {
	enabled = false;
}
