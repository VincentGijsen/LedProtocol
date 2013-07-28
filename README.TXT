SpaceLen library v1.0.0 (20130122) for Arduino 1.0
Made by Randy Simons http://randysimons.nl/ adopted for yt weather station


This software is based on the work of Oopsje. See docs/CrestaProtocol.pdf

License: GPLv3. See ./RemoteSensor/license.txt

Latest source and wiki: https://bitbucket.org/fuzzillogic/433mhzforarduino


Installation of library:
 - Make sure Arduino is closed
 - Copy the directory RemoteSensor to the Arduino library directory (usually
   <Sketchbook directory>/libraries/)
   See http://arduino.cc/en/Guide/Libraries for detailed instructions.
 
Default installation sender & demo:
 - Connect tha data-in-pin of a 433MHz transmitter to digital pin 11. See 
   docs/hardware setup.jpg.
   (Note: your hardware may have a different pin configuration!)
 - Start Arduino, and open the example: File -> Examples -> RemoteSensor ->
   ThermoHygroTransmitter
 - Alter addresses/devices to reflect your own setup. Otherwise nothing will
   happen.
 - Compile, upload and run!

Default installation receiver & demo:
 - Connect the data-out-pin of a 433MHz receiver to digital pin 2. See photo.
  (Note: your hardware may have a different pin configuration!)
 - Start Arduino, and open the example: File -> Examples -> RemoteSensor ->
   ThermoHygroReceiver
 - Compile, upload and run
 - Open serial monitor in Arduino (115200 baud)
 - Wait for your 433MHz-sensor to transmit (less than 1 minute), and watch the
   serial monitor




RemoteSensor library v1.0.0 (20130122) for Arduino 1.0.2
 - Initial version