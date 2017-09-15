#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

// Is wifi sleeping
boolean sleep = true;

// Debouncer variables
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;  

// Init
void setup() {
  Serial.begin(9600);
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(btn, INPUT_PULLUP);

  digitalWrite(BUILTIN_LED, HIGH);
}

// Connect to wifi
void connectWifi() {
  flash(200, 3);

  WiFi.hostname("ESP8266-BRGNP");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  sleep = false;
}

// Send request to server and check if request succeeded 
void sendRequest() {
  bool fail = true;

  // Wake wifi if sleeping
  if (sleep) {
    WiFi.forceSleepWake();
    delay(200);
    connectWifi();
  }

  // Send request via HTTP
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  // Check the HTTP response code
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      if (payload == "done") {
        flash(2000, 1);
        fail = false;
      }
    }
  } else {
    Serial.println(httpCode);
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  if (fail) {
    flash(100, 5);
  }

  http.end();

  // Sent wifi back to sleep
  sleep = true;
  WiFi.forceSleepBegin();
  delay(500);
}

// Flash LED x times with a specific delay
void flash(int speed, int count) {
  bool state = false;

  for (int i = 0; i < count * 2; i++) {
    digitalWrite(BUILTIN_LED, state ? HIGH : LOW);
    state = !state;
    delay(speed);
  }
}

// Loop only contains debouncer and sendRequest call
void loop() {
  int reading = digitalRead(btn);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        sendRequest();
      }
    }
  }
  
  lastButtonState = reading;
}
