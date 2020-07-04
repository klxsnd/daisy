// QyadFO
// Leon Rothenberg / 20200701
// 4 phasing LFOs based on DivKid's Ochd
//

// includes
#include <cmath>
#include "daisy_patch.h"
#include "daisysp.h"
// any of my own includes, like my_interface.h


// namespaces
using namespace std;
using namespace daisy;
using namespace daisysp;

// globals (can also be in .h files)

// needed bc the knobs are not so accurate. aknob range from 0...10 might not actualy hit the bounds
// so this sets a limit below which everything is truncated to zero.
static const float ZERO_FUDGE_FACTOR = 0.003;

// object definition
DaisyPatch hw;
Oscillator LFOS[4]; // = { lfo1, lfo2, lfo3, lfo4 };
Parameter freqctrl, wavectrl, phasectrl;
//float factors[4] = { 1.0, 1.5, 2.2, 3.5 }; // array of mult factors for each LFO
//float factors[4] = { 1.0, 1.1, 1.3, 1.7 }; // array of mult factors for each LFO
//float factors[4] = { 0.0, 0.1, 0.3, 0.7 }; // array of mult factors for each LFO (1 added later) : subtle version
float factors[4] = { 0.0, 0.5, 1.2, 2.3 }; // array of mult factors for each LFO (1 added later)
float lfo_freq, lfo_amp, lfo_kr_amp;

// a better way to define available waves
// create and use my own array with the standard definitions.
uint8_t lfo_waveforms[] = {
	Oscillator::WAVE_SIN,
	Oscillator::WAVE_TRI,
	Oscillator::WAVE_SAW,
//	Oscillator::WAVE_RAMP,
	Oscillator::WAVE_SQUARE,
//	Oscillator::WAVE_POLYBLEP_TRI,
//	Oscillator::WAVE_POLYBLEP_SAW,
//	Oscillator::WAVE_POLYBLEP_SQUARE,
};
static const size_t NUM_WAVEFORMS = 4;


static void AudioCallback(float **in, float **out, size_t size)
{
//	float sigs[4];
	//float freq, amp; //, sig1, sig2;
	size_t wave;
	float phase;
	
	hw.UpdateAnalogControls();
	hw.DebounceControls();

	lfo_freq = freqctrl.Process(); // all 4 LFOs are linked to a base freq
	wave = wavectrl.Process(); // lfo waveform
	phase = phasectrl.Process(); //floor(phasectrl.Process())/10.0; // lfo "phase", or freq offset factor. trying to get to a reliable zero
	lfo_amp = 0.5f;
	lfo_kr_amp = 1.0; // if lfo_freq < 1, amp must be 1 instead of .5? why??!?
//	wave = LFOS[0].WAVE_SIN;  // this enum should work bc using daisysp namespace...
	//wave = Oscillator::WAVE_SIN;  // this enum should work bc using daisysp namespace...
	
	for (size_t i = 0; i < size; i++) // why is the increment i+=2 sometimes and i++ sometimes? what are we stepping through?
	{
		// Set lfo params
		for (size_t j = 0; j < 4; j++)
		{
			LFOS[j].SetFreq(lfo_freq * (1.0+(factors[j]* (phase<ZERO_FUDGE_FACTOR?0:phase) ))); // * freq by phase factor and amount from phase knob 
			LFOS[j].SetWaveform(lfo_waveforms[wave]);
			LFOS[j].SetAmp(lfo_freq > 1 ? lfo_amp : lfo_kr_amp); // the audio outputs are AC Coupled, so there are issues at low frequency
//			LFOS[j].SetAmp(lfo_freq>1.0 ? lfo_amp : lfo_kr_amp); // why doesn't this work? bc audio outputs are AC coupled
			out[j][i] = LFOS[j].Process();
		} 
	}
} // end audio callback

int main (void)
{
	float samplerate;
	bool led_state = true; // turn the LED on to tell that its not in bootloader mode

	// init the daisy patch
	hw.Init();
	samplerate = hw.AudioSampleRate();
	hw.seed.SetLed(led_state); // set LED on so we know we got this far and its running

	// init the 4 LFOs with the hw sample rate
	for (size_t a = 0; a < 4; a++) 
	{
		LFOS[a].Init(samplerate);
	}
	
	// init the control knobs
	freqctrl.Init(hw.controls[hw.CTRL_1], 0.1, 200.0f, Parameter::LINEAR); // LFO base freq
	wavectrl.Init(hw.controls[hw.CTRL_2], 0, NUM_WAVEFORMS, Parameter::LINEAR); // lfo wave shape
	phasectrl.Init(hw.controls[hw.CTRL_3], 0.0, 1.0, Parameter::LINEAR); // lfo "phase" amount (diff in freq)
	
	// get to work!
	hw.StartAdc();
	hw.StartAudio(AudioCallback);
 	while(1); // GO TO 10 until told not to
} // end main

