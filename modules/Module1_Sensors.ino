#include <DHT.h>

// ---- Pin Definitions (adjust to match your wiring) ----
#define DHT_PIN          4
#define DHT_TYPE         DHT22
#define IR_SENSOR_1_PIN  32
#define IR_SENSOR_2_PIN  33

#define DHT_READ_INTERVAL_MS    60000UL
#define IR_CROSSING_TIMEOUT_MS  1500UL

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastDHTReadTime = 0;
float currentTemp = 0;
float currentHumidity = 0;

int occupancyCount = 0;

enum IRState { IR_WAITING, IR1_TRIGGERED, IR2_TRIGGERED };
IRState irState = IR_WAITING;
unsigned long irStateTimestamp = 0;

void readDHT22();
void updateOccupancy();

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(IR_SENSOR_1_PIN, INPUT);
  pinMode(IR_SENSOR_2_PIN, INPUT);
  Serial.println("Sensors module test started...");
}

void loop() {
  readDHT22();
  updateOccupancy();
  delay(50);
}

void readDHT22() {
  unsigned long now = millis();
  if (lastDHTReadTime == 0 || now - lastDHTReadTime >= DHT_READ_INTERVAL_MS) {
    lastDHTReadTime = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) {
      currentTemp = t;
      currentHumidity = h;
      Serial.print("Temp: "); Serial.print(currentTemp);
      Serial.print(" C  Humidity: "); Serial.print(currentHumidity);
      Serial.println(" %");
    } else {
      Serial.println("DHT22 read failed");
    }
  }
}

void updateOccupancy() {
  bool ir1 = digitalRead(IR_SENSOR_1_PIN) == LOW;  // flip to HIGH if your module's logic is opposite
  bool ir2 = digitalRead(IR_SENSOR_2_PIN) == LOW;
  unsigned long now = millis();

  if (irState != IR_WAITING && (now - irStateTimestamp > IR_CROSSING_TIMEOUT_MS)) {
    irState = IR_WAITING;
  }

  switch (irState) {
    case IR_WAITING:
      if (ir1 && !ir2) {
        irState = IR1_TRIGGERED;
        irStateTimestamp = now;
      } else if (ir2 && !ir1) {
        irState = IR2_TRIGGERED;
        irStateTimestamp = now;
      }
      break;

    case IR1_TRIGGERED:
      if (ir2) {
        occupancyCount++;
        Serial.print("Person IN. Count: "); Serial.println(occupancyCount);
        irState = IR_WAITING;
      }
      break;

    case IR2_TRIGGERED:
      if (ir1) {
        occupancyCount--;
        if (occupancyCount < 0) occupancyCount = 0;
        Serial.print("Person OUT. Count: "); Serial.println(occupancyCount);
        irState = IR_WAITING;
      }
      break;
  }
}
