#define AUDIO_BLOCK_SIZE 128



#define RIGHT 0
#define LEFT 1



#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>
#include <arm_math.h>
//#include <variable.h>

AudioControlSGTL5000_Extended audio;

AudioInputI2S_F32 mic;
AudioOutputI2S_F32 hP;
AudioRecordQueue_F32 rec;

AudioConnection_F32 mic2hPR(mic, RIGHT, hP, RIGHT);
AudioConnection_F32 mic2hPL(mic, LEFT, hP, LEFT);
AudioConnection_F32 mic2rec(mic, rec);

arm_cfft_radix4_instance_f32 fft_inst;
float32_t complex_buffer[AUDIO_BLOCK_SIZE * 2];

//extern int gabriele;



void setup()
{
	Serial.begin(9600);
	while (!Serial);
	Serial.println("SETUP");
	initAudio();
	arm_cfft_radix4_init_f32(&fft_inst, AUDIO_BLOCK_SIZE, 0, 1);
	Serial.println("LOOP");
	Serial.println(gabriele());
}

void loop()
{
	if(rec.available()){
		float32_t* audioBlock = rec.readBuffer();
		for(int i = 0; i < AUDIO_BLOCK_SIZE; i++){
			complex_buffer[i * 2] = audioBlock[i];
			complex_buffer[i * 2 + 1] = 0;
		}
		rec.freeBuffer();
		arm_cfft_radix4_f32(&fft_inst, complex_buffer);
		
		for(int i = 0; i < 5; i++){
			Serial.print(i);
			Serial.print("{r:");
			Serial.print(complex_buffer[i * 2]);
			Serial.print(",i:");
			Serial.print(complex_buffer[i * 2 + 1]);
			Serial.print("} ");
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

	rec.begin();
}

