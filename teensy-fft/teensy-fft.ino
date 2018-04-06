#include <Audio.h>

const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S            audioInput;
AudioAnalyzeFFT1024      myFFT;

AudioConnection          patchCord1(audioInput, 0, myFFT, 0);

AudioControlSGTL5000     audioShield;

void setup() {
  Serial.begin(115200);
  AudioMemory(12);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.micGain(40);  //0-63
  audioShield.volume(0.5);  //0-1

  myFFT.windowFunction(AudioWindowHanning1024);
}

byte buffer[1024];

void loop(){
  if (myFFT.available()){
    memcpy(buffer, myFFT.output, 1024);
    Serial.write(buffer, 1024);
  }
}
