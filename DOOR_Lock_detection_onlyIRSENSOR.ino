#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* webhook_url = "https://discord.com/api/webhooks/your_webhook_url_here"; //discord Webhook

const int irSensorPin = D5; // GPIO14

bool lastState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(irSensorPin, INPUT);
  connectWiFi();
}

void loop() {
  bool currentState = digitalRead(irSensorPin);

  if (currentState != lastState) {
    if (currentState == LOW) {
      sendDiscordMessage("🔒 Lab Door Locked");
    } else {
      sendDiscordMessage("🔓 Lab Door Unlocked / Opened");
    }
    lastState = currentState;
    delay(2000); // debounce + prevent spam
  }

  delay(100);
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void sendDiscordMessage(String content) {
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect("discord.com", 443)) {
    Serial.println("Connection to Discord failed!");
    return;
  }

  String payload = "{\"content\":\"" + content + "\"}";

  client.println("POST /api/webhooks/your_webhook_url_path_here HTTP/1.1"); //discord Webhook url path in path
  client.println("Host: discord.com");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.println(payload);

  Serial.println("Sent to Discord: " + content);
}
