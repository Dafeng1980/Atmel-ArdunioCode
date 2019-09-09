/*
ShiftDisplay
by MiguelPynto
Arduino library for driving multiple-digit 7-segment LED displays using 74HC595 shift registers
https://miguelpynto.github.io/ShiftDisplay/
*/

#ifndef CharacterEncoding_h
#define CharacterEncoding_h

// encoding for characters segments (common cathode with LSBFIRST)
const byte NUMBERS[] = {
	//ABCDEFG
	 B00000011, // 0
	 B10011111, // 1
	 B00100101, // 2
	 B00001101, // 3
	 B10011001, // 4
	 B01001001, // 5
	 B01000001, // 6
	 B00011111, // 7
	 B00000001, // 8
	 B00011001  // 9
};

const byte LETTERS[] = {
	//ABCDEFG
	 B00010001, // a
	 B11000001, // b
	 B01100011, // c
	 B10000101, // d
	 B01100001, // e
	 B01110001, // f
	 B01000011, // g
	 B10010001, // h
	 B11110011, // i
	 B10000111, // j
	 B01010001, // k
	 B11100011, // l
	 B00010011, // m
	 B11010101, // n
	 B11000101, // o
	 B00110001, // p
	 B00011001, // q
	 B01110011, // r
	 B01001001, // s
	 B11100001, // t
	 B11000111, // u
	 B10000011, // v
	 B10000001, // w
	 B10010011, // x
	 B10001001, // y
	 B00100101 // z
};

const byte MINUS = B11111101;
const byte ULINE = B11101111;
const byte SHARP = B11110111;
const byte BLANK = B11111111;

// encoding for each index in display (common anode with LSBFIRST)
const byte INDEXES[] = {
	B10000000, // 0
	B01000000, // 1
	B00100000, // 2
	B00010000, // 3
	B00001000, // 4
	B00000100, // 5
	B00000010, // 6
	B00000001  // 7
};

#endif
