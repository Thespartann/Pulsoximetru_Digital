// Pulsoximetru Digital - Proiect PCLP 2
// Realizat de: Dănilă Ștefan-Flavian
// Profesor coordonator: Damian Cătalin

#include <Wire.h> // Libraria pentru I2C
#include <LiquidCrystal_I2C.h> // Libraria pentru display
#include "MAX30100_PulseOximeter.h" // Libraria pentru Pulsoximetru
#include <SD.h> // Libraria pentru cardul SD
#include <SPI.h> // Libraria pentru interfata seriala SPI
#include <LibStef.h> // Libraria proprie
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
  Serial.println("Bataie detectata!");
}

void setup() {
  // Setăm numarul de coloane si linii ale LCD-ului
  lcd.begin(16, 2);
  lcd.createChar(1 , smile); // Emoticoane -- Vezi LibStef
  lcd.createChar(2 , mod);
  lcd.createChar(3 , sad);
  lcd.createChar(4, heart);  
  lcd.createChar(5, neutral);  
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print(" Proiect PCLP2");
  delay(3000);  // Wait for 3 seconds

  lcd.clear();
  lcd.print("Pulsoximetru ");
  lcd.setCursor(13, 0);
  lcd.write(4);  // Display the heart symbol
  delay(2000);
  lcd.clear();
  lcd.print("Realizat de:");
  delay(2000);
  lcd.setCursor(1, 1);
  lcd.print("Danila Stefan");
  delay(2000);
    lcd.clear();
  lcd.print("Profesor Coord.:");
  delay(2000);
  lcd.setCursor(1, 1);
  lcd.print("Damian Catalin");
  delay(2000);


  // Setăm pinul butonului ca INPUT_PULLUP
  // Pentru a folosi rezisența internă
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Asteapta ca butonul sa fie apasat
  lcd.clear();
  lcd.print("Apasa butonul");
  while (digitalRead(BUTTON_PIN) == HIGH) {
    // Asteapta ca butonul sa fie apasat
  }

  if (!pox.begin()) {
    Serial.println("FAILED");
    while (true);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

  if (!SD.begin(CHIP_SELECT_PIN)) {
    Serial.println("Initializare esuata!");
    lcd.setCursor(0, 1);
    lcd.print("Eroare SD");
    while (true);
  }
  Serial.println("Initializare reusita.");
}

void loop() {
  // Afiseaza numarul persoanei curente
  lcd.clear();
  lcd.print("Persoana 1");
  lcd.setCursor(0, 1);
  lcd.print("Inregistrare...");
  
  // Reseteaza ultimul timp cunoscut
  tsLastReport = millis();
  
  // Inregistreaza datele pentru 30 de secunde
  uint32_t startTime = millis();
  while (millis() - startTime < 30000) {
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      File dataFile = SD.open("data.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(pox.getHeartRate());
        dataFile.print(",");
        dataFile.println(pox.getSpO2());
        dataFile.close();
      } else {
        Serial.println("Eroare la deschiderea fișierului.");
      }
      lcd.clear();
      lcd.setCursor(0 , 0);
      lcd.print("BPM : ");
      lcd.print(pox.getHeartRate());
      lcd.setCursor(12, 0);
      lcd.write(4);
      lcd.setCursor(0 , 1);
      lcd.print("Sp02: ");
      lcd.print(pox.getSpO2());
      lcd.print("%");
      tsLastReport = millis();
          if (pox.getSpO2() >= 96) {
      lcd.setCursor(12 , 1);
      lcd.write(1);                 
    }
    else if (pox.getSpO2() <= 95 && pox.getSpO2() >= 91) {
      lcd.setCursor(12 , 1);
      lcd.write(2);                 
    }
    else if (pox.getSpO2() <= 90 && pox.getSpO2() >= 1 ) {
      lcd.setCursor(12 , 1);
      lcd.write(3);
    }
    else {
      lcd.setCursor(12 , 1);
      lcd.write(5);  // Display neutral symbol by default
    }
    }
        }
        // Calculul mediei din fisierul text data.txt
        File dataFileRead = SD.open("data.txt");
        if (dataFileRead) {
          int sumPuls = 0;
          int sumSaturatie = 0;
          int count = 0;

          while (dataFileRead.available()) {
            String dataString = dataFileRead.readStringUntil('\n');
            int commaIndex = dataString.indexOf(',');

            if (commaIndex != -1) {
              String value1 = dataString.substring(0, commaIndex);
              String value2 = dataString.substring(commaIndex + 1);

              int num1 = value1.toInt();
              int num2 = value2.toInt();

              sumPuls += num1;
              sumSaturatie += num2;
              count++;
            }
          }
          dataFileRead.close();

          if (count > 0) {
            float mediapuls = sumPuls / (float)count;
            float mediasaturatie = sumSaturatie / (float)count;

            lcd.print("Media puls: ");
            lcd.println(mediapuls);
            lcd.print("Media saturatie: ");
            lcd.println(mediasaturatie);
          } else {
            lcd.println("Nu exista date valide.");
          }
        } else {
          Serial.println("Eroare la deschiderea fisierului.");
        }
 
  // Asteapta butonul pentru restart
  lcd.setCursor(0, 1);
  lcd.print("Apasa butonul");
  while (digitalRead(BUTTON_PIN) == HIGH) {
    // Asteapta ca butonul sa fie apasat
  }
}
