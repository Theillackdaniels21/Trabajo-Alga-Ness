#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ----- LCD -----
LiquidCrystal_I2C lcd(0x27, 16, 2); // dirección común: 0x27

// ----- DHT11 -----
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----- Sensor salinidad -----
int sensorSalinidad = A0;

// ----- LEDs -----
int rojo = 8;
int verde = 10;
int amarillo = 9;

void setup() {
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);

  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();
}

void loop() {

  // ----- SALINIDAD -----
  int valorSal = analogRead(sensorSalinidad);

  Serial.print("Salinidad: ");
  Serial.println(valorSal);

  // LEDs segun salinidad
  digitalWrite(rojo, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(amarillo, LOW);

  if (valorSal < 341) {
    digitalWrite(verde, HIGH);
  }
  else if (valorSal < 682) {
    digitalWrite(amarillo, HIGH);
  }
  else {
    digitalWrite(rojo, HIGH);
  }

  // ----- DHT11 -----
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error leyendo DHT11");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C  Hum: ");
  Serial.println(hum);

  // ----- LCD -----
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(valorSal);

  lcd.print(" Temp:");
  lcd.print(temp, 1);

  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(hum, 1);

  lcd.print("%");

  delay(1000);
}