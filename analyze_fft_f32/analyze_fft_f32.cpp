#include <Arduino.h>
#include "analyze_fft_f32.h"



void AudioAnalyzeFFT_F32::update(void)
{
	static int counter = 0;
	audio_block_f32_t *block;

	block = receiveReadOnly_f32();
	if (!block) return;
	
	prevblocks[counter++] = block;
	if(counter < blocks) return;

	for(int i = 0; i < blocks; i++){
		memcpy(in_buf + i * AUDIO_BLOCK_SIZE, prevblocks[i] -> data, AUDIO_BLOCK_SIZE * 4);
	}

	arm_rfft_fast_f32(&fft_inst, in_buf, out_buf, 0);
	
	arm_cmplx_mag_f32(out_buf + 3, out_buf, n_fft / 2);

	for(int i = 0; i < n_fft / 2; i++){
		res[i] += out_buf[i] / n_avg;
	}
	
	if(++avg >= n_avg){
		avg = 0;
		outputflag = true;
		for(int i = 0; i < n_fft / 2; i++){
			output[i] = res[i];
			res[i] = 0.0f;
		}
	}

	counter = blocks / 2;

	for(int i = 0; i < counter; i++){
		release(prevblocks[i]);
		prevblocks[i] = prevblocks[i + counter];
	}
}
