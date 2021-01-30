// QyadFO
// Leon Rothenberg / 20200701
// 4 phasing LFOs based on DivKid's Ochd
//

// includes
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "daisy_patch.h"
#include "daisysp.h"
// any of my own includes, like my_interface.h


// namespaces
using namespace std;
using namespace daisy;
using namespace daisysp;

// globals (can also be in .h files)

// object definition
DaisyPatch hw;
Oscillator LFOS[4]; // = { lfo1, lfo2, lfo3, lfo4 };
Parameter freqctrl;
float factors[4] = { 1.0f, 1.5f, 2.2f, 3.5f }; // array of mult factors for each LFO
float lfo_freq, lfo_amp;

static void AudioCallback(float **in, float **out, size_t size)
{
//	float sigs[4];
	//float freq, amp; //, sig1, sig2;
	size_t wave;
	
	lfo_freq = freqctrl.Process(); // all 4 LFOs are linked to a base freq
	lfo_amp = 0.5; 
//	wave = LFOS[0].WAVE_SIN;  // this enum should work bc using daisysp namespace...
	wave = Oscillator::WAVE_SIN;  // this enum should work bc using daisysp namespace...
	
	for (size_t i = 0; i < size; i++) // why is the increment i+=2 sometimes and i++ sometimes? what are we stepping through?
	{
		// Set lfo params
		for (size_t j = 0; j < 4; j++)
		{
			LFOS[j].SetFreq(lfo_freq * factors[j]);
			LFOS[j].SetWaveform(wave);
			LFOS[j].SetAmp(lfo_amp);
//			sigs[j] = LFOS[j].Process();
//			out[j][i] = sigs[j];
			out[j][i] = LFOS[j].Process();
		}
	}
} // end audio callback

char* ftos ( float adc_read )
{
	// char str[32];
	char* str = 0;
	//float adc_read = 678.0123;

	char* tmpSign = ""; //(adc_read < 0) ? "-" : "";
	float tmpVal = (adc_read < 0) ? -adc_read : adc_read;

	int tmpInt1 = tmpVal;                  // Get the integer (678).
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).

	// Print as parts, note that you need 0-padding for fractional bit.
	sprintf (str, "%s%d.%04d\n", tmpSign, tmpInt1, tmpInt2);
	return str;
}

int main (void)
{
	float samplerate;
	bool led_state = true; // always turn the LED on to tell that its not in bootloader mode
	char msgbuff[32];

	// init the daisy patch
	hw.Init();
	samplerate = hw.AudioSampleRate();
	// init the 4 LFOs with the hw sample rate
	for (size_t a = 0; a < 4; a++) 
	{
		LFOS[a].Init(samplerate);
	}
	// init the freq control knob
	freqctrl.Init(hw.controls[hw.CTRL_1], 0.01f, 10.0f, Parameter::EXPONENTIAL); 
	hw.seed.SetLed(led_state); // set LED on so we know we got this far and its running
	
	// get to work!
	hw.StartAdc();
	hw.StartAudio(AudioCallback);
	hw.display.Fill(true);
 	//while(1); // GO TO 10 until told not to
	for (;;)
	{
		sprintf(msgbuff, "%s", ftos(lfo_freq));
		hw.display.SetCursor(5, (SSD1309_HEIGHT - Font_16x26.FontHeight) / 2);
		hw.display.WriteString(msgbuff, Font_16x26, false);
		hw.display.Update();
	// hw.DisplayControls();
	}
} // end main

