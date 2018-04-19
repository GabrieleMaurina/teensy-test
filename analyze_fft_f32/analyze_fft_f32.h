#ifndef _analyze_fft_f32_h_
#define _analyze_fft_f32_h_

#include "Arduino.h"
#include "AudioStream_F32.h"
#include "arm_math.h"

#define AUDIO_BLOCK_SIZE 128

class AudioAnalyzeFFT_F32 : public AudioStream_F32
{
public:
	AudioAnalyzeFFT_F32(int n_fft) : AudioStream_F32(1, inputQueueArray), outputflag(false), blocks(n_fft / AUDIO_BLOCK_SIZE), n_fft(n_fft), n_avg(1), avg(0) {
		arm_rfft_fast_init_f32(&fft_inst, n_fft);
		prevblocks = new audio_block_f32_t*[blocks];
		output = new float32_t[n_fft / 2];
		res = new float32_t[n_fft / 2];
		in_buf = new float32_t[n_fft];
		out_buf = new float32_t[n_fft + 1];

		for(int i = 0; i < n_fft / 2; i++){
			res[i] = 0.0f;
		}
	}
	bool available() {
		if (outputflag == true) {
			outputflag = false;
			return true;
		}
		return false;
	}
	void setAvg(int n){
		n_avg = n;
	}
	int getAvg(){
		return n_avg;
	}
	int getNFFT(){
		return n_fft;
	}
	virtual void update(void);
	float32_t* output;
private:
	const int blocks;
	const int n_fft;
	int n_avg;
	int avg;
	volatile bool outputflag;
	audio_block_f32_t *inputQueueArray[1];
	audio_block_f32_t** prevblocks;
	float32_t* in_buf;
	float32_t* out_buf;
	float32_t* res;
	arm_rfft_fast_instance_f32 fft_inst;
};

#endif
