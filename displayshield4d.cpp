/*
	displayshield4d.h - Arduino Library for 4Display-Shield by 4d Systems
	Copyright(c) December 2010 Oscar Gonzalez - www.BricoGeek.com

	http://code.google.com/p/displayshield4d/

	Licensed under GNU General Public License v3 
	http://creativecommons.org/licenses/by/3.0/
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

*/

#include "displayshield4d.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*******************************************************
	Constructor
 ********************************************************/
DisplayShield4d::DisplayShield4d() {
	// Constructor
	pinMode(OLED_RESETPIN, OUTPUT);
}

/*******************************************************
	Function: 	
		Init
	Description:	
		Initialise display. Prior to this function
		you must start serial by Serial.begin()
		For example: Serial.begin(57600);
	Params:	None	
	Return:	0
 ********************************************************/
uint8_t DisplayShield4d::Init()
{
	pinMode(OLED_RESETPIN, OUTPUT);
	Reset();  
	delay(OLED_INITDELAYMS);
	Serial.write(OLED_DETECT_BAUDRATE); 
	delay(10);

	//GetDeviceInfo();

	return 0;
}

/*
void DisplayShield4d::GetDeviceInfo()
{
	Serial.write(OLED_GETDEVICEINFO);
	Serial.write(OLED_GETDEVICEINFO_SERIAL);

	byte incomingByte;
	char counter=0;
	while (!Serial.available()) { delayMicroseconds(150); }
	
	do 
	{
		incomingByte = Serial.read();

		switch (counter)
		{
			case 0: { device_type 		= incomingByte; }; break;
			case 1: { device_hardware_rev 	= incomingByte; }; break;
			case 2: { device_firmware_rev 	= incomingByte; }; break;
			case 3: { device_width 		= incomingByte; }; break;
			case 4: { device_height 	= incomingByte; }; break;
		}

		counter++;
		

	} while (Serial.available());		
	
}

char *DisplayShield4d::GetDeviceType()
{
	switch (device_type)
	{
		case 0x00: { return "micro-OLED"; }; break;
		case 0x01: { return "micro-LCD"; }; break;
		case 0x02: { return "micro-VGA"; }; break;
	}
}

uint8_t DisplayShield4d::GetDeviceWidth()
{
	return device_width;
}

uint8_t DisplayShield4d::GetDeviceHeight()
{
	return device_height;
}
*/
/*******************************************************
	Function:	
		Reset 
	Description:
		Phisicaly reset the display. D7 jumper must be conected
		Also is a good thing to have pinMode(7, OUTPUT) set in Setup function
	Params:	None
	Return: No return
 ********************************************************/
uint8_t DisplayShield4d::Reset() {

	digitalWrite(OLED_RESETPIN, LOW);
	delay(10);                  
	digitalWrite(OLED_RESETPIN, HIGH);
	delay(10); 
}

/*******************************************************
	Function: 
		Clear
	Description:
		Clear entire display
	Params:	None
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::Clear() {
	Serial.write(OLED_CLEAR);
	return GetReply();
}

/*******************************************************
	Function: 
		GetReply
	Description:
		Used to get command response
	Params:	None
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::GetReply() {

	byte incomingByte = OLED_ACK;
	while (!Serial.available()) { delayMicroseconds(150); }
	incomingByte = Serial.read();
	return incomingByte;
}

/*******************************************************
	Function: 
		RGB
	Description:
		Returns correct int format for color parm
	Params:	red, green, blue - From 0 to 254
	Return:	None
 ********************************************************/
unsigned int DisplayShield4d::RGB(uint8_t red, uint8_t green, uint8_t blue) 
{

	char text[255];

	int outR = ((red * 31) / 255);
	int outG = ((green * 63) / 255);
	int outB = ((blue * 31) / 255);

	return (outR << 11) | (outG << 5) | outB;

}

/*******************************************************
	Function: 
		SetPenSize
	Description:
		This  command  determines  if  certain graphics  objects  are drawn in solid or  wire frame fashion.
	Params:	0 for solid or 1 for wireframe
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::SetPenSize(char val)
{
	Serial.write(OLED_SETPENSIZE);
	Serial.write(val);

	return GetReply();
}

/*******************************************************
	Function: 
		SetBackground
	Description:
		changes the current background colour. Once this command is sent, only the background colour will change.

	Params:	color
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::SetBackground(unsigned int color)
{
	Serial.write(OLED_SETBACKGROUND);

	// Color
	Serial.write(color >> 8);				// MSB
	Serial.write(color & 0xFF);				// LSB

	return GetReply();
}

/*******************************************************
	Function: 
		SetContrast
	Description:
		Set current contrast

	Params:	cval - 0 to 15
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::SetContrast(char val)
{
	Serial.write(OLED_COMMAND_CONTROL);

	// Set contrast
	Serial.write(val); // 0-15

	return GetReply();
}

/*******************************************************
	Function: 
		SetState
	Description:
		This command changes some of the display settings

	Params:	state - Can be COMMAND_DISPLAY_ON, COMMAND_DISPLAY_OFF, OLED_COMMAND_SHUTDOWN, OLED_COMMAND_POWEROFF (for low power save state)
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::SetState(char state)
{
	Serial.write(OLED_COMMAND_CONTROL);

	// Set contrast
	Serial.write(state);

	return GetReply();
}

/*******************************************************
	Function: 
		SetState
	Description:
		Puts GOLDELOX-SGC chip in to low power mode and optionally waits for certain conditions to wake it up

	Params:	wake_cond - Can be OLED_COMMAND_STOP_SD, OLED_COMMAND_WAKEONKOYSTICK or OLED_COMMAND_WAKEONSERIAL
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::Sleep(char wake_cond)
{
	Serial.write(OLED_COMMAND_SLEEP);

	// Set wakeup condition
	Serial.write(wake_cond);

	return GetReply();
}

/*******************************************************
	Function: 
		ScreenCopy
	Description:
		This command copies a specified area of the screen as a bitmap block to another screen position

	Params:	source_x, source_y, dest_x, dest_y, width and height
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::ScreenCopy(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height)
{

	Serial.write(OLED_SCREENCOPY);

	Serial.write(source_x);
	Serial.write(source_y);
	Serial.write(dest_x);
	Serial.write(dest_y);
	Serial.write(width);
	Serial.write(height);

	return GetReply();

}

/*******************************************************
	Function: 
		putpixel
	Description:
		Basic function to draw one pixel
	Params:
		x, y and color. Please check your screen size in order to use this function!
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::putpixel(uint8_t x, uint8_t y, unsigned int color)
{
	Serial.write(OLED_PUTPIXEL);
	Serial.write(x);
	Serial.write(y);

	// Color
	Serial.write(color >> 8);				// MSB			
	Serial.write(color & 0xFF);				// LSB

    	return GetReply();
}

/*******************************************************
	Function: 
		readpixel
	Description:
		Basic function to get the color of one pixel
	Params:
	Return: Returns 16bits color info (MSB/LSB)
 ********************************************************/
uint16_t DisplayShield4d::readpixel(uint8_t x, uint8_t y)
{
	Serial.write(OLED_READPIXEL);
	Serial.write(x);
	Serial.write(y);

	while (!Serial.available()) { delayMicroseconds(150); } // Let's display take the time...

    	return Serial.read() & Serial.read();
}

/*******************************************************
	Function: 
		line
	Description:
	Params:
		x1, y1, x2, y2 and color
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, unsigned int color)
{

	Serial.write(OLED_LINE);

	Serial.write(x1);						// X1
	Serial.write(y1);						// Y1
	Serial.write(x2);						// X2
	Serial.write(y2);						// Y2

	// Color
	Serial.write(color >> 8);					// MSB			
	Serial.write(color & 0xFF);					// LSB

	return GetReply();

}

/*******************************************************
	Function: 
		rectangle
	Description:
	Params:
		x, y, width, height, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color)
{

	SetPenSize(filled);

	Serial.write(OLED_RECTANGLE);

	Serial.write(x);						// X1
	Serial.write(y);						// Y1
	Serial.write(x+width);						// X2
	Serial.write(y+height);						// Y2

	// Color
	Serial.write(color >> 8);					// MSB			
	Serial.write(color & 0xFF);					// LSB

	return GetReply();

}

/*******************************************************
	Function: 
		circle
	Description:
	Params:
		x, y, radius, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color)
{
	SetPenSize(filled);

        Serial.write(OLED_CIRCLE); 

	Serial.write(x);	// x
	Serial.write(y);	// y
	Serial.write(radius);	// radius

	// Color
	Serial.write(color >> 8);		// MSB			
	Serial.write(color & 0xFF);		// LSB

        return GetReply();

}

/*******************************************************
	Function: 
		triangle
	Description:
	Params:
		x and y of each vertex, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		The vertices must be specified in an anticlock wise
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color)
{

	SetPenSize(filled);

        Serial.write(OLED_TRIANGLE); 

	Serial.write(x1);	// x
	Serial.write(y1);	// y
	Serial.write(x2);	// x
	Serial.write(y2);	// y
	Serial.write(x3);	// x
	Serial.write(y3);	// y

	// Color
	Serial.write(color >> 8);		// MSB			
	Serial.write(color & 0xFF);		// LSB

        return GetReply();

}

/*******************************************************
	Function: 
		setfont
	Description:
		This  command selects  one of  the available internal  fonts.
	Params:
		font_type: OLED_FONT5x7, OLED_FONT8x8 or OLED_FONT8x12
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::setfont(uint8_t font_type)
{
        Serial.write(OLED_SETFONT); 
	Serial.write(font_type);

        return GetReply();
}

/*******************************************************
	Function: 
		setfontmode
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		font_mode: OLED_FONT_TRANSPARENT or OLED_FONT_OPAQUE
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::setfontmode(uint8_t font_mode)
{
        Serial.write(OLED_SETFONTMODE); 
	Serial.write(font_mode);

        return GetReply();
}

/*******************************************************
	Function: 
		drawstringblock
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		font_mode: OLED_FONT_TRANSPARENT or OLED_FONT_OPAQUE
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::drawstringblock(uint8_t x, uint8_t y, uint8_t font, unsigned int color, uint8_t width, uint8_t height, char *text)
{

	Serial.write(OLED_STRING_BLOCK);

	Serial.write(x);
	Serial.write(y);
	Serial.write(font);

	// Color
	Serial.write(color >> 8);		// MSB			
	Serial.write(color & 0xFF);		// LSB

	Serial.write(width);
	Serial.write(height);

	for (int i=0 ; i<strlen(text) ; i++)
	{
		Serial.write(text[i]);
	}

	Serial.write(OLED_STRINGTERMINATOR, 1); // String terminator

	return GetReply();
}

/*******************************************************
	Function: 
		drawtextbutton
	Description:
		This command will draw/display a string of ASCII text anywhere on the screen in character unit coordinates.

	Params:
		Self explanatory ;)
		Use OLED_TEXT_BUTTON_DOWN or OLED_TEXT_BUTTON_UP for button state
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::drawtextbutton(uint8_t state, uint8_t x, uint8_t y, unsigned int buttoncolor, uint8_t font, unsigned int stringColor, uint8_t width, uint8_t height, char *text)
{	
	Serial.write(OLED_TEXT_BUTTON);

	Serial.write(state);

	Serial.write(x);
	Serial.write(y);

	// Button color
	Serial.write(buttoncolor >> 8);		// MSB			
	Serial.write(buttoncolor & 0xFF);		// LSB

	Serial.write(font);

	// String color
	Serial.write(stringColor >> 8);		// MSB			
	Serial.write(stringColor & 0xFF);		// LSB

	Serial.write(width);
	Serial.write(height);

	for (int i=0 ; i<strlen(text) ; i++)
	{
		Serial.write(text[i]);
	}

	Serial.write(OLED_STRINGTERMINATOR, 1); // String terminator

	return GetReply();
}

/*******************************************************
	Function: 
		drawasciichar
	Description:
		This command will draw/display an ASCII character anywhere on the screen in pixel coordinates specified by x and y parameters

	Params:
		Self explanatory ;)
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::drawasciichar(uint8_t strChar, uint8_t x, uint8_t y, unsigned int color, uint8_t width, uint8_t height)
{
	Serial.write(OLED_ASCII_CHAR);

	Serial.write(strChar);

	Serial.write(x);
	Serial.write(y);

	// Color
	Serial.write(color >> 8);		// MSB			
	Serial.write(color & 0xFF);		// LSB

	Serial.write(width);
	Serial.write(height);

	return GetReply();
}

/*******************************************************
	Function: 
		drawasciichar
	Description:
		Initialise Memory Card
	Params:
		
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::init_sd()
{
	Serial.write(OLED_SD_COMMAND);
	Serial.write(OLED_INIT_SD);

	return GetReply();

}

/*******************************************************
	Function: 
		set_address_pointer
	Description:
		sets the internal memory address pointer for byte wise reads and writes.
		After a byte read or write, the memory Address pointer is automatically incremented
		internally to the next byte address location.

	Params:
		addr: 32bits address (long)
		
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t DisplayShield4d::set_address_pointer(long addr)
{
	Serial.write(OLED_SD_COMMAND);
	Serial.write(OLED_SD_SET_PTR);

	// NOTE: Not sure if that works... (not tested yet)
	Serial.write(addr >> 24);
	Serial.write(addr >> 16);
	Serial.write(addr >> 8);
	Serial.write(addr & 0xFFFF);

	return GetReply();

}

