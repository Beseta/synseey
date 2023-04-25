#include <MIDI.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=151,252
AudioOutputI2S           audioOutput;           //xy=539,229
AudioConnection          patchCord1(waveform1, 0, audioOutput, 0);
AudioConnection          patchCord2(waveform1, 0, audioOutput, 1);
AudioControlSGTL5000 audioShield;
// GUItool: end automatically generated code

inline float noteToFreq(uint8_t note) {
  return powf(2, (float)(note - 69) / 12.0f) * 440.0f;
}

void setup() {
  // Begin MIDI channel
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Audio Shield setup
  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.3);

  // Begin serial communication
  Serial.begin(57600);
  Serial.println("MIDI Input Test");
}

void loop() {
  int velocity, channel;
  uint8_t note;

  if (MIDI.read()) {
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          AudioNoInterrupts();
          waveform1.amplitude(0.5);
          waveform1.frequency(noteToFreq(note));
          AudioInterrupts();
          Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity + ", freq=" + noteToFreq(note));
        } else {
          Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
          waveform1.amplitude(0);
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity + ", freq=" + noteToFreq(note));
        break;
    }
  }

  AudioNoInterrupts();
  waveform1.begin(WAVEFORM_SINE);
  AudioInterrupts();
}