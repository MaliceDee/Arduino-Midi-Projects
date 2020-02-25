#include <MIDIUSB2.h>
#define MIDI_CHANNEL 1
#define Threshold 150
#define MaxPressure 1000

#define NOTE_OFF 1
#define RISE_WAIT 2
#define NOTE_ON 3
#define AT_INTERVAL 40
#define RISE_TIME 3

#define sensorPin A0

int noteSounding;
int sensorValue;
int state;
unsigned long breath_on_time = 0L;
int initial_breath_value;
int atVal;
unsigned long atSendTime = 0L;


void setup() {
  state = NOTE_OFF;  
}

int get_velocity(int initial, int final, unsigned long time_delta) {
  return map(final, Threshold, MaxPressure, 0, 127);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  stateMachine();
  delay(10);
}

void stateMachine() {
  if (state == NOTE_OFF) {
    if (sensorValue > Threshold) {
      breath_on_time = millis();
      initial_breath_value = sensorValue;
      state = RISE_WAIT;  
    }
  } else if (state == RISE_WAIT) {
    if (sensorValue > Threshold) {
      if (millis() - breath_on_time > RISE_TIME) {
        noteSounding = get_note();
        int velocity = get_velocity(initial_breath_value, sensorValue, RISE_TIME);
        MidiUSB.sendNoteOn(noteSounding, velocity, MIDI_CHANNEL);
        state = NOTE_ON;
      }
    } else {

      state = NOTE_OFF;
    }
  } else if (state == NOTE_ON) {
    if (sensorValue < Threshold) {
      MidiUSB.sendNoteOff(noteSounding, 100, MIDI_CHANNEL);  
      state = NOTE_OFF;
    } else {
      if (millis() - atSendTime > AT_INTERVAL) {
        atVal = map(sensorValue, Threshold, 1023, 0, 127);
        MidiUSB.sendAfterTouch(atVal, MIDI_CHANNEL);
        atSendTime = millis();
      }
    }
    int newNote = get_note();
    if (newNote != noteSounding) {
      int velocity = map(sensorValue, Threshold, MaxPressure, 0, 127);
      MidiUSB.sendNoteOn(newNote, velocity, MIDI_CHANNEL);
      MidiUSB.sendNoteOff(noteSounding, 100, MIDI_CHANNEL);
      noteSounding = newNote;

    }
  }
}
int get_note() {
return 36;
}
