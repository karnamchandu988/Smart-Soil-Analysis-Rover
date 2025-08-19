#include <LiquidCrystal.h>
#include <Servo.h>
#include "DHT.h"

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHTPIN 3  
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

int buz = 2;
int m1 = 6;
int m2 = 7;
int m3 = 4;
int m4 = 5;
int m5 = A0;
int m6 = A1;
int m7 = A2;
int m8 = A3;
int mos = A4;   // Soil moisture sensor
int ms  = A5;   // Metal sensor

int x;
int cnt = 0;

void setup() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(m5, OUTPUT);
  pinMode(m6, OUTPUT);
  pinMode(m7, OUTPUT);
  pinMode(m8, OUTPUT);
  pinMode(mos, INPUT);
  pinMode(ms, INPUT);
  pinMode(buz, OUTPUT);

  Serial.begin(9600);

  digitalWrite(m1, 0);
  digitalWrite(m2, 0);
  digitalWrite(m3, 0);
  digitalWrite(m4, 0);
  digitalWrite(m5, 0);
  digitalWrite(m6, 0);
  digitalWrite(m7, 0);
  digitalWrite(m8, 0);

  lcd.begin(16, 2);
  lcd.print("smart rover");
  delay(1500);
  lcd.clear();
  dht.begin();
}

void loop() {
  int t = dht.readTemperature();
  int h = dht.readHumidity();
  int mval = 1023 - analogRead(mos);
  int mtval = 1023 - analogRead(ms);

  // Metal detection
  if (mtval > 500) {
    digitalWrite(buz, 1);
  } else {
    digitalWrite(buz, 0);
  }

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:" + String(t) + " H:" + String(h));
  lcd.setCursor(0, 1);
  lcd.print("MOS:" + String(mval) + " MT:" + String(mtval));
  delay(500);

  // Send data to cloud
  cnt++;
  if (cnt > 10) {
    Serial.print("2838060,P6Z07T35NVBAGTEV,0,0,project1,12345678,"  
      + String(t) + "," + String(h) + ","  
      + String(mval) + "," + String(mtval) + "\n");
    cnt = 0;
  }

  // Movement control via Serial
  x = Serial.read();

  if (x == 1) { digitalWrite(m1, 1); digitalWrite(m2, 0); }
  if (x == 2) { digitalWrite(m1, 0); digitalWrite(m2, 1); }
  if (x == 3) { digitalWrite(m3, 1); digitalWrite(m4, 0); }
  if (x == 4) { digitalWrite(m3, 0); digitalWrite(m4, 1); }
  if (x == 5) {
    digitalWrite(m1, 0); digitalWrite(m2, 0);
    digitalWrite(m3, 0); digitalWrite(m4, 0);
    digitalWrite(m5, 0); digitalWrite(m6, 0);
    digitalWrite(m7, 0); digitalWrite(m8, 0);
  }
  if (x == 6) {
    digitalWrite(m5, 1); digitalWrite(m6, 0);
    digitalWrite(m7, 1); digitalWrite(m8, 0);
  }
  if (x == 7) {
    digitalWrite(m5, 0); digitalWrite(m6, 1);
    digitalWrite(m7, 0); digitalWrite(m8, 1);
  }
  if (x == 8) {
    digitalWrite(m5, 0); digitalWrite(m6, 1);
    digitalWrite(m7, 1); digitalWrite(m8, 0);
  }
  if (x == 9) {
    digitalWrite(m5, 1); digitalWrite(m6, 0);
    digitalWrite(m7, 0); digitalWrite(m8, 1);
  }
}
