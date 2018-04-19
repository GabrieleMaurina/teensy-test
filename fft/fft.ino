#define N_FFT 4096

#define RIGHT 0
#define LEFT 1

#define SAMPLE_RATE 8000
#define AUDIO_BLOCK_SAMPLES 128



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioControlSGTL5000_Extended audio;
AudioSettings_F32 audioSettings(SAMPLE_RATE, AUDIO_BLOCK_SAMPLES);

AudioInputI2S_F32 mic;
AudioOutputI2S_F32 hP(audioSettings);
AudioAnalyzeFFT_F32 fft(N_FFT);

AudioConnection_F32 mic2hPR(mic, RIGHT, hP, RIGHT);
AudioConnection_F32 mic2hPL(mic, LEFT, hP, LEFT);
AudioConnection_F32 mic2fft(mic, fft);



void setup()
{
	Serial.begin(9600);
	while (!Serial);
	Serial.println("SETUP");
	initAudio();
	fft.setAvg(10);
	Serial.println("LOOP");
}


float32_t buf[N_FFT / 2];
void loop()
{
	if(fft.available()){
		memcpy(buf, fft.output, N_FFT * 2);
		for(int i = 0; i < 20; i++){
			Serial.print(buf[i]);
			Serial.print("\t");
		}
		Serial.println();
	}
}

void initAudio()
{
	AudioMemory(100);
	AudioMemory_F32(100);
	
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.volume(0.2);
	/*audio.lineInLevel(10, 10);
	audio.adcHighPassFilterDisable();
	audio.micBiasEnable(3.0);*/
}

