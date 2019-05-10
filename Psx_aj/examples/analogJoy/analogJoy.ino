/*  Analog Joystick version PSX Controller Decoder Library (Psx_aj.ino)
 *   Modified by Dale Heatherington April 21 2018 - Added analog joystick functions.  
 *   This works with PS1 and PS2 game controllers and will probably work on all Arduinos
 *   I tested it with Arduino Uno and Adafruit M0 boards.
 *   
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

  PS2 wire colors
  1. Brown:  Data (PS2 output, needs 3.3K pullup resistor)
  2. Orange: Command (PS2 input)
  3. Gray:   Vibrate (not used)
  4. Black:  Ground
  5. Red:    Power 3.3V
  6. Yellow: Att (PS2 input. also called Select)
  7. Blue:   Clock (PS2 input)
  8. White:  N/C
  9. Green:  Ack (Not used)

  Logic levels are 3.3V
    
    
*/

#include <Psx_aj.h>          // Include the Psx_aj Library 
                                                          
//Sony Ps1 or Ps2 pins map to Arduino digital pins
// Any pins can be used since it is done in software

#define dataPin 9         //Brown
#define cmndPin 10       //Orange
#define attPin 11        //Yellow
#define clockPin 12     //Blue

#define LEDPin 13       //LED indicates button pushed

Psx_aj Psx;                                            // Initializes the library

unsigned int data = 0;                                    // data stores the controller response
char s[128];

void setup()
{
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 10);  // Defines what each pin is used
                                                          // (Data Pin #, Cmnd Pin #, Att Pin #, Clk Pin #, Delay)
                                                          // Delay measures how long the clock remains at each state,
                                                          // measured in microseconds.
                                                          // too small delay may not work (under 5)
  pinMode(LEDPin, OUTPUT);                                // Establishes LEDPin as an output so the LED can be seen
  Serial.begin(9600);

  Psx.setAnalogMode();                 //enable analog joysticks
  
}

void loop()
{
  data = Psx.read();   // Psx.read() reads joysticks and returns the button data in "data".  

  sprintf(s,"%03d %03d %03d %03d ",Psx.left_x_joy,Psx.left_y_joy,Psx.right_x_joy,Psx.right_y_joy);
  Serial.print(s);              //Print joystick values
  
  sprintf(s," %04x ",data);                                                      
  Serial.print(s);              // Print the returned 16 bit numeric button value in HEX
  


// Print which buttons are pressed. Button bit masks can be found in Psx_aj.h
  if(data & psxLeft) Serial.print(" Left ");
  if(data & psxDown) Serial.print(" Down ");
  if(data & psxUp) Serial.print(" Up ");
  if(data & psxRight) Serial.print(" Right");
  if(data & psxStrt) Serial.print(" Start ");
  if(data & psxSlct) Serial.print(" Select ");
  if(data & psxSqu) Serial.print(" Square");
  if(data & psxX) Serial.print(" X ");
  if(data & psxO) Serial.print(" O ");
  if(data & psxTri) Serial.print(" Triangle ");
  if(data & psxR1) Serial.print(" R1 ");
  if(data & psxL1) Serial.print(" L1 ");
  if(data & psxR2) Serial.print(" R2 ");
  if(data & psxL2) Serial.print(" L2 ");
  Serial.println();
  
  if (data != 0)                                          
                                                          
  {
    digitalWrite(LEDPin, HIGH);                           // If button is pressed, turn on the LED
  }
  else
  {
    digitalWrite(LEDPin, LOW);                            // If the button isn't pressed, turn off the LED
  }
  delay(100);
}
