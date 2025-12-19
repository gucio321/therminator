#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const auto DHT11_PIN = 5;
const auto DHT22_PIN = 4;
const auto LM35_PIN = A6;
const auto DS18B20_PIN = 3;
const auto NTC_PIN = A5;
const auto NTC_R = 180000/2;
const auto VREF = 1.0;
auto const SUPPLY_V = 5.0;

DHT dht11(DHT11_PIN, DHT11);
DHT dht22(DHT22_PIN, DHT22);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);

void setup() {
  Serial.begin(9600);
  dht11.begin();
  dht22.begin();
  ds18b20.begin();
  analogReference(AR_INTERNAL1V0);
}

void loop() {
  Serial.println("===========BEGIN============");
  dht11Read();
  dht22Read();
  lm35Read();
  ds18b20Read();
  ntcRead();
  Serial.println("============END=============");
  delay(1000);
}

void dht11Read() {
  Serial.print("DHT11: T = ");
  Serial.print(dht11.readTemperature());
  Serial.print(" *C\n");
}

void dht22Read() {
  Serial.print("DHT22: T = ");
  Serial.print(dht22.readTemperature());
  Serial.print(" *C\n");
}

void lm35Read() {
  Serial.print("LM35: T = ");
  auto rawVal = analogRead(LM35_PIN);
  auto voltage = (VREF / 1023.0) * rawVal; // Convert ADC value to voltage
  auto temp = voltage * 100.0; // LM35 outputs 10mV per degree Celsius
  Serial.print(temp);
  Serial.print(" *C\n");
}

void ds18b20Read() {
  ds18b20.requestTemperatures();
  delay(1500);
  Serial.print("DS18B20: T = ");
  Serial.print(ds18b20.getTempCByIndex(0));
  Serial.print(" *C\n");
  Serial.println(ds18b20.getDeviceCount());
}

void ntcRead() {
  double rawVal = analogRead(NTC_PIN);
  Serial.print("NTC: ADC = ");
  Serial.print(rawVal);
  Serial.print("\n");

  double ntcV = rawVal / 1023.0 * VREF;
  Serial.print("NTC: V = ");
  Serial.print(ntcV);
  Serial.print(" V\n");

  double ntcR = ntcV*NTC_R/((SUPPLY_V - ntcV));
  Serial.print("NTC: R = ");
  Serial.print(ntcR);
  Serial.print(" Ohm\n");

  double B = 3380000;
  double ntcT = B/(log(ntcR/NTC_R) + B/298.15) - 273.15;
  Serial.print("NTC: T = ");
  Serial.print(ntcT);
  Serial.print(" *C\n");
}
