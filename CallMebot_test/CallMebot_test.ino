#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "FireNation";
const char* password = "KingZuko3";

#define buttonPin1 0
#define ledPin1 20

int buttonState1 = 0;  // States for all buttons
int count_value1 = 0;  // Counters for all buttons
int prestate1 = 0;     // Previous states for all buttons
int onoff1 = 0;        // LED states for all LEDs

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+923355227853";
String apiKey = "8460610";

void sendMessage(String message) {
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.print("Message sent successfully");
  } else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 == LOW && prestate1 == 0) {
    button1();
    prestate1 = 1;
  } else if (buttonState1 == HIGH) {
    prestate1 = 0;
  }
}

void button1() {
  count_value1++;
  onoff1 = count_value1 % 2;
  Serial.print("count_value1: ");
  Serial.print(count_value1);
  Serial.print(" | onoff1: ");
  Serial.println(onoff1);
  if (onoff1 == 1) {
    digitalWrite(ledPin1, HIGH);  // Toggle LED 1 based on onoff1
  } else if (onoff1 == 0) {
    digitalWrite(ledPin1, LOW);  // Toggle LED 1 based on onoff1
  }

  // Create a message string with the count value and onoff status
  String message = "Count value: " + String(count_value1) + " | LED state: " + (onoff1 == 1 ? "ON" : "OFF");
  Serial.println("Sending message: " + message);

  // Send the message
  sendMessage(message);
}
