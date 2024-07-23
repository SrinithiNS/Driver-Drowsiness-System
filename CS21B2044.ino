#define BLYNK_TEMPLATE_ID "TMPL35-5i4TMj"
#define BLYNK_TEMPLATE_NAME "Driver Drowsiness Detection System"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "x7sWDv_M1pB_KlhaSwrh662GGnYZxk1P"; // Blynk authentication token

const char* ssid = "Ananya";
const char* password = "Ananya@13";

const int eyeBlinkSensorPin = D2; // Pin connected to the eye blink sensor
const int buzzerPin = D3; // Pin connected to the buzzer

unsigned long lastBlinkTime = 0;
const unsigned long blinkThreshold = 3000; // Threshold for detecting drowsiness (in milliseconds)

BlynkTimer timer;

void setup() {
  pinMode(eyeBlinkSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Blynk.begin(auth, ssid, password);
  timer.setInterval(1000L, checkDrowsiness); // Check drowsiness every second

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkDrowsiness() {
  // Read the state of the eye blink sensor
  int eyeState = digitalRead(eyeBlinkSensorPin);

  // Check if eyes are closed
  if (eyeState == HIGH) {
    unsigned long currentTime = millis();
    // Check if eyes have been closed for more than the threshold
    if (currentTime - lastBlinkTime >= blinkThreshold) {
      // Eyes closed for more than threshold, drowsiness detected
      activateBuzzer();
      Blynk.logEvent("drowsy_alert", "Drowsiness detected!"); // Send notification to Blynk app
      Blynk.virtualWrite(V5, HIGH); // Turn on the LED in the Blynk app
    }
  } else {
    // Eyes are open, turn off the buzzer and LED
    deactivateBuzzer();
    lastBlinkTime = millis(); // Reset the last blink time when eyes are open
    Blynk.virtualWrite(V5, LOW); // Turn off the LED in the Blynk app
  }
}

void activateBuzzer() {
  digitalWrite(buzzerPin, HIGH);
  Serial.println("Drowsiness detected!");
}

void deactivateBuzzer() {
  digitalWrite(buzzerPin, LOW);
}\