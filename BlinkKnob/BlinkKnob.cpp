// leonBlinkKnob / 20200628
// flashes the LED on the seed
// with a rate specified by a knob (ctrl 1)
//

#include "daisysp.h"
#include "daisy_patch.h"


// use the daisy/sp namespaces to avoid typing daisy::
using namespace daisy;
using namespace daisysp;

// must declare an object
DaisyPatch patch;
Parameter ratectrl;

int main (void)
{
	// var to hold LED state
	bool led_state; // naming convention - variables lc with u/s
	led_state = true; // give it an init value
	
	// init the daisy patch
	patch.Init(); // inits hardware: both seed and patch
	ratectrl.Init(patch.controls[patch.CTRL_1], 100, 1000, Parameter::LINEAR);
	
	// start adc. not sure what that is but it doesn't work wo it.
	patch.StartAdc();

	// blink forever. that's all this does.
	// at a rate specified by the first knob 
	for (;;)
	{
		// set the LED
		patch.seed.SetLed(led_state);
		
		// toggle the state
		led_state = !led_state;
		
		// wait some number of ms
		int blink_rate = ratectrl.Process();
		dsy_system_delay(blink_rate);
	}
}

