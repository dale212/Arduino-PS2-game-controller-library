/*  Psx_aj Controller Decoder Library (Psx_aj.h)
 
 April 20, 2018
 Original code PsX.h was written by Kevin Ahrendt
 Modifications by Dale Heatherington
 
 Changed library name from PsX to Psx_aj.  The "aj" suffix means "Analog Joystick".
 This works with PS1 and PS2 compatible controllers.
 It's been tested on Arduino Uno and Adafruit feather M0 Arm boards.
 
 To make it work with the M0 Arm boards I had to add some small delays in
 the bit shifting code to make sure both rising and falling clock edges
 were within the valid command bit period.
 
 I modified code to read analog joystick values and changed _dataIn bit shifts
 to left-shift because analog joystick values were in reverse bit order.
 Side effect: Now the digital button bits are reversed from the original
 so I changed the button hex defines to match.
 Analog joystick values are returned in public variables after each read():
 right_x_joy, right_y_joy, left_x_joy, left_x_joy
 
 User must push "analog" button on controller to enable analog mode or enable
 it in software with setAnalogMode()  command.

 
 
 
	Originally written by: Kevin Ahrendt June 22nd, 2008
	
	Controller protocol implemented using Andrew J McCubbin's analysis.
	http://www.gamesx.com/controldata/psxcont/psxcont.htm
	
	Shift command is based on tutorial examples for ShiftIn and ShiftOut
	functions both written by Carlyn Maw and Tom Igoe
	http://www.arduino.cc/en/Tutorial/ShiftIn
	http://www.arduino.cc/en/Tutorial/ShiftOut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef Psx_aj_h
#define Psx_aj_h

#include "Arduino.h"

// Button Hex Representations:
#define psxLeft		0x0080
#define psxDown		0x0040
#define psxRight	0x0020
#define psxUp		0x0010
#define psxStrt		0x0008
#define psxSlct		0x0001

#define psxSqu		0x8000
#define psxX		0x4000
#define psxO		0x2000
#define psxTri		0x1000
#define psxR1		0x0800
#define psxL1		0x0400
#define psxR2		0x0200
#define psxL2		0x0100


class Psx_aj
{
	public:
		Psx_aj();
		void setupPins(byte , byte , byte , byte , byte );	// (Data Pin #, CMND Pin #, ATT Pin #, CLK Pin #, Delay)
															// Delay is how long the clock goes without changing state
															// in Microseconds. It can be lowered to increase response,
															// but if it is too low it may cause glitches and have some
															// keys spill over with false-positives. A regular PSX controller
															// works fine at 50 uSeconds.
															
		unsigned int read();								// Returns the status of the button presses in an unsignd int.
															// The value returned corresponds to each key as defined above.
    
        byte left_x_joy;                //Analog joystick values.  These are updated by read() function.
        byte left_y_joy;
        byte right_x_joy;
        byte right_y_joy;
    
    void setAnalogMode();           //Joy sticks work
    void setDigitalMode();          //Joy sticks disabled
    void setCommandMode();          //Enter mode to read data
    void setConfigMode();           //Enter mode to select analog/digital modes
		
	private:
		byte shift(byte _dataOut);

		byte _dataPin;
		byte _cmndPin;
		byte _attPin;
		byte _clockPin;
		
		byte _delay;
		byte _i;
		boolean _temp;
		byte _dataIn;
		
		byte _data1;
		byte _data2;
		unsigned int _dataOut;
};

#endif
