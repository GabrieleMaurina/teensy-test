To set up environment:

1) Install arduino
https://www.arduino.cc/en/Main/Software

2) Install teensyduino
https://www.pjrc.com/teensy/td_download.html

3) Update CMSIS
https://forum.pjrc.com/threads/40590-Teensy-Convolution-SDR-(Software-Defined-Radio)?p=129081&viewfull=1#post129081

{
	Hi Frank - I finally succeeded in compiling the Teensy Convolution code! Your direction about the location of the .a files was key, as well as fixing some file version discrepancies. In the hope that it may help others, here is a full summary of steps (including those described in earlier posts by Duff and yourself, to capture the full picture in one post).

	Two important items to check for the Teensy Convolution project:
	1. use the correct Si5351 library for Arduino environment from Jason Milldrum here: https://github.com/etherkit/Si5351Arduino

	2. Follow these install guidelines for the required CMSIS version 4.5.0 library functions:
	A)download the CMSIS v4.5.0 library from https://github.com/ARM-software/CMSIS
	B) unzip and copy these three files from the unzipped sub directory /CMSIS/Include:
	arm_common_tables.h ; arm_const_structs.h ; arm_math.h
	C) save these files to /--/Arduino/hardware/teensy/avr/cores/teensy3 (/--/ being the root of your Arduino environment)
	D) Make these modification to the arm_math.h file
	insert:
	#include <stdint.h>
	#define __ASM __asm
	#define __INLINE inline
	#define __STATIC_INLINE static inline
	#define __CORTEX_M 4
	#define __FPU_USED 1
	#define ARM_MATH_CM4
	#include "core_cmInstr.h"
	#include "core_cm4_simd.h"

	comment out:
	#if defined(ARM_MATH_CM7)
	#include "core_cm7.h"
	#elif defined (ARM_MATH_CM4)
	#include "core_cm4.h"
	#elif defined (ARM_MATH_CM3)
	#include "core_cm3.h"
	#elif defined (ARM_MATH_CM0)
	#include "core_cm0.h"
	#define ARM_MATH_CM0_FAMILY
	#elif defined (ARM_MATH_CM0PLUS)
	#include "core_cm0plus.h"
	#define ARM_MATH_CM0_FAMILY
	#else
	#error "Define according the used Cortex core ARM_MATH_CM7, ARM_MATH_CM4, ARM_MATH_CM3, ARM_MATH_CM0PLUS or ARM_MATH_CM0"
	#endif
	#undef __CMSIS_GENERIC enable NVIC and Systick functions */
	E) Copy two more files from the unzipped CMSIS library, folder CMSIS/Lib/GCC
	libarm_cortexM4l_math.a ; libarm_cortexM4lf_math.a
	F) save these files to /--/Arduino/hardware/tools/arm/arm-none-eabi/lib/


	That's it! 
	Jan
}

4) Install AnalyzeFFT_F32
Copy files in OpenAudio Folder
Inlcude new files in OpenAudio.h
