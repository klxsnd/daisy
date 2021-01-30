#pragam once
#ifndef PATCHFACE_H
#define PATCHFACE_H /**< Macro */

#ifndef PF_LED_HEIGHT
#define PF_LED_HEIGHT SSD1309_HEIGHT // 64
#endif

#ifndef PF_LED_WIDTH
#define PF_LED_WIDTH SSD1309_WIDTH // 128
#endif

const int PF_BUFFER_MAX_LEN PF_LED_WIDTH;

namespace daisy
{
/** 
	classes for setting up some easy interface elemnents
	on the LED screen of the Daisy Patch.
*/

class IFLabel // static label value
{
	public:
		void Init();
}; // end class IFLabel

class IFValue // dynamic value, associated with a label
{
	public:
		void Init();
}; // end class IFValue

class IFRow // a set of labels or values distributed horizontally
{
	public:
		void Init();
}; // end class IFRow

class LEDInterface // class representing the whole screen and its capabilities
{
	public:
		void Init();
}; // end class IFRow


} // namesapce daisy

#endif