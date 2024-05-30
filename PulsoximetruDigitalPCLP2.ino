// Pulsoximetru Digital - Proiect PCLP 2
// Realizat de: Dănilă Ștefan-Flavian
// Profesor coordonator: Damian Cătalin

#include <Wire.h> // Libraria pentru I2C
#include <LiquidCrystal_I2C.h> // Libraria pentru display

// Initializam LCD-ul
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup() {
  // Setăm numarul de coloane si linii ale LCD-ului
  lcd.begin(16, 2);
  lcd.clear();
  // Afisăm un mesaj pe LCD
  lcd.print("Pulsoximetru Digital");
}
void loop() {}
