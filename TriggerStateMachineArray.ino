#include <MIDIUSB2.h>
#define MIDI_CHANNEL 1
#define Threshold 150
#define MaxPressure 1000

#define NOTE_OFF 1
#define RISE_WAIT 2
#define NOTE_ON 3
#define AT_INTERVAL 40
#define RISE_TIME 3

const int pinCount = 4;

int sensorPins[pinCount] = {
  A0, A1, A2, A3
};
int midiChannel[pinCount] = {
  1, 2, 3, 4
};
int noteSounding[pinCount] = {
  36, 38, 42, 48
};
int sensorValue[pinCount] = {
  0, 0, 0, 0
};
int state[pinCount] = {
  0, 0, 0, 0
};
unsigned long breath_on_time[pinCount] = {
  0L, 0L, 0L, 0L
};
int initial_breath_value[pinCount] = {
  0, 0, 0, 0
};
int atVal[pinCount] = {
  0, 0, 0, 0
};
unsigned long atSendTime[pinCount] = {
  0L, 0L, 0L, 0L
};


void setup() {
  for (int i = 0; i < pinCount; i++) {
    state[i] = NOTE_OFF;
  }
}

int get_velocity(int initial, int final, unsigned long time_delta) {
  return map(final, Threshold, MaxPressure, 0, 127);
}

void loop() {
  for (int i = 0; i < pinCount; i++) {
    sensorValue[i] = analogRead(sensorPins[i]);
    if (state[i] == NOTE_OFF) {
      if (sensorValue[i] > Threshold) {
        breath_on_time[i] = millis();
        initial_breath_value[i] = sensorValue[i];
        state[i] = RISE_WAIT;
      }
    } else if (state[i] == RISE_WAIT) {
      if (sensorValue[i] > Threshold) {
        if (millis() - breath_on_time[i] > RISE_TIME) {
          int velocity = get_velocity(initial_breath_value[i], sensorValue[i], RISE_TIME);
          MidiUSB.sendNoteOn(noteSounding[i], velocity, MIDI_CHANNEL);
          state[i] = NOTE_ON;
        }
      } else {

        state[i] = NOTE_OFF;
      }
    } else if (state[i] == NOTE_ON) {
      if (sensorValue[i] < Threshold) {
        MidiUSB.sendNoteOff(noteSounding[i], 100, MIDI_CHANNEL);
        state[i] = NOTE_OFF;
      } else {
        if (millis() - atSendTime[i] > AT_INTERVAL) {
          atVal[i] = map(sensorValue[i], Threshold, 1023, 0, 127);
          MidiUSB.sendAfterTouch(atVal[i], MIDI_CHANNEL);
          atSendTime[i] = millis();
        }
      }
    }
  }
}
