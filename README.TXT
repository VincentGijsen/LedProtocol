LedProtocol

This slightly modified version of SpaceLen library by Randy Simmons lets you read (and control in a bit), Led controllers
Modifications by Vincent Gijsen (http://www.vincentgijsen.nl)


SpaceLen library v1.0.0 (20130122) for Arduino 1.0
Made by Randy Simons http://randysimons.nl/ adopted for yt weather station


--initial version.

this code was written for ENERGIA rather than ARDUINO, as the MSP chips don't have the 'change' event-type, it is implemented in software in the latets commits (as of ~5 aug 2013).

Therefore you'll find some workaround which calls the interuptHandler inside the library. 

For arduino users, just change the addInterupt to utilize 'change' and strip out the not(bit) P1IES stuff int he interuptHandler in the example code.


