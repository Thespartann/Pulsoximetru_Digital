// Pulsoximetru Digital - Proiect PCLP 2
// Realizat de: Dănilă Ștefan-Flavian
// Profesor coordonator: Damian Cătalin

#include <Wire.h> // Libraria pentru I2C
#include <LiquidCrystal_I2C.h> // Libraria pentru display
#include "MAX30100_PulseOximeter.h" // Libraria pentru Pulsoximetru
#define REPORTING_PERIOD_MS     1000

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
}

void loop() {
    // Citim de la senzor
    pox.update();

    // Preia pulsul actualizat si saturatia oxigenului
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        led.print("Puls:");
        led.print(pox.getHeartRate());
        led.print("Puls / SpO2:");
        led.print(pox.getSpO2());
        led.println("%");

        tsLastReport = millis();
    }
}
