**Psx_aj  An Arduino library for interfacing with a PS1 or PS2 game controller**

This is an improved version of code written by Keven Ahrendt June 22nd 2008.
* The analog joysticks are fully operational
* It works with both PS1 and PS2 controllers.
* It's been tested with Arduino ATMEGA328 and M0 ARM processors and should work with others.
* Interrupts are never disabled.

See example "analogJoy.ino" for programming details.

**PS2 wire colors**
```
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
```

To interface with 5V logic, pins 2,6 and 7 need to have a resistive voltage divider to reduce 5V to 3.3V.  
You can use an 820 ohm series resistor and 1.2K resistor to ground to do this.  
820 ohm resistor goes between Arduino digital out pin and PS2 input pin.  1.2k resistor goes between 
PS2 input pin and ground.
Or, you can use a logic level converter module.

