#include <LiquidCrystal.h>
#include "libADC.hpp"
float actualPi = 3.14159265358;
unsigned long previousMillis = 0;
int face = 0;
const long interval = 1000;
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;

const int pin_BL = 10;

uint16_t raw;

#define PIN_SEL 1
#define PIN_LEFT 2
#define PIN_NONE 0

LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

byte sad[8] = { 0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b01110, 0b10001, 0b10001 };

int what_button() {
  while (1) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
      ;
    raw = ADC;
    // SELECT
    if ((700 < raw) && (raw < 850)) {
      return PIN_SEL;
    }

    // LEFT
    if ((400 < raw) && (raw < 650)) {
      return PIN_LEFT;
    }
    return PIN_NONE;
  }
}

void setup() {
  lcd.begin(16, 2);      // initialize the lcd
  Serial.begin(115200);  // initialize serial
  ADC_Init();
  sei();
  lcd.createChar(1, sad);

  lcd.home();  // go home
  lcd.print("Hey! Wanna see ");
  lcd.setCursor(0, 1);  // go to the next line
  lcd.print("me calculate Pi?");
  delay(2500);
  lcd.clear();
  lcd.print("YES:PRESS SELECT");
  lcd.setCursor(0, 1);
  lcd.print("NO:PRESS LEFT");
}

double numofTerms = 100000;
float pi = 0;

void loop() {
  if (what_button() == 1) {
    int time = 0;

    Serial.println("Working...");
    for (float k = 0.0; k <= numofTerms; k++) {
      pi += 4.0 * (pow((-1.0), k)) * (1.0 / (2.0 * k + 1));

      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        lcd.home();
        lcd.clear();
        lcd.print(pi, 7);
        Serial.print("Iteration: ");
        Serial.print(k);
        Serial.print(" Pi = ");
        Serial.print(pi, 7);
        Serial.println("");
        //Serial.print(".");
        time++;
      }
      //  numofTerms++;
    }
    Serial.println(" ");
    Serial.println(" ");
    Serial.print("Final calculation of Pi is: ");
    Serial.print(pi, 7);
    lcd.setCursor(0, 1);

    Serial.println("");
    if (actualPi > pi) {
      float tempval = (actualPi - pi) / ((actualPi + pi) / 2) * 100;
      lcd.print("-");
      lcd.print(tempval, 4);

      Serial.println(tempval, 7);
      Serial.print("-");
    } else {
      float tempval = (pi - actualPi) / ((pi + actualPi) / 2) * 100;
      lcd.print("+");
      lcd.print(tempval, 4);
      Serial.println(tempval, 7);
      Serial.print("+");
    }
    lcd.print("%");
    delay(5000);
    lcd.setCursor(0, 1);
    lcd.print("in ");
    Serial.print(" Taking: ");
    Serial.print(time);
    lcd.print(time);
    lcd.print(" seconds");
    Serial.println(" Seconds to complete");
    Serial.println("Of course Pi really is 3.14159265358... but I had to look it up.");
    delay(10000);
    lcd.clear();
  }

  if (what_button() == 2) {
    delay(100);
    lcd.clear();
    lcd.print("What a pity!");
    lcd.setCursor(1,5);
    lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);lcd.write(1);

    for (int rightSideCounter = 0; rightSideCounter < 29; rightSideCounter++) {
      lcd.scrollDisplayRight();
      delay(2000);
    }  
    delay(1000);
    lcd.clear();
  }
}