// Knob Value LED / 20200628
// experiments in doing useful things with the LED
//
// #include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include "daisy_patch.h"
#include "daisysp.h"


// use the daisy/sp namespaces to avoid typing daisy::
using namespace daisy;
using namespace daisysp;

// must declare an object
DaisyPatch patch;		// this is the daisy patch object, contains a seed
Parameter ctrlA;		// this obj manages the value of the knobs and cv intput. this is a Daisy specific object, it really should be called DaisyParameter...

// declare some useful constants
static const size_t BLINK_RATE_MIN = 20; 	// slowest
static const size_t BLINK_RATE_MAX = 20000; 		// fastest
static const Parameter::Curve CTRL1_CURVE = Parameter::LOGARITHMIC; // Curve is an enum in hid_parameter.h
//static const Parameter::Curve CTRL1_CURVE = Parameter::LINEAR; // Curve is an enum in hid_parameter.h
static const size_t CTRL_VALUE_MULLIGAN = 1; // checks to see if the ctrl value is outside this range before updating the screen. prevents flicker.

static const size_t OLED_BUFFER_MAX_LEN = 32; 
static const size_t OLED_POLLING_TIME = 2; // in ms

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
	ctrlA.Init(patch.controls[patch.CTRL_1], BLINK_RATE_MIN, BLINK_RATE_MAX, CTRL1_CURVE);
	
	// start ADC. not sure what that is but it doesn't work w/o it.
	// update: this starts the analog to digital conversion.
	 patch.StartAdc();
	// not sure what these do and it works wo them
	//patch.UpdateAnalogControls();
	//patch.DebounceControls();
	// get the value of of ctrl 1 where it was left
	size_t blink_rate = patch.GetCtrlValue(patch.CTRL_1); //ctrlA.Process();
	size_t blink_rate_tmp = blink_rate; // for comparison to avoid calling .Process() too many times

	// init the screen
	patch.display.Fill(true);
	char titlebuff[OLED_BUFFER_MAX_LEN];
	char msgbuff[OLED_BUFFER_MAX_LEN];
	// char tmp[sizeof(BLINK_RATE_MIN)*8+1];
	sprintf(titlebuff, "knob values *");
	sprintf(msgbuff, "%6d", blink_rate);
	// font choices are 6x8, 7x10, 11x18, 16x26
	patch.display.SetCursor(5, 5);
	patch.display.WriteString(titlebuff, Font_7x10, false);
	patch.display.SetCursor(5, (SSD1309_HEIGHT - Font_16x26.FontHeight) / 2);
	patch.display.WriteString(msgbuff, Font_16x26, false);
	patch.display.Update();
	

	// poll for changed knob values at a given increment and update the LED
	// bonus: blink a "light" on the LED at a different rate than the display polling
	for (;;)
	{
		// set the LED on as an indicator of not being in boot mode
		patch.seed.SetLed(led_state); 
		// set the display value. check that its outside the mulligan range to prevent flicker
		blink_rate_tmp = ctrlA.Process();
		if ( -(blink_rate - blink_rate_tmp) > CTRL_VALUE_MULLIGAN)
		{
			blink_rate = blink_rate_tmp; // ctrlA.Process();
			// memset(msgbuff, 0, OLED_BUFFER_MAX_LEN); // reset msgbuff - not needed
			sprintf(msgbuff, "%6d", blink_rate);
			patch.display.SetCursor(5, (SSD1309_HEIGHT - Font_16x26.FontHeight) / 2);
			patch.display.WriteString(msgbuff, Font_16x26, false);
			patch.display.Update();
		}
		// this is the polling time for the system to ask the knobs where they are.
		dsy_system_delay(OLED_POLLING_TIME);
		
	}
}