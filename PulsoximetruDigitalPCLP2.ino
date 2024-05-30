// Pulsoximetru Digital - Proiect PCLP 2
// Realizat de: Dănilă Ștefan-Flavian
// Profesor coordonator: Damian Cătalin

#include <Wire.h> // Libraria pentru I2C
#include <LiquidCrystal_I2C.h> // Libraria pentru display
#include "MAX30100_PulseOximeter.h" // Libraria pentru Pulsoximetru
#include <SD.h> // Libraria pentru cardul SD
#include <SPI.h> // Libraria pentru interfata seriala SPI

#define REPORTING_PERIOD_MS     1000
#define CHIP_SELECT_PIN         10
#define BUTTON_PIN              9 

// Initializam LCD-ul
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Cream un obiect PulseOximeter
PulseOximeter pox;

// Timpul la care ultima bataia a avut loc
uint32_t tsLastReport = 0;

// Functia de apel se executa cand bataia e detectata
void onBeatDetected() {
    lcd.println("Bataie detectata!");
}

void setup() {
  // Setăm numarul de coloane si linii ale LCD-ului
  lcd.begin(16, 2);
  lcd.clear();
  // Afisăm un mesaj pe LCD
  lcd.print(" Initializăm pulsoximetrul.. ");
  // Initializăm sensorul
  if (!pox.begin()) {
      lcd.println("FAILED");
      for(;;);
  } else {
      lcd.println("SUCCESS");
  }
  // Configuram senzorul sa foloseasca 7.6mA pentru LED
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Inregistram o functie de apel
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Initializam cardul SD
    if (!SD.begin(CHIP_SELECT_PIN)) {
        Serial.println("Inițializare eșuată!");
        lcd.setCursor(0, 1);
        lcd.print("Eroare SD");
        while (1);
    }
    Serial.println("Inițializare reușită.");
    
    // Setăm pinul butonului ca INPUT_PULLUP
    // Pentru a folosi rezisența internă
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    // Așteptăm apăsarea butonului
    lcd.setCursor(0, 1);
    lcd.print("Apasa butonul pentru a incepe");
    while (digitalRead(BUTTON_PIN) == HIGH) {
        // Așteaptă până când butonul este apăsat
    } 
}

void loop() {
    // Citim de la senzor
    pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        tsLastReport = millis();

    // Preia pulsul actualizat si saturatia oxigenului
    lcd.print(" Puls: ");
    lcd.print(pox.getHeartRate());
    lcd.print(" SpO2: ");
    lcd.print(pox.getSpO2());
    lcd.print("%");


        // Salvam datele pe cardul SD
        File dataFile = SD.open("data.txt", FILE_WRITE);
        if (dataFile) {
            dataFile.print(pox.getHeartRate());
            dataFile.print(",");
            dataFile.println(pox.getSpO2());
            dataFile.close();
        } else {
            Serial.println("Eroare la deschiderea fișierului.");
        }
    }
}
