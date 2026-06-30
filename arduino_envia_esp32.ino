// ── Monitor de nivel de agua y temperatura ───────────
// HC-SR04: mide distancia a la superficie del agua
// DS18B20: mide temperatura del agua
// Envía los datos al ESP32-C3 por UART (Serial)

#include <OneWire.h>
#include <DallasTemperature.h>

// ── Pines del HC-SR04 ──────────────────────────────
const int trigPin = 9;
const int echoPin = 8;

// ── Pines del DS18B20 ──────────────────────────────
const int oneWireBus = 2;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600); // Comunicación hacia el ESP32

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  sensors.begin();
}

void loop() {
  // ── Medir distancia con HC-SR04 ──────────────────
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  float distancia;
  if (duration == 0) {
    distancia = -1.0; // error: sin eco
  } else {
    distancia = duration * 0.034 / 2;
  }

  // ── Medir temperatura con DS18B20 ────────────────
  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

  if (temperatura == DEVICE_DISCONNECTED_C) {
    temperatura = -1.0; // error: desconectado
  }

  // ── Enviar datos al ESP32 ─────────────────────────
  // Formato: "distancia,temperatura"
  // Ejemplo: "177.5,24.3"
  Serial.print(distancia, 1);
  Serial.print(",");
  Serial.println(temperatura, 1);

  delay(1000); // cada segundo
}
