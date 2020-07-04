 #include "daisy_patch.h"


// use the daisy namespace to avoid daisy::
using namespace daisy;

// must declare an object
DaisyPatch patch;

int main (void)
{
	// var to hold LED state
	bool led_state; // naming convention - variables lc with u/s
	led_state = true;
	
	// init the daisy seed
	// these are 2 lines to allow for easier reconfig (not sure how)
	patch.seed.Configure();
	patch.Init();
	
	// blink forever. that's all this does. 
	for (;;)
	{
		// set the LED
		patch.seed.SetLed(led_state);
		
		// toggle the state
		led_state = !led_state;
		
		// wait some number of ms
		int blink_rate = 500;
		dsy_system_delay(blink_rate);
	}
}

