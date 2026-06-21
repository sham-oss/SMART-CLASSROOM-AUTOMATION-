#include <Wire.h>
#include <RTClib.h>

#define BUZZER_PIN 23

// ---- Period end times (HH:MM) - replace with your actual school schedule ----
const int periodEndHour[]   = {9, 10, 11, 13, 14, 15};   // example: 6 periods
const int periodEndMinute[] = {45, 30, 15, 0, 45, 30};
const int NUM_PERIODS = sizeof(periodEndHour) / sizeof(periodEndHour[0]);

bool endTriggered[NUM_PERIODS];
bool reminderTriggered[NUM_PERIODS];
int lastCheckedDay = -1;

RTC_DS3231 rtc;
bool rtcFound = true;

void longBeep();
void shortBeeps(int count);
void resetDailyFlags();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!rtc.begin()) {
    Serial.println("DS3231 not found!");
    rtcFound = false;
  }

  resetDailyFlags();
  Serial.println("Buzzer module test started.");
}

void loop() {
  if (!rtcFound) { delay(1000); return; }

  DateTime now = rtc.now();

  // Reset flags once per new day so alerts can fire again tomorrow
  if (now.day() != lastCheckedDay) {
    resetDailyFlags();
    lastCheckedDay = now.day();
  }

  for (int i = 0; i < NUM_PERIODS; i++) {
    int nowTotalMin      = now.hour() * 60 + now.minute();
    int reminderTotalMin = periodEndHour[i] * 60 + periodEndMinute[i] - 5;

    // 5-minute-before reminder: three short beeps
    if (!reminderTriggered[i] && nowTotalMin == reminderTotalMin) {
      Serial.print("5-min reminder for period "); Serial.println(i + 1);
      shortBeeps(3);
      reminderTriggered[i] = true;
    }

    // Period-end alert: one long beep
    if (!endTriggered[i] && now.hour() == periodEndHour[i] && now.minute() == periodEndMinute[i]) {
      Serial.print("Period "); Serial.print(i + 1); Serial.println(" ended.");
      longBeep();
      endTriggered[i] = true;
    }
  }

  delay(1000); // checking once per second is enough since we're matching to the minute
}

void longBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void shortBeeps(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
}

void resetDailyFlags() {
  for (int i = 0; i < NUM_PERIODS; i++) {
    endTriggered[i] = false;
    reminderTriggered[i] = false;
  }
}
