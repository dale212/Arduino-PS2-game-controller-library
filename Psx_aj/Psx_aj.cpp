/*  PSX Controller Decoder Library (Psx_aj.cpp)
 
 v2.0 March 26 2019 - fixed clock timing issue
 
 
 Original code PsX.j was written by Kevin Ahrendt
 Modifications by Dale Heatherington April 20, 2018
 
 Changed library name from PsX to Psx_aj.  The "aj" suffix means "Analog Joystick".
 This works with PS1 and PS2 compatible controllers.
 It's been tested on Arduino Uno and Adafruit feather M0 Arm boards.
 
 To make it work with the M0 Arm boards I had to add some small delays in
 the bit shifting code to make sure both rising and falling clock edges
 were within the valid comand bit period.
 
 I modified code to read analog joystick values and changed _dataIn bit shifts
 to left-shift because analog joystick values were in reverse bit order.
 Side effect: Now the digital button bits are reversed from the original
 so I changed the button hex defines to match.
 Analog joystick values are in public variables:
 right_x_joy, right_y_joy, left_x_joy, left_x_joy
 
 User must push "analog" button on controller to enable analog mode or enable
 it in software with setAnalogMode()  command.
 
 
 
	Original written by: Kevin Ahrendt June 22nd, 2008
	
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
#include "Psx_aj.h"

Psx_aj::Psx_aj()
{
}

byte Psx_aj::shift(byte _dataOut)		// Does the actual shifting, both in and out simultaneously
{
	_temp = 0;
	_dataIn = 0;

	for (_i = 0; _i <= 7; _i++)
	{
		
        //noInterrupts();
        
        
		if ( _dataOut & (1 << _i) ) digitalWrite(_cmndPin, HIGH);	// Writes out the _dataOut bits
		else digitalWrite(_cmndPin, LOW);
        
        delayMicroseconds(_delay/2);        //Need both falling and rising clock edge in bit time interval
        digitalWrite(_clockPin, LOW);
		
		
		delayMicroseconds(_delay);

		_temp = digitalRead(_dataPin);					// Reads the data pin
		if (_temp)
		{
			_dataIn = _dataIn | (B00000001 << _i);		// Shifts the read data left into _dataIn
		}

		digitalWrite(_clockPin, HIGH);
		delayMicroseconds(_delay);
        //interrupts();
	}
    
    delayMicroseconds(_delay * 2);
    return _dataIn;
}


void Psx_aj::setupPins(byte dataPin, byte cmndPin, byte attPin, byte clockPin, byte delay)
{
	pinMode(dataPin, INPUT);
	//digitalWrite(dataPin, HIGH);	// Turn on internal pull-up
	_dataPin = dataPin;

	pinMode(cmndPin, OUTPUT);
	_cmndPin = cmndPin;

	pinMode(attPin, OUTPUT);
	_attPin = attPin;
	digitalWrite(_attPin, HIGH);

	pinMode(clockPin, OUTPUT);
	_clockPin = clockPin;
	digitalWrite(_clockPin, HIGH);
	
	_delay = delay;
}


unsigned int Psx_aj::read()        //Read game pad buttons and joy sticks
{
    byte id;
	digitalWrite(_attPin, LOW);

	shift(0x01);
	id = shift(0x42);
	shift(0x00);

	_data1 = ~shift(0x00);
	_data2 = ~shift(0x00);
    
    if(id == 0x73){   //Check for "Analog" mode and get joy stick values
        right_x_joy =   shift(0x00);
        right_y_joy =   shift(0x00);
        left_x_joy =    shift(0x00);
        left_y_joy =    shift(0x00);
    }
    else              //If not Analog mode set joy stick values to neutral.
    {   right_x_joy =  0x80;
        right_y_joy =  0x80;
        left_x_joy =   0x80;
        left_y_joy =   0x80;
    }

    
    
	digitalWrite(_attPin, HIGH);

	_dataOut = (_data2 << 8) | _data1;
    
    delayMicroseconds(_delay * 10);
    

	return _dataOut;
}

void Psx_aj::setConfigMode()
{
    byte id;
    digitalWrite(_attPin, LOW);
    
    id = shift(0x01);
    id = shift(0x43);
    id = shift(0x00);
    id = shift(0x01);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    
    digitalWrite(_attPin, HIGH);
    delayMicroseconds(_delay);
}

void Psx_aj::setCommandMode()
{
    byte id;
    digitalWrite(_attPin, LOW);
    
    id = shift(0x01);
    id = shift(0x43);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    
    digitalWrite(_attPin, HIGH);
    delayMicroseconds(_delay);
}


void Psx_aj::setAnalogMode()
{
    byte id;
    
    setConfigMode();
    
    digitalWrite(_attPin, LOW);
    
    id = shift(0x01);
    id = shift(0x44);
    id = shift(0x00);
    id = shift(0x01);       //01 = set Analog mode
    id = shift(0x03);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    
    digitalWrite(_attPin, HIGH);
    delayMicroseconds(_delay);
    
    setCommandMode();
}


void Psx_aj::setDigitalMode()
{
    byte id;
    
    setConfigMode();
    
    digitalWrite(_attPin, LOW);
    
    id = shift(0x01);
    id = shift(0x44);
    id = shift(0x00);
    id = shift(0x00);       //00 = set Digital mode
    id = shift(0x03);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    id = shift(0x00);
    
    digitalWrite(_attPin, HIGH);
    delayMicroseconds(_delay);
    
    setCommandMode();
}



    
    
