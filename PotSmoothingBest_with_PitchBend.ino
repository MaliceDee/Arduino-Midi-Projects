/* Smoothing, Debugging and Sending Midi with Analog Inputs
 * 
 */
#include <MIDIUSB2.h>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Array Constants
const int inCount = 2; // Array counter (how many ins are being used) 
// Array Variables
int inPins[inCount] = {
  A0, A1               // input pins for array
};
int analogVal[inCount] = {
  0, 0                // the raw value from the analog inputs
};
int smoothed[inCount] = {
  0, 0                // the smoothed values to be mapped to midi 
};
int alpha[inCount] = {
  4, 4                // the number of past samples to average by
};
static long lastVal[inCount] = {
  0L, 0L             // previous values to compare current values to 
};
//ToDo; Make array for mapping parameters and test more pots on the ins 
//Pitchbend slider is pretty smooth, 10k slider doesn't even need smoothing
//have some 10k rotary pots to test, softpot to test, wearable softpot, etc 
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
//Uncomment for debugging 
//Serial.begin(9600);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
//Begin array
  for (int i = 0; i < inCount; i++) {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Input Smoothing, to use values call; smoothed[0],smoothed[1],smoothed[2],etc, 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // read the pots, use it to generate alpha:
    alpha[i] = (analogRead(inPins[i]) / 114) + 1;
    // get an analog reading:
    analogVal[i] = analogRead(inPins[i]);
    // Smooth out an analog reading:
    if (analogVal[i] > smoothed[i]) {
      smoothed[i] = smoothed[i] + (analogVal[i] - smoothed[i]) / alpha[i];
    } else {
      smoothed[i] = smoothed[i] - (smoothed[i] - analogVal[i]) / alpha[i];
    } // end of smoothing code 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Map Midi Values 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//(0-16383). (8192) is the default / middle value for pitchbend. 
//(0-127). for CC messages / If sending notes; (36)=C1; (48)=C2; (60)=C3; etc 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//int pbVal = map(analogVal[0], 0, 1023, 0, 16383); 
    int pbVal = map(smoothed[0], 0, 1015, 0, 127);
    int ccVal = map(analogVal[1], 0, 1023, 0, 127);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Calibrate Pitchbend centre here
    if  (pbVal == 63) {       //Center offset to compensate for bad values 
      pbVal = pbVal + 1;      //Sometimes middle indent lands a value too low 
    }                         
    if (pbVal > 64) {         //If value is greater than pb center
      pbVal = map(pbVal, 64, 127, 8192, 16383);
    } else if (pbVal < 64) {  //If value is lower than pb center 
      pbVal = map(pbVal, 0, 64, 0, 8192);
    } else {                  //Otherwise pitchbend = pb center 
      pbVal = 8192;           //(down = 0)(Center = 8192)(up = 16383)
    } 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//if value is greater than previous value
    if (smoothed[i] != lastVal[i]) { 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Debug values;
//      debug1();
//      Serial.print("; pbVal; ");
//      Serial.print(pbVal);
//      Serial.print(" } { ");
//      debug2();
//      Serial.print("; ccVal; ");
//      Serial.print(ccVal);
//      Serial.println(" ~ ");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Send Midi;
      MidiUSB.sendPitchBend(pbVal, 1);
      MidiUSB.sendControlChange(1, ccVal, 1);
      lastVal[i] = smoothed[i];
      delay(10);
    } //If
  } //For
} //Loop
//Debugging
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void debug1() {
      Serial.print(" smoothed1; ");
      Serial.print(smoothed[0]);
      Serial.print("; raw1; ");
      Serial.print(analogVal[0]);     
}
void debug2() {
      Serial.print(" smoothed2; ");
      Serial.print(smoothed[1]);
      Serial.print("; raw2; ");
      Serial.print(analogVal[1]);
}
//void debug3() {
//      Serial.print(" smoothed3; ");
//      Serial.print(smoothed[2]);
//      Serial.print("; raw3; ");
//      Serial.print(analogVal[2]);
//}
//void debug4() {
//      Serial.print(" smoothed4; ");
//      Serial.print(smoothed[3]);
//      Serial.print("; raw4; ");
//      Serial.print(analogVal[3]);
//}
//void debug5() {
//      Serial.print(" smoothed5; ");
//      Serial.print(smoothed[4]);
//      Serial.print("; raw5; ");
//      Serial.print(analogVal[4]);
//}
//void debug6() {
//      Serial.print(" smoothed6; ");
//      Serial.print(smoothed[5]);
//      Serial.print("; raw6; ");
//      Serial.print(analogVal[5]);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
