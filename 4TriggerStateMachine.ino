#include <MIDIUSB2.h>
#define MIDI_CHANNEL1 1
#define MIDI_CHANNEL2 2
#define MIDI_CHANNEL3 3
#define MIDI_CHANNEL4 4

#define Threshold 150
#define MaxPressure 1000

#define NOTE_OFF 1
#define RISE_WAIT 2
#define NOTE_ON 3
#define AT_INTERVAL 40
#define RISE_TIME 3

#define sensorPin1 A0
#define sensorPin2 A1
#define sensorPin3 A2
#define sensorPin4 A3


int noteSounding1;
int sensorValue1;
int state1;
unsigned long breath_on_time1 = 0L;
int initial_breath_value1;
int atVal1;
unsigned long atSendTime1 = 0L;

int noteSounding2;
int sensorValue2;
int state2;
unsigned long breath_on_time2 = 0L;
int initial_breath_value2;
int atVal2;
unsigned long atSendTime2 = 0L;

int noteSounding3;
int sensorValue3;
int state3;
unsigned long breath_on_time3 = 0L;
int initial_breath_value3;
int atVal3;
unsigned long atSendTime3 = 0L;

int noteSounding4;
int sensorValue4;
int state4;
unsigned long breath_on_time4 = 0L;
int initial_breath_value4;
int atVal4;
unsigned long atSendTime4 = 0L;

void setup() {
  state1 = NOTE_OFF;  
  state2 = NOTE_OFF;  
  state3 = NOTE_OFF;  
  state4 = NOTE_OFF;  

}

int get_velocity(int initial, int final, unsigned long time_delta) {
  return map(final, Threshold, MaxPressure, 0, 127);
}

void loop() {
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);
  sensorValue4 = analogRead(sensorPin4);

  stateMachine1();
  stateMachine2();
  stateMachine3();
  stateMachine4();

  delay(10);
}

void stateMachine1() {
  if (state1 == NOTE_OFF) {
    if (sensorValue1 > Threshold) {xzx
      breath_on_time1 = millis();
      initial_breath_value1 = sensorValue1;
      state1 = RISE_WAIT;  
    }
  } else if (state1 == RISE_WAIT) {
    if (sensorValue1 > Threshold) {
      if (millis() - breath_on_time1 > RISE_TIME) {
        noteSounding1 = get_note1();
        int velocity = get_velocity(initial_breath_value1, sensorValue1, RISE_TIME);
        MidiUSB.sendNoteOn(noteSounding1, velocity, MIDI_CHANNEL1);
        state1 = NOTE_ON;
      }
    } else {

      state1 = NOTE_OFF;
    }
  } else if (state1 == NOTE_ON) {
    if (sensorValue1 < Threshold) {
      MidiUSB.sendNoteOff(noteSounding1, 100, MIDI_CHANNEL1);  
      state1 = NOTE_OFF;
    } else {
      if (millis() - atSendTime1 > AT_INTERVAL) {
        atVal1 = map(sensorValue1, Threshold, 1023, 0, 127);
        MidiUSB.sendAfterTouch(atVal1, MIDI_CHANNEL1);
        atSendTime1 = millis();
      }
    }
  }
}
void stateMachine2() {
  if (state2 == NOTE_OFF) {
    if (sensorValue2 > Threshold) {
      breath_on_time2 = millis();
      initial_breath_value2 = sensorValue2;
      state2 = RISE_WAIT;  
    }
  } else if (state2 == RISE_WAIT) {
    if (sensorValue2 > Threshold) {
      if (millis() - breath_on_time2 > RISE_TIME) {
        noteSounding2 = get_note2();
        int velocity = get_velocity(initial_breath_value2, sensorValue2, RISE_TIME);
        MidiUSB.sendNoteOn(noteSounding2, velocity, MIDI_CHANNEL2);
        state2 = NOTE_ON;
      }
    } else {

      state2 = NOTE_OFF;
    }
  } else if (state2 == NOTE_ON) {
    if (sensorValue2 < Threshold) {
      MidiUSB.sendNoteOff(noteSounding2, 100, MIDI_CHANNEL2);  
      state2 = NOTE_OFF;
    } else {
      if (millis() - atSendTime2 > AT_INTERVAL) {
        atVal2 = map(sensorValue2, Threshold, 1023, 0, 127);
        MidiUSB.sendAfterTouch(atVal2, MIDI_CHANNEL2);
        atSendTime2 = millis();
      }
    }
  }
}
void stateMachine3() {
  if (state3 == NOTE_OFF) {
    if (sensorValue3 > Threshold) {
      breath_on_time3 = millis();
      initial_breath_value3 = sensorValue3;
      state3 = RISE_WAIT;  
    }
  } else if (state3 == RISE_WAIT) {
    if (sensorValue3 > Threshold) {
      if (millis() - breath_on_time3 > RISE_TIME) {
        noteSounding3 = get_note3();
        int velocity = get_velocity(initial_breath_value3, sensorValue3, RISE_TIME);
        MidiUSB.sendNoteOn(noteSounding3, velocity, MIDI_CHANNEL3);
        state3 = NOTE_ON;
      }
    } else {

      state3 = NOTE_OFF;
    }
  } else if (state3 == NOTE_ON) {
    if (sensorValue3 < Threshold) {
      MidiUSB.sendNoteOff(noteSounding3, 100, MIDI_CHANNEL3);  
      state3 = NOTE_OFF;
    } else {
      if (millis() - atSendTime3 > AT_INTERVAL) {
        atVal3 = map(sensorValue3, Threshold, 1023, 0, 127);
        MidiUSB.sendAfterTouch(atVal3, MIDI_CHANNEL3);
        atSendTime3 = millis();
      }
    }
  }
}
void stateMachine4() {
  if (state4 == NOTE_OFF) {
    if (sensorValue4 > Threshold) {
      breath_on_time4 = millis();
      initial_breath_value4 = sensorValue4;
      state4 = RISE_WAIT;  
    }
  } else if (state4 == RISE_WAIT) {
    if (sensorValue4 > Threshold) {
      if (millis() - breath_on_time4 > RISE_TIME) {
        noteSounding4 = get_note4();
        int velocity = get_velocity(initial_breath_value4, sensorValue4, RISE_TIME);
        MidiUSB.sendNoteOn(noteSounding4, velocity, MIDI_CHANNEL4);
        state4 = NOTE_ON;
      }
    } else {

      state4 = NOTE_OFF;
    }
  } else if (state4 == NOTE_ON) {
    if (sensorValue4 < Threshold) {
      MidiUSB.sendNoteOff(noteSounding4, 100, MIDI_CHANNEL4);  
      state4 = NOTE_OFF;
    } else {
      if (millis() - atSendTime4 > AT_INTERVAL) {
        atVal4 = map(sensorValue4, Threshold, 1023, 0, 127);
        MidiUSB.sendAfterTouch(atVal4, MIDI_CHANNEL4);
        atSendTime4 = millis();
      }
    }
  }
}

int get_note1() {
return 36;
}
int get_note2() {
return 38;
}
int get_note3() {
return 42;
}
int get_note4() {
return 48;
}
