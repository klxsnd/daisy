#include <stdio.h>
#include <string>
#include "daisy_patch.h"

#define PF_DARK_MODE false
#define PF_LIGHT_MODE true

static const bool PF_DISP_MODE = PF_DARK_MODE;

// use the daisy namespace to avoid daisy::
using namespace daisy;
using namespace std;

// must declare an object
DaisyPatch patch;

const int NUM_LABELS = 4;
const int ROW_BUFFER = 2; // pixels btwn rows


string labels[NUM_LABELS] = 
{
	"frq",
	"phs",
	"wav",
	"amp",
};
Parameter values[NUM_LABELS]; // values fr ctrl knobs assoc w/ these labels


void UpdateLedTest(); // the whole 'above' main() thing. defined below
void UpdateInterface(); // the whole 'above' main() thing. defined below

int main (void)
{
//	float samplerate;
	patch.Init(); // init hardware: patch, seed
//	samplerate = patch.AudioSampleRate(); // not needed for the moment
	
	// init controls
	for (int v=0;v<NUM_LABELS;v++)
	{
		values[v].Init(patch.controls[v],1,10,Parameter::LINEAR);
	}
	
	patch.StartAdc();
	// audio callback would go here
	while(1) // always be updating the controls
	{
		patch.display.Fill(PF_DISP_MODE); // "dark" mode. does this have to be done everuptime the interface is updated? apparently not. 
		//UpdateLedTest();
		UpdateInterface();
	}
}

// // // support functions

void InterfaceWrite( int x_, int y_, char* v_, FontDef f_, OledDisplay d_)
{
	d_.SetCursor(x_,y_);
	d_.WriteString(v_, f_,!PF_DISP_MODE); // bool must be the opposite of MODE so as not to appear "hilited"
	//d_.Update();
}

void UpdateLedTest()
{
	int x, y; // coords for writing to the LED
	patch.display.Fill(PF_DISP_MODE); // "dark" mode
	
	string label_ = "label";
	char* cstr_ = &label_[0]; // I think this converts the string to a char* with the value of a pointer to the label string. I assume .writestring is looking for a char*, not a string
	x = y = 0;
	InterfaceWrite(x,y,cstr_, Font_6x8, patch.display);

}

void UpdateInterface()
{
	int row_ = 0; // y coord
	int cell_ = 0; // x coord
	char tmp_[SSD1309_WIDTH]; // a string buffer as long as LED is wide
	FontDef font_ = Font_6x8;

    patch.DebounceControls(); // a patch obj should really be passed in...
    patch.UpdateAnalogControls(); // but is that too much memory waste? how about a pointer to a patch obj?

	for (int i=0;i<NUM_LABELS;i++)
	{
		cell_ = i * (SSD1309_WIDTH / NUM_LABELS); // cell size
		
		// header
		InterfaceWrite(
			cell_, // "column"
			row_, // row on the screen
			&labels[i][0], // label to print
			font_, // the font to use
			patch.display // pass the display
		);
		sprintf(tmp_, "%d", i); //values[i].Process());
		InterfaceWrite(
			cell_, // "column"
			row_ + font_.FontHeight + ROW_BUFFER, // row on the screen
			tmp_, // value to print
			font_, // the font to use
			patch.display // pass the display
		);
		// footer
		InterfaceWrite(
			cell_, // "column"
			SSD1309_HEIGHT-font_.FontHeight, // row on the screen
			&labels[i][0], // label to print
			font_, // the font to use
			patch.display // pass the display
		);
		
	}
	patch.display.Update();
	
}