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

#include <LedProtocol.h>

byte LedProtocol::halfBit = 0;
word LedProtocol::clockTime;
boolean LedProtocol::isOne;
unsigned long LedProtocol::lastChange=0;
SpaceLenCallback LedProtocol::callback;
byte LedProtocol::data[14];
word LedProtocol::duration;
boolean LedProtocol::enabled;
int LedProtocol::markerState;
int8_t _interrupt;

boolean LedProtocol::packagestart;

void LedProtocol::init(short int interrupt, SpaceLenCallback callbackIn) {
    callback = callbackIn;
    enable();
    _interrupt = interrupt;
    
    if (interrupt >= 0) {
        //attachInterrupt(interrupt, interruptHandler, RISING);
    }
}


void LedProtocol::interruptHandler() {
    //interrupts();
	if (!enabled) {
		return;
	}
        
	/*
     The bits are encoded in the spacelength.
     The package starts with a space of minimal 5ms.
     the markers are between 400 and 500 uS.
     After a markerker, the space is ~700uS for a zero
     After a marker (500uS) for 'One' a space of 1700uS follows
     
     */
	unsigned long currentTime=micros();
	duration=currentTime-lastChange; // Duration = Time between edges
	lastChange=currentTime;
    
	int state=digitalRead(_interrupt);
    
	// A package starts with a long space but it's detected afterwards. that's why the state is the markerstate.
	if ( duration > 3000 ) {
		packagestart=true;
		halfBit=0;
		markerState=state;
		clockTime=0;
        //Serial.print(',');
        
	} else {
		if ( packagestart ) {
			if ( duration > ( 4800 )) { // first space is ~4.5ms long
				//error
				packagestart=true;
				clockTime=0;
				halfBit=0;
				//Serial.println('e');
				return;
			}
            
			if ( state != markerState) {
				//capture the first package for the clockpulselength
				if ( clockTime==0 ) {
					// the first pulse will be a clock pulse
					clockTime=duration;
					if ( clockTime < 300 or clockTime > 1300 ) { //somewhere 400 for 1, 500 for 0 pulse width
						reset();
						return;
					}
				}
				if ( duration > ( 2400 )) {
					// should be the same as all markers
					reset();
					//Serial.print(duration);
					//Serial.println('!');
					return;
				}
			} else {
				// if the pulse is near 1x the clock it is a 0
                //eg    2.2mS        500       * 4
				isOne=( duration < ( 1200 ))?false:true; // 500 uS *4
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

void LedProtocol::reset() {
	packagestart=false;
	halfBit=0;
}


void LedProtocol::enable() {
	halfBit = 0;
	packagestart=false;
	enabled = true;
}

void LedProtocol::disable() {
	enabled = false;
}
