#include <LiquidCrystal.h>

// Define LCD pins
const int rs = 22, en = 23, d4 = 13, d5 = 12, d6 = 14, d7 = 27;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);       // Initialize LCD
  lcd.print("Test Start"); // Display test message
}

void loop() {
  // Do nothing
}

