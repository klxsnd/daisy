// leonBlinkKnob / 20200628
// flashes the LED on the seed
// with a rate specified by a knob (ctrl 1)
//
#include <stdio.h>
#include <stdlib.h>
// #include <cstdio>
#include "daisy_patch.h"
#include "daisysp.h"


// use the daisy/sp namespaces to avoid typing daisy::
using namespace daisy;
using namespace daisysp;

// must declare an object
DaisyPatch patch;
Parameter ratectrl;

// declare some useful constants
//static const int BLINK_RATE_MIN = 2000; 	// slowest
static const size_t BLINK_RATE_MIN = 2000; 	// slowest
//static const int BLINK_RATE_MAX = 50; 		// fastest
static const size_t BLINK_RATE_MAX = 50; 		// fastest
static const size_t OLED_BUFFER_MAX_LEN = 32; 

// globals
//char[32] kRateMsg;

// knob value
//void GetBlinkRate (int rate)
//{
//	char[32] tmp;
//		if (rate < 1000)
//		{
//			kRateMsg = "slow";
//		}
//		else if (rate > 500)
//		{
//			kRateMsg = "fast";
//		}
//		else
//		{
//			kRateMsg = "med";
//		}
//	}
//}


int main (void)
{
	// var to hold LED state
	bool led_state; // naming convention - variables lc with u/s
	led_state = true; // give it an init value
	
	// init the daisy patch
	patch.Init(); // inits hardware: both seed and patch
	// init the rate control
	ratectrl.Init(patch.controls[patch.CTRL_1], BLINK_RATE_MIN, BLINK_RATE_MAX, Parameter::LINEAR);
	
	// start ADC. not sure what that is but it doesn't work w/o it.
	// update: this starts the analog to digital conversion.
	patch.StartAdc();
	//patch.UpdateAnalogControls();
	//patch.DebounceControls();
	// get the value of of ctrl 1 where it was left
	int blink_rate = patch.GetCtrlValue(patch.CTRL_1); //ratectrl.Process();

	// init the screen
	patch.display.Fill(true);
	char titlebuff[OLED_BUFFER_MAX_LEN];
	char msgbuff[OLED_BUFFER_MAX_LEN];
	// char tmp[sizeof(BLINK_RATE_MIN)*8+1];
	sprintf(titlebuff, "blinking LED");
	sprintf(msgbuff, "%d", blink_rate);
	// font choices are 6x8, 7x10, 11x18, 16x26
	patch.display.SetCursor(5, 5);
	patch.display.WriteString(titlebuff, Font_7x10, false);
	patch.display.SetCursor(5, (SSD1309_HEIGHT - Font_16x26.FontHeight) / 2);
	patch.display.WriteString(msgbuff, Font_16x26, false);
	patch.display.Update();
	

	// blink forever and tell us the value on the screen.
	// at a rate specified by the first knob.
	for (;;)
	{
		// set the LED
		patch.seed.SetLed(led_state);
		// set the display value
		blink_rate = ratectrl.Process();
		sprintf(msgbuff, "%d", blink_rate);
		patch.display.SetCursor(5, (SSD1309_HEIGHT - Font_16x26.FontHeight) / 2);
		patch.display.WriteString(msgbuff, Font_16x26, false);
		patch.display.Update();
		
		// toggle the state
		led_state = !led_state;
		
		// wait some number of ms
		dsy_system_delay(blink_rate);
		// clean up
		// tmp = clean up the char buffer, somehow...if necessary
		
	}
}

