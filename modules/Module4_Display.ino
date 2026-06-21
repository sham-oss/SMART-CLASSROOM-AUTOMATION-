#include <Wire.h>
#include <RTClib.h>
#include <TM1637Display.h>

// ---- TM1637 4-digit display pins (adjust to match your wiring) ----
#define CLK_PIN 5
#define DIO_PIN 17

TM1637Display display(CLK_PIN, DIO_PIN);
RTC_DS3231 rtc;
bool rtcFound = true;

#define ROTATE_INTERVAL_MS 30000UL // 30 seconds, as decided
unsigned long lastSwitchTime = 0;
bool showingTime = true;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.setBrightness(0x0f);

  if (!rtc.begin()) {
    Serial.println("DS3231 not found!");
    rtcFound = false;
  }

  Serial.println("Display module test started.");
  Serial.println("Note: this 4-digit 7-segment display can only show numbers.");
  Serial.println("Rotating between TIME and DATE every 30 seconds instead of quotes.");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSwitchTime >= ROTATE_INTERVAL_MS) {
    showingTime = !showingTime;
    lastSwitchTime = now;
  }

  if (rtcFound) {
    DateTime t = rtc.now();
    if (showingTime) {
      int timeValue = t.hour() * 100 + t.minute();
      display.showNumberDecEx(timeValue, 0b01000000, true); // HH:MM with colon
    } else {
      int dateValue = t.day() * 100 + t.month();
      display.showNumberDecEx(dateValue, 0b01000000, true); // DD.MM
    }
  } else {
    display.showNumberDec(0, false); // fallback if RTC not detected
  }

  delay(500);
}
