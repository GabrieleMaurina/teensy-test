#define SWITCH 29

#include <Audio.h>

AudioInputI2S mic;
AudioRecordQueue rec;
AudioConnection con(mic, 0, rec, 0);
AudioControlSGTL5000 audio;

void setup()
{
	Serial.begin(0);
	pinMode(SWITCH, INPUT);

	AudioMemory(100);
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.micGain(40);
	audio.volume(0.5);
}

void loop(){
	static bool record = false;
	bool shouldRecord = digitalRead(SWITCH) == HIGH;
	if(record && !shouldRecord){
		record = false;
		rec.end();
	}
	else if(!record && shouldRecord){
		record = true;
		rec.begin();
	}

	if(rec.available()){
		Serial.write((byte*)rec.readBuffer(), 256);
		rec.freeBuffer();
	}
}
