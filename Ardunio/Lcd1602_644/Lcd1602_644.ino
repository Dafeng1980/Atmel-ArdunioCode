/* ------------------------------------------------------------------------------- */
// character LCD example code
// www.hacktronics.com

#include <LiquidCrystal.h>

// Connections:
// rs (LCD pin 4) to Arduino pin 12
// rw (LCD pin 5) to Arduino pin 11
// enable (LCD pin 6) to Arduino pin 10
// LCD pin 15 to Arduino pin 13
// LCD pins d4, d5, d6, d7 to Arduino pins 5, 4, 3, 2
const int numRows = 2;
const int numCols = 16;
LiquidCrystal lcd(24, 25, 27, 28, 29, 30);

int backLight = 26;    // pin 13 will control the backlight

void setup()
{
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
 // lcd.begin(16, 2);
    lcd.begin(numCols, numRows);
  //delay(10);// columns, rows.  use 16,2 for a 16x2 LCD, etc.
 // lcd.clear(); 
  //lcd.print("hello World");// start with a blank screen
 // delay(1000);
 //lcd.setCursor(0, 0); 
 //lcd.print("hello World");// set cursor to column 0, row 0 (the first row)
 //lcd.print("Line 1 text here");    // change this text to whatever you like. keep it clean.
  //lcd.setCursor(0, 1);          // set cursor to column 0, row 1
  //lcd.print("Line 2 text here");

  // if you have a 4 row LCD, uncomment these lines to write to the bottom rows
  // and change the lcd.begin() statement above.
  //lcd.setCursor(0,2);         // set cursor to column 0, row 2
  //lcd.print("Row 3");
  //lcd.setCursor(0,3);         // set cursor to column 0, row 3
  //lcd.print("Row 4");
}

void loop()
{
// loop from ASCII 'a' to ASCII 'z':
  for (int thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
    // loop over the columns:
    for (int  thisRow = 0; thisRow < numRows; thisRow++) {
      // loop over the rows:
      for (int thisCol = 0; thisCol < numCols; thisCol++) {
        // set the cursor position:
        lcd.setCursor(thisCol, thisRow);
        // print the letter:
        lcd.write(thisLetter);
        delay(200);
      }
    }
  }
}
