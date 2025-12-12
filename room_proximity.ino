#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "your wifi ssid"; 
char pass[] = "your pw";

#define PIR_SENSOR 13
#define BUZZER_PIN 25

BlynkTimer timer;

bool motionSent = false;  // Flag to avoid repeated alerts

void checkMotion() {
  int pirState = digitalRead(PIR_SENSOR);

  // -------------------------
  // MOTION DETECTED SECTION
  // -------------------------
  if (pirState == HIGH) {

    if (!motionSent) {  // Trigger only once
      Serial.println("🚨 Motion Detected!");

      // Check WiFi connection
      if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(BUZZER_PIN, HIGH);  // Local alarm
      } else {
        Blynk.logEvent("your event name in blynk", "Somebody near the stairs"); // Cloud alert
      }

      motionSent = true; // Prevent repeated alerts
    }
  }

  // -------------------------
  // NO MOTION SECTION
  // -------------------------
  else {
    if (motionSent) {
      Serial.println("✅ Safe Zone");
    }

    // Reset everything
    motionSent = false;
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  pinMode(PIR_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Faster response → check every 200 ms
  timer.setInterval(200L, checkMotion);

  Serial.println("🔒 Stair Proximity System Ready");
}

void loop() {
  Blynk.run();
  timer.run();
}
