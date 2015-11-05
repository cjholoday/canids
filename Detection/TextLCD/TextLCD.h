/* mbed TextLCD Library
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 */
 
#ifndef MBED_TEXTLCD_H
#define MBED_TEXTLCD_H

#include "Stream.h"
#include "DigitalOut.h"
#include "BusOut.h"

namespace mbed {

/* Class: TextLCD
 * A 16x2 Text LCD controller
 *
 * Allows you to print to a Text LCD screen, and locate/cls. Could be
 * turned in to a more generic libray.
 *
 * If you are connecting multiple displays, you can connect them all in
 * parallel except for the enable (e) pin, which must be unique for each
 * display.
 *
 * Example:
 * > #include "mbed.h"
 * > #include "TextLCD.h"
 * >
 * > TextLCD lcd(p24, p25, p26, p27, p28, p29, p30); // rs, rw, e, d0, d1, d2, d3
 * >
 * > int main() {
 * >     lcd.printf("Hello World!");
 * > }
 */
class TextLCD : public Stream {

public:
    /* Constructor: TextLCD
     * Create a TextLCD object, connected to the specified pins
     *
     * All signals must be connected to DigitalIn compatible pins. 
     *
     * Variables:
     *  rs -  Used to specify data or command
     *  rw - Used to determine read or write
     *  e - enable
     *  d0..d3 - The data lines
     */
	TextLCD(PinName rs, PinName rw, PinName e, PinName d0, PinName d1, 
		PinName d2, PinName d3, int columns = 16, int rows = 2);
#if 0 // Inhereted from Stream, for documentation only
    /* Function: putc
     *  Write a character
     *
     * Variables:
     *  c - The character to write to the serial port
     */
    int putc(int c);

    /* Function: printf
     *  Write a formated string
     *
     * Variables:
     *  format - A printf-style format string, followed by the
     *      variables to use in formating the string.
     */
    int printf(const char* format, ...);
#endif
        
    /* Function: locate
     * Locate to a certian position
     *
     * Variables:
     *  column - the column to locate to, from 0..15
     *  row - the row to locate to, from 0..1
     */
	virtual void locate(int column, int row);
    
    /* Function: cls
     * Clear the screen, and locate to 0,0
     */
	virtual void cls();	
    
	virtual void reset();
		
protected:

	void clock();
	void writeData(int data);
	void writeCommand(int command);
	void writeByte(int value);
	void writeNibble(int value);
	virtual int _putc(int c);		
	virtual int _getc();
	virtual void newline();				
			
	int _row;
	int _column;	
	DigitalOut _rw, _rs, _e;
	BusOut _d;
	int _columns;
	int _rows;

};

}

#endif
