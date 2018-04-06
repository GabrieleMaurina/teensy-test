#define BAUD_RATE 115200

#define SAMPLE_RATE 8000
#define AUDIO_BLOCK_SAMPLES 128

#define RIGHT 0
#define LEFT 1

#define SWITCH 29



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioSettings_F32 audioSettings(SAMPLE_RATE, AUDIO_BLOCK_SAMPLES);

AudioControlSGTL5000_Extended audio;

AudioInputI2S_F32 mic(audioSettings);
AudioOutputI2S_F32 hP(audioSettings); 
AudioRecordQueue_F32 rec(audioSettings);

AudioConnection_F32 mic2hPR(mic, RIGHT, hP, RIGHT);
AudioConnection_F32 mic2hPL(mic, LEFT, hP, LEFT);
AudioConnection_F32 mic2recR(mic, RIGHT, rec, RIGHT);
AudioConnection_F32 mic2recL(mic, LEFT, rec, LEFT);

void setup()
{
  Serial.begin(BAUD_RATE);

  pinMode(SWITCH, INPUT);

  AudioMemory(100);
  AudioMemory_F32(100, audioSettings);

  audio.enable();
  audio.inputSelect(AUDIO_INPUT_MIC);
  audio.volume(0.8);
  audio.lineInLevel(10, 10);
  audio.adcHighPassFilterDisable();
  audio.micBiasEnable(3.0);

  rec.begin();
}

void loop()
{
  if(Serial && digitalRead(SWITCH) && rec.available())
  {
    Serial.write((byte*)rec.readBuffer(), AUDIO_BLOCK_SAMPLES * 4);
    rec.freeBuffer();
  }
}
