//ARM CFFT Q15

#define TIMES 50000
#include <arm_math.h>

const int s[] = {256, 1024};
arm_cfft_radix4_instance_q15 fft;
int16_t a[2048];

unsigned long t = 0;

void setup()
{
	Serial.begin(9600);
	randomSeed(73);
	for(int i = 0; i < 2048; i++)
	{
		a[i] = random(INT16_MIN, INT16_MAX);
	}
	while(!Serial);
}

void loop()
{
	Serial.println("fft+mag");
	for(int i = 0; i < 2; i++)
	{
		arm_cfft_radix4_init_q15(&fft, s[i], 0, 1);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_cfft_radix4_q15(&fft, a);
				arm_cmplx_mag_q15(a, a, s[i]);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
	Serial.println("fft");
	for(int i = 0; i < 2; i++)
	{
		arm_cfft_radix4_init_q15(&fft, s[i], 0, 1);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_cfft_radix4_q15(&fft, a);
				arm_cmplx_mag_q15(a, a, s[i]);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
	Serial.println("mag");
	for(int i = 0; i < 2; i++)
	{
		arm_cfft_radix4_init_q15(&fft, s[i], 0, 1);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_cfft_radix4_q15(&fft, a);
				arm_cmplx_mag_q15(a, a, s[i]/2);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
}





//ARM RFFT F32

/*#define TIMES 50000
#include <arm_math.h>

const float32_t MAX = 2147483647;
const int s[] = {256, 512, 1024, 2048, 4096};
arm_rfft_fast_instance_f32 fft;
float32_t in[4096];
float32_t out[4097];

unsigned long t = 0;

void setup()
{
	Serial.begin(9600);
	randomSeed(73);
	for(int i = 0; i < 4096; i++)
	{
		in[i] = random(MAX)/MAX;
	}
	while(!Serial);
}

void loop()
{
	Serial.println("fft+mag");
	for(int i = 0; i < 5; i++)
	{
		arm_rfft_fast_init_f32(&fft, s[i]);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_rfft_fast_f32(&fft, in, out, 0);
				arm_cmplx_mag_f32(out + 3, out, s[i] / 2);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
	
	Serial.println("fft");
	for(int i = 0; i < 5; i++)
	{
		arm_rfft_fast_init_f32(&fft, s[i]);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_rfft_fast_f32(&fft, in, out, 0);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
	Serial.println("mag");
	for(int i = 0; i < 5; i++)
	{
		arm_rfft_fast_init_f32(&fft, s[i]);
		for(int j = 0; j < 5; j++)
		{
			t = micros();
			for(int k = 0; k < TIMES; k++)
			{
				arm_cmplx_mag_f32(out + 3, out, s[i] / 2);
			}
			t = micros() - t;
			Serial.print(s[i]);
			Serial.print("\t");
			Serial.println(t);
		}
	}
}*/





//FFT OPEN

/*#define N_FFT 256

#include <Audio.h>
#include <OpenAudio_ArduinoLibrary.h>

AudioControlSGTL5000_Extended audio;
AudioInputI2S_F32 mic;
AudioAnalyzeFFT_F32 fft(N_FFT);

AudioConnection_F32 con(mic, fft);

unsigned long t = 0;
int c = 0;
int res[60];
int i = 0;

void setup()
{
	Serial.begin(9600);
	
	AudioMemory(100);
	AudioMemory_F32(100);
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.micGain(40);

	t = millis();
}


void loop()
{
	if(fft.available())
	{
		c++;
	}
	if(millis() - t >= 1000 * (i + 1))
	{
		if(i < 60)
		{
			res[i++] = c;
			c = 0;
			if(i > 59){
				for(int j = 0; j < 60; j++)
				{
					Serial.println(res[j]);
				}
			}
		}
	}
}*/





//FFT AUDIO

/*#include <Audio.h>


AudioControlSGTL5000 audio;
AudioInputI2S mic;
AudioAnalyzeFFT256 fft;
AudioConnection con(mic, fft);


unsigned long t = 0;
int c = 0;
int res[60];
int i = 0;

void setup()
{
	Serial.begin(9600);
	
	AudioMemory(100);
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.micGain(40);

	fft.averageTogether(1);

	t = millis();
}


void loop()
{
	if(fft.available())
	{
		c++;
	}
	if(millis() - t >= 1000 * (i + 1))
	{
		if(i < 60)
		{
			res[i++] = c;
			c = 0;
			if(i > 59){
				for(int j = 0; j < 60; j++)
				{
					Serial.println(res[j]);
				}
			}
		}
	}
}*/





//SAMPLE RATE

/*#include <Audio.h>


AudioControlSGTL5000 audio;
AudioInputI2S mic;
AudioRecordQueue rec;
AudioConnection con(mic, rec);


unsigned long t = 0;
int c = 0;
int res[60];
int i = 0;

void setup()
{
	Serial.begin(9600);
	
	AudioMemory(100);
	audio.enable();
	audio.inputSelect(AUDIO_INPUT_MIC);
	audio.micGain(40);

	rec.begin();
	t = millis();
}


void loop()
{
	if(rec.available())
	{
		rec.readBuffer();
		rec.freeBuffer();
		c++;
	}
	if(millis() - t >= 1000 * (i + 1))
	{
		if(i < 60)
		{
			res[i++] = c;
			c = 0;
			if(i > 59){
				for(int j = 0; j < 60; j++)
				{
					Serial.println(res[j] * 128);
				}
			}
		}
	}
}*/
