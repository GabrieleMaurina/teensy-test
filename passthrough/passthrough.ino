#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioControlSGTL5000_Extended audio;
AudioInputI2S_F32 mic;
AudioOutputI2S_F32 hP;
AudioConnection_F32 patchCord20(mic, hP);

void setup()
{
  AudioMemory(100);
  AudioMemory_F32(100);

  audio.enable();
  audio.inputSelect(AUDIO_INPUT_MIC);
  audio.volume(0.8);
  audio.lineInLevel(10, 10);
  audio.adcHighPassFilterDisable();
  audio.micBiasEnable(3.0);
}

void loop()
{
  
}
