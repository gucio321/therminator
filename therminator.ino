#include <DHT.h>

const auto DHT_PIN = 5;

DHT dht11(DHT_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht11.begin();
}

void loop() {
  dht11Read();
  delay(1000);
}

void dht11Read() {
  Serial.print("DHT11: T = ");
  Serial.print(dht11.readTemperature());
  Serial.print(" *C\n");
}
