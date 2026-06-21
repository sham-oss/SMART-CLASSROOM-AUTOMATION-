// ---- Relay Pins (3 partitions x light+fan) — adjust to match your wiring ----
#define RELAY_LIGHT1 25
#define RELAY_FAN1   26
#define RELAY_LIGHT2 27
#define RELAY_FAN2   14
#define RELAY_LIGHT3 12
#define RELAY_FAN3   13

// ---- Touch Switch Pins (manual override, one per relay) ----
#define TOUCH_LIGHT1 34
#define TOUCH_FAN1   35
#define TOUCH_LIGHT2 36
#define TOUCH_FAN2   39
#define TOUCH_LIGHT3 18
#define TOUCH_FAN3   19

#define AUTO_OFF_DELAY_MS  300000UL  // 5 minutes, as decided
#define DEBOUNCE_MS        200UL

const int relayPins[6]    = {RELAY_LIGHT1, RELAY_FAN1, RELAY_LIGHT2, RELAY_FAN2, RELAY_LIGHT3, RELAY_FAN3};
const int touchPins[6]    = {TOUCH_LIGHT1, TOUCH_FAN1, TOUCH_LIGHT2, TOUCH_FAN2, TOUCH_LIGHT3, TOUCH_FAN3};
const char* relayNames[6] = {"Light1", "Fan1", "Light2", "Fan2", "Light3", "Fan3"};

bool relayState[6]      = {false, false, false, false, false, false};
bool manualOverride[6]  = {false, false, false, false, false, false}; // true = locked to manual, auto logic skips it
bool lastTouchState[6]  = {false, false, false, false, false, false};
unsigned long lastTouchTime[6] = {0, 0, 0, 0, 0, 0};

int occupancyCount = 0;
unsigned long zeroOccupancySince = 0;
bool zeroTimerRunning = false;

void setRelay(int index, bool on) {
  relayState[index] = on;
  digitalWrite(relayPins[index], on ? HIGH : LOW); // flip HIGH/LOW if your relay module is active-LOW
  Serial.print(relayNames[index]); Serial.println(on ? " -> ON" : " -> OFF");
}

void checkTouchSwitches() {
  for (int i = 0; i < 6; i++) {
    bool pressed = digitalRead(touchPins[i]) == HIGH; // flip if your touch module is active-LOW
    unsigned long now = millis();
    if (pressed && !lastTouchState[i] && (now - lastTouchTime[i] > DEBOUNCE_MS)) {
      manualOverride[i] = true;      // locks this relay to manual control
      setRelay(i, !relayState[i]);   // toggle it
      lastTouchTime[i] = now;
    }
    lastTouchState[i] = pressed;
  }
}

void applyAutomaticControl() {
  unsigned long now = millis();

  if (occupancyCount > 0) {
    zeroTimerRunning = false;
    for (int i = 0; i < 6; i++) {
      if (!manualOverride[i] && !relayState[i]) setRelay(i, true);
    }
  } else {
    if (!zeroTimerRunning) {
      zeroTimerRunning = true;
      zeroOccupancySince = now;
    } else if (now - zeroOccupancySince >= AUTO_OFF_DELAY_MS) {
      for (int i = 0; i < 6; i++) {
        if (!manualOverride[i] && relayState[i]) setRelay(i, false);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 6; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);
    pinMode(touchPins[i], INPUT);
  }
  Serial.println("Relay Control module test started.");
  Serial.println("Type 'i' = person in, 'o' = person out, to simulate occupancy.");
}

void loop() {
  // Simulated occupancy for standalone testing — replace with sensors_getOccupancyCount() when merging modules
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'i') { occupancyCount++; Serial.print("Occupancy: "); Serial.println(occupancyCount); }
    if (c == 'o') { occupancyCount = max(0, occupancyCount - 1); Serial.print("Occupancy: "); Serial.println(occupancyCount); }
  }

  checkTouchSwitches();
  applyAutomaticControl();
  delay(50);
}
