#include <Wire.h>
#include <RTClib.h>
#include <WiFi.h>

// ---- WiFi credentials (replace with your own) ----
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ---- Timezone: IST (UTC+5:30) ----
const long GMT_OFFSET_SEC      = 5 * 3600 + 30 * 60; // 19800
const int  DAYLIGHT_OFFSET_SEC = 0;
const char* NTP_SERVER         = "pool.ntp.org";

#define NTP_RESYNC_INTERVAL_MS 86400000UL // 24 hours, as decided

RTC_DS3231 rtc;
unsigned long lastNTPSyncMillis = 0;
bool rtcFound = true;

void connectWiFi();
void syncRTCWithNTP();
void printCurrentTime();

void setup() {
  Serial.begin(115200);
  Wire.begin(); // default ESP32 I2C pins: SDA=21, SCL=22

  if (!rtc.begin()) {
    Serial.println("DS3231 not found! Check wiring.");
    rtcFound = false;
  } else {
    Serial.println("DS3231 found.");
    if (rtc.lostPower()) {
      Serial.println("RTC lost power - time may be inaccurate until NTP sync.");
    }
  }

  connectWiFi();
  syncRTCWithNTP();
}

void loop() {
  if (millis() - lastNTPSyncMillis >= NTP_RESYNC_INTERVAL_MS) {
    if (WiFi.status() == WL_CONNECTED) syncRTCWithNTP();
  }

  printCurrentTime();
  delay(1000);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.status() == WL_CONNECTED
    ? "WiFi connected."
    : "WiFi not connected - RTC will run on its own battery-backed time.");
}

void syncRTCWithNTP() {
  if (WiFi.status() != WL_CONNECTED) return;

  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 10000)) {
    if (rtcFound) {
      rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    }
    lastNTPSyncMillis = millis();
    Serial.println("RTC synced with NTP (IST).");
  } else {
    Serial.println("Failed to get NTP time.");
  }
}

void printCurrentTime() {
  if (!rtcFound) {
    Serial.println("RTC not available.");
    return;
  }
  DateTime now = rtc.now();
  Serial.print(now.year());  Serial.print('-');
  Serial.print(now.month()); Serial.print('-');
  Serial.print(now.day());   Serial.print(' ');
  Serial.print(now.hour());  Serial.print(':');
  Serial.print(now.minute()); Serial.print(':');
  Serial.println(now.second());
}

// Helper functions for other modules (display, buzzer, occupancy reset) once merged:
int rtc_getHour()   { return rtc.now().hour(); }
int rtc_getMinute() { return rtc.now().minute(); }
int rtc_getSecond() { return rtc.now().second(); }
