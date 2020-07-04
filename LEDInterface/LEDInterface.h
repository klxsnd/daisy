#pragam once
#ifndef KLX_LED_INTERFACE_H
#define KLX_LED_INTERFACE_H /**< Macro */

#ifndef SSD1309_HEIGHT
#define SSD1309_HEIGHT 64 /**< SSD1309 height in pixels */
#endif

#ifndef SSD1309_WIDTH
#define SSD1309_WIDTH 128 /**< SSD1309 width in pixels */
#endif

# define KLX_FIELD_BUFFER_MAX 32

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