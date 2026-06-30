// ── Monitor de nivel de agua y temperatura ───────────
// HC-SR04: mide distancia a la superficie del agua
// DS18B20: mide temperatura del agua

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
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  sensors.begin();

  // ── Diagnóstico DS18B20 ──────────────────────────
  int encontrados = sensors.getDeviceCount();
  Serial.print("Sensores DS18B20 encontrados: ");
  Serial.println(encontrados);

  if (encontrados == 0) {
    Serial.println("ERROR: Revisá el cableado y la resistencia pull-up del DS18B20");
  }
}

void loop() {
  // ── Medir distancia con HC-SR04 ──────────────────
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  // Diagnóstico HC-SR04
  Serial.print("Duration cruda HC-SR04: ");
  Serial.println(duration);

  float distancia;
  if (duration == 0) {
    distancia = -1.0;
  } else {
    distancia = duration * 0.034 / 2;
  }

  // ── Medir temperatura con DS18B20 ────────────────
  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

  if (temperatura == DEVICE_DISCONNECTED_C) {
    temperatura = -1.0;
  }

  // ── Enviar datos al ESP32 ─────────────────────────
  Serial.print(distancia, 1);
  Serial.print(",");
  Serial.println(temperatura, 1);

  delay(1000);
}
