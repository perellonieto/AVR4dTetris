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

#ifndef _DISPLAYSHIELD4D_H_
	#define _DISPLAYSHIELD4D_H_

	#include <inttypes.h>

	#define OLED_BAUDRATE			9600
	#define OLED_RESETPIN			7  
	#define OLED_INITDELAYMS		1000

	#define	OLED_DETECT_BAUDRATE		0x55
	#define OLED_GETDEVICEINFO		0x56
	#define OLED_STRINGTERMINATOR		0x00

	#define	OLED_CLEAR			0x45

	#define OLED_SOLID			0
	#define OLED_WIREFRAME			1

	#define	OLED_COMMAND_CONTROL		0x59
	#define	OLED_COMMAND_DISPLAY_OFF	0x00
	#define	OLED_COMMAND_DISPLAY_ON		0x01
	#define	OLED_COMMAND_SHUTDOWN		0x00
	#define	OLED_COMMAND_POWEROFF		0x01
	#define OLED_COMMAND_SLEEP		0x5A
	#define OLED_COMMAND_STOP_SD		0x80
	#define OLED_COMMAND_WAKEONKOYSTICK	0x02
	#define OLED_COMMAND_WAKEONSERIAL	0x01
	#define OLED_SCREENCOPY			0x63

	#define OLED_ACK			0x06
	#define OLED_NAK 			0x15

	// Graphics
	#define	OLED_PUTPIXEL			0x50
	#define	OLED_READPIXEL			0x52
	#define	OLED_LINE			0x4C
	#define	OLED_SETBACKGROUND		0x42
	#define	OLED_SETPENSIZE			0x70
	#define	OLED_RECTANGLE			0x72
	#define	OLED_CIRCLE			0x43
	#define	OLED_TRIANGLE			0x47

	// Text
	#define	OLED_SETFONT			0x46
		#define	OLED_FONT5x7		0x00
		#define	OLED_FONT8x8		0x01
		#define	OLED_FONT8x12		0x02

	#define	OLED_SETFONTMODE		0x4F
		#define	OLED_FONT_TRANSPARENT	0x00
		#define	OLED_FONT_OPAQUE	0x01

	#define OLED_STRING_BLOCK		0x53
	#define OLED_TEXT_BUTTON		0x62
		#define OLED_TEXT_BUTTON_DOWN	0x00
		#define OLED_TEXT_BUTTON_UP	0x01
	#define OLED_ASCII_CHAR			0x74

	// SD Card functions
	#define OLED_SD_COMMAND			0x40
		#define OLED_INIT_SD		0x69
		#define OLED_SD_SET_PTR		0x41

	// Class definition
	class DisplayShield4d 
	{
		public:
			DisplayShield4d();

			uint8_t Init();
			uint8_t Reset();
			uint8_t Clear();
			uint8_t GetReply();			
			/*
			char *GetDeviceType();
			uint8_t GetDeviceWidth();
			uint8_t GetDeviceHeight();
			*/
			unsigned int RGB(uint8_t red, uint8_t green, uint8_t blue);
			uint8_t SetPenSize(char val);
			uint8_t SetBackground(unsigned int color);
			uint8_t SetContrast(char val);
			uint8_t SetState(char state);
			uint8_t Sleep(char wake_cond);

			// Utility
			uint8_t ScreenCopy(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height);

			// Graphics functions
			uint8_t putpixel(uint8_t x, uint8_t y, unsigned int color);
			uint16_t readpixel(uint8_t x, uint8_t y);
			uint8_t line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, unsigned int color);
			uint8_t rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color);
			uint8_t circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color);
			uint8_t triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color);

			// Text functions
			uint8_t setfont(uint8_t font_type);
			uint8_t setfontmode(uint8_t font_mode);
			uint8_t drawstringblock(uint8_t x, uint8_t y, uint8_t font, unsigned int color, uint8_t width, uint8_t height, char *text);
			uint8_t drawtextbutton(uint8_t state, uint8_t x, uint8_t y, unsigned int buttoncolor, uint8_t font, unsigned int stringColor, uint8_t width, uint8_t height, char *text);
			uint8_t drawasciichar(uint8_t strChar, uint8_t x, uint8_t y, unsigned int color, uint8_t width, uint8_t height);

			// SD Card function
			uint8_t init_sd();
			uint8_t set_address_pointer(long addr);


		private:
			//void GetDeviceInfo();

			uint8_t	device_type;
			uint8_t	device_hardware_rev;
			uint8_t	device_firmware_rev;
			uint8_t	device_width;
			uint8_t	device_height;

	};

#endif
