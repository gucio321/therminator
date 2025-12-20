#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

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
LiquidCrystal_I2C lcd(0x27,  16, 2);

const int N_SENSORS = 5;
double record[N_SENSORS] = {0};
char** names = new char*[N_SENSORS] {
  (char*)"DHT11",
  (char*)"DHT22",
  (char*)"LM35",
  (char*)"DS18B20",
  (char*)"NTC"
};

enum SensorType {
  DHT11_SENSOR,
  DHT22_SENSOR,
  LM35_SENSOR,
  DS18B20_SENSOR,
  NTC_SENSOR
};

int currentPage = 0;

void setup() {
  Serial.begin(9600);
  dht11.begin();
  dht22.begin();
  ds18b20.begin();
  analogReference(AR_INTERNAL1V0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello from the");
  lcd.setCursor(0,1);
  lcd.print("Therminator!");
}

void loop() {
  Serial.println("===========BEGIN============");
  dht11Read();
  dht22Read();
  lm35Read();
  ds18b20Read();
  ntcRead();
  Serial.println("============END=============");
  // print the record on the screen
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(names[currentPage]);
  lcd.setCursor(0,1);
  lcd.print("T=");
  lcd.print(record[currentPage]);
  lcd.print(" C");

  currentPage++;
  if (currentPage >= N_SENSORS) {
    currentPage = 0;
  }

  delay(1000);
}

void dht11Read() {
  Serial.print("DHT11: T = ");
  double temp = dht11.readTemperature();
  record[DHT11_SENSOR] = temp;
  Serial.print(temp);
  Serial.print(" *C\n");
}

void dht22Read() {
  Serial.print("DHT22: T = ");
  record[DHT22_SENSOR] = dht22.readTemperature();
  Serial.print(record[1]);
  Serial.print(" *C\n");
}

void lm35Read() {
  Serial.print("LM35: T = ");
  auto rawVal = analogRead(LM35_PIN);
  auto voltage = (VREF / 1023.0) * rawVal; // Convert ADC value to voltage
  auto temp = voltage * 100.0; // LM35 outputs 10mV per degree Celsius
  record[LM35_SENSOR] = temp;
  Serial.print(temp);
  Serial.print(" *C\n");
}

void ds18b20Read() {
  ds18b20.requestTemperatures();
  delay(1500);
  Serial.print("DS18B20: T = ");
  record[DS18B20_SENSOR]= ds18b20.getTempCByIndex(0);
  Serial.print(record[DS18B20_SENSOR]);
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
  record[NTC_SENSOR] = ntcT;
  Serial.print(ntcT);
  Serial.print(" *C\n");
}
