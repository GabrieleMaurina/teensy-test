#include <Audio.h>

AudioControlSGTL5000 audio;

AudioInputI2S mic;
AudioOutputI2S hP;

AudioConnection con(mic, hP);

void setup()
{
	AudioMemory(100);
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.volume(0.8);
	audio.micGain(40);
}


void loop()
{
	
}

